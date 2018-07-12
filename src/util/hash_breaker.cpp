#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// ***
// Different methods for breaking rolling hashes.
// See the blog post by dacin21 ( https://codeforces.com/blog/entry/60442 ) for explanations
// All functions are of the form pair<vector<int>, vector<int>> func(n, const vector<int>& char_vals, ll mult, ll mod)
// so that they can be used in the combineHashes function.
// ***

uint32_t fastRandom(uint32_t& rs) {
	rs ^= rs << 16;
	rs ^= rs >> 5;
	rs ^= rs << 1;
	return rs;
}
ll rollHash(const vector<int>& vec, ll mult, ll mod) {
	lll res = 0;
	for (int i = 0; i < vec.size(); ++i) {
		res = (res * mult + vec[i]) % mod;
	}
	return res;
}
uint32_t genRandVec(vector<int>& vec, const vector<int>& char_vals, uint32_t rs) {
	for (int i = 0; i < vec.size(); ++i) vec[i] = char_vals[fastRandom(rs) % char_vals.size()];
	return rs;
}

// Calculates and returns the thue-morse sequence of length n, and it's inverse.
// Assumes n is a power of two
// Works against codes with overflow as modulo, or overflow-related bugs.
// mult and mod aren't used, but they are useful so this can be used as a function in the combineHashes function.
pair<vector<int>, vector<int>> thueMorse(int n, const vector<int>& char_vals, ll mult, ll mod) {
	vector<int> res (n);
	res[0] = 1;
	for (int j = 0; (1<<j) < n; ++j) {
		int len = (1<<j);
		for (int i = 0; i < len; ++i) res[i + len] = 1 - res[i];
	}
	vector<int> oth(n);
	for (int i = 0; i < n; ++i) oth[i] = char_vals[1 - res[i]];
	for (int i = 0; i < n; ++i) res[i] = char_vals[res[i]];
	return {res, oth};
}

// Does a basic birthday attack in time sqrt(mod).
// n needs to be large enough for this to work. In practice,
// n=6 works for sigma=26, mod=1e9+7, n=20 works for sigma=2, mod=1e9+7
pair<vector<int>, vector<int>> birthdayAttack(int n, const vector<int>& char_vals, ll mult, ll mod) {
	int steps = 1<<16;
	uint32_t rs = 1;
	while(true) {
		vector<pair<int, uint32_t>> hashes(steps);
		for (int i = 0; i < steps; ++i) {
			vector<int> vec(n);
			hashes[i].second = rs;
			rs = genRandVec(vec, char_vals, rs);
			hashes[i].first = rollHash(vec, mult, mod);
		}
		sort(hashes.begin(), hashes.end());
		for (int i = 0; i+1 < steps; ++i) {
			if (hashes[i].first == hashes[i+1].first) {
				vector<int> seg1(n);
				vector<int> seg2(n);
				genRandVec(seg1, char_vals, hashes[i].second);
				genRandVec(seg2, char_vals, hashes[i+1].second);
				if (seg1 != seg2) return {seg1, seg2};
			}
		}
		steps <<= 1;
	}
}

// Does a single-tree attack
// assumes n is a power of two. Can fail.
// We consider the rolling hash of S1 - S2. Therefore, we can pick a multiplier a \in {-1, 0, 1}
// for all of the numbers. Start by having all trees consist of a single node. Then sort trees by
// sum, and join adjanced trees, subtracting the first from the second.
pair<vector<int>, vector<int>> treeAttack(int n, const vector<int>& char_vals, ll mult, ll mod) {
	ll diff = char_vals[1] - char_vals[0];
	vector<lll> base_vals(n);
	base_vals[n-1] = 1;
	for (int i = n-2; i >= 0; --i) base_vals[i] = (base_vals[i+1] * mult) % mod;
	for (int i = 0; i < n; ++i) base_vals[i] = (base_vals[i] * diff) % mod;

	vector<int> group(n);
	vector<int> sign(n);
	vector<lll> sum(n);
	for (int i = 0; i < n; ++i) {
		group[i] = i;
		sign[i] = 1;
		sum[i] = base_vals[i];
	}
	for (int j = 0; (1<<j) < n; ++j) {
		vector<pair<ll, int>> actives;
		actives.reserve(n>>j);
		for (int i = 0; i < n; ++i) {
			if (group[i] == i) actives.push_back({sum[i], i});
		}
		sort(actives.begin(), actives.end());
		int zeroed = -1;
		for (int i = 0; 2*i < actives.size(); ++i) {
			int fi = actives[2*i].second;
			int se = actives[2*i+1].second;
			group[fi] = se;
			sign[fi] = -1;
			sum[se] -= sum[fi];
			if (sum[se] == 0) zeroed = se;
		}
		if (zeroed != -1) {
			for (int i = 0; i < n; ++i) {
				if (group[i] == i && i != zeroed) sign[i] = 0;
			}
			vector<int> res_sign (n);
			for (int i = 0; i < n; ++i) {
				int j = i;
				res_sign[i] = sign[j];
				while(group[j] != j) {
					j = group[j];
					res_sign[i] *= sign[j];
				}
			}
			vector<int> seg1(n, char_vals[0]);
			vector<int> seg2(n, char_vals[0]);
			for (int i = 0; i < n; ++i) {
				if (res_sign[i] == -1) seg2[i] = char_vals[1];
				if (res_sign[i] ==  1) seg1[i] = char_vals[1];
			}
			return {seg1, seg2};
		}
	}
	// We failed :(
	return {{},{}};
}
// Does a multi-tree attack
// Same as a single-tree attack, but calculates multiple values for each node.
// assumes n is a power of two
pair<vector<int>, vector<int>> multiTreeAttack(int n, const vector<int>& char_vals, ll mult, ll mod) {
	// Find all possible char val differences
	int sig = char_vals.size();
	vector<pair<lll, pair<int, int>>> diffs;
	diffs.reserve(sig * sig);
	for (int a = 0; a < sig; ++a) {
		for (int b = 0; b < sig; ++b) {
			if (a == b) continue;
			ll val = (char_vals[a] - char_vals[b]) % mod;
			if (val < 0) val += mod;
			diffs.push_back({val, {char_vals[a], char_vals[b]}});
		}
	}

	// Remove duplicates
	sort(diffs.begin(), diffs.end());
	int ds = diffs.size();
	int rem = 0;
	for (int i = 1; i + rem < ds; ++i) {
		swap(diffs[i], diffs[i + rem]);
		if (diffs[i].first == diffs[i-1].first) {
			++rem;
			--i;
		}
	}
	diffs.resize(ds - rem);
	ds = diffs.size();

	// Calculate modulo multipliers per index
	vector<lll> base_vals(n);
	base_vals[n-1] = 1;
	for (int i = n-2; i >= 0; --i) base_vals[i] = (base_vals[i+1] * mult) % mod;
	
	int m = 4;
	while(true) {
		// Initialize
		vector<int> group(2*n-1);
		vector<int> sign(2*n-1); // Is this the left or right child?
		vector<int> used(2*n-1); // which one of this node's possible trees was used?
		vector<vector<pair<ll, pair<int, pair<int, int>>>>> vals(2*n-1);

		for (int i = 0; i < n; ++i) {
			group[i] = i;
			vals[i].resize(ds);
			for (int j = 0; j < ds; ++j) {
				ll val = (diffs[j].first * base_vals[i]) % mod;
				vals[i][j] = {val, {1, diffs[j].second}};
			}
			sort(vals[i].begin(), vals[i].end());
			vals[i].resize(m);
			for (int j = ds; j < m; ++j) vals[i][j] = vals[i][j-1];
		}
		// Combine
		int base = 0;
		for (int j = 0; (1<<j) < n; ++j) {
			int len = n >> j;
			vector<pair<lll, int>> actives (len);
			for (int i = 0; i < len; ++i) {
				actives[i] = {vals[base + i][0].first, base + i};
			}
			sort(actives.begin(), actives.end());
			
			base += len;
			for (int ii = 0; ii < len; ii += 2) {
				int ind = base + ii / 2;
				vals[ind].reserve(m*(m+2));
				int le = actives[ii].second;
				int ri = actives[ii+1].second;
				sign[le] = 1;
				sign[ri] = -1;
				group[le] = ind;
				group[ri] = ind;
				for (int a = 0; a < m; ++a) {
					for (int b = 0; b < m; ++b) {
						// Offers where both sides have a value
						// Subtract smaller from larger
						ll lv = vals[le][a].first;
						ll rv = vals[ri][b].first;
						int sign = (lv < rv ? -1 : 1);
						ll val = abs(vals[le][a].first - vals[ri][b].first);
						vals[ind].push_back({val, {sign, {a, b}}});
					}
					// Offers where other side is zeroed out
					vals[ind].push_back({vals[le][a].first, {1, {a, -1}}});
					vals[ind].push_back({vals[ri][a].first, {-1,{-1, a}}});
				}

				// Sort offers, remove duplicates, crop to m
				// We might need to keep some duplicates to have m values.
				sort(vals[ind].begin(), vals[ind].end());
				
				rem = 0;
				for (int i = 1; i + rem < vals[ind].size(); ++i) {
					swap(vals[ind][i], vals[ind][i + rem]);
					if (vals[ind][i].first == vals[ind][i-1].first) {
						++rem;
						--i;
					}
				}
				vals[ind].resize(m);
				vals[ind].shrink_to_fit();
			}
		}
		// Success?
		if (vals.back()[0].first == 0) {
			sign.back() = 1;
			used.back() = 0;
			for (int i = 2*n-3; i >= 0; --i) {
				int pr = group[i];
				if (used[pr] == -1) {
					used[i] = -1; // Parent not used
					sign[i] = 1;
				} else {
					if (sign[i] ==  1) used[i] = vals[pr][used[pr]].second.second.first; // Left subtree
					if (sign[i] == -1) used[i] = vals[pr][used[pr]].second.second.second; // Right subtree
					sign[i] *= vals[pr][used[pr]].second.first;
				}
				sign[i] *= sign[pr];
			}
			vector<int> seg1(n);
			vector<int> seg2(n);
			for (int i = 0; i < n; ++i) {
				if (used[i] == -1) {
					seg1[i] = char_vals[0];
					seg2[i] = char_vals[0];
				} else {
					seg1[i] = vals[i][used[i]].second.second.first;
					seg2[i] = vals[i][used[i]].second.second.second;
				}
				if (sign[i] == -1) swap(seg1[i], seg2[i]);
			}
			return {seg1, seg2};
		}
		// Double cropoff and try again
		m *= 2;
		cout << "retrying multiTreeAttack with m=" << m << '\n';
	}
}



int main() {

	ll mult1 = 7;
	ll mod1 = 131;
	ll mult2 = 1113111;
	ll mod2 = 1e9 + 7;
	ll mult3 = 1283756671128375667LL;
	ll mod3 = 1364713457136471347LL;

	ll mult4 = 911382323;
	ll mod4 = (ll)972663749 << 32;

	vector<int> alphabet(26);
	for (int i = 0; i < 26; ++i) alphabet[i] = 'a' + i;

	auto sub = thueMorse(8, {'a', 'b'}, mult1, mod1);
	for (auto it : sub.first) cout << (char)it; cout << '\n';
	for (auto it : sub.second) cout << (char)it; cout << '\n';
	cout << rollHash(sub.first, mult1, mod1) << ' ' << rollHash(sub.second, mult1, mod1) << '\n';

	sub = birthdayAttack(20, {'a', 'b'}, mult2, mod2);
	for (auto it : sub.first) cout << (char)it; cout << '\n';
	for (auto it : sub.second) cout << (char)it; cout << '\n';
	cout << rollHash(sub.first, mult2, mod2) << ' ' << rollHash(sub.second, mult2, mod2) << '\n';

	sub = treeAttack(1<<12, {'a', 'b'}, mult3, mod3);
	for (auto it : sub.first) cout << (char)it; cout << '\n';
	for (auto it : sub.second) cout << (char)it; cout << '\n';
	cout << rollHash(sub.first, mult3, mod3) << ' ' << rollHash(sub.second, mult3, mod3) << '\n';
	
	sub = multiTreeAttack(1<<6, alphabet, mult4, mod4);
	for (auto it : sub.first) cout << (char)it; cout << '\n';
	for (auto it : sub.second) cout << (char)it; cout << '\n';
	cout << rollHash(sub.first, mult4, mod4) << ' ' << rollHash(sub.second, mult4, mod4) << '\n';
}
