
ll modPow(ll a, ll b, ll c) {
	if (b & 1) return modPow(a, b^1, c) * a % c;
	if (b > 0) return modPow(a*a % c, b >> 1, c);
	return 1;
}

// Garner's algorithm for chinese remainder theorem in O(|P|^2)
// https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
class Garner {
		vector<int> p;
		vector<vector<int>> inv;
	public:
		Garner(const vector<int>& primes) : p(primes), inv(primes.size()) {
			for (int i = 0; i < p.size(); ++i) {
				for (int j = 0; j < i; ++j) inv[i].push_back(modPow(p[j] % p[i], p[i] - 2, p[i]));
			}
		}
		ll eval(vector<int> r, int mod) {
			ll res = 1, mult = 1;
			for (int i = 0; i < p.size(); ++i) {
				for (int j = 0; j < i; ++j) {
					r[i] = ((ll)r[i] - r[j]) * inv[i][j] % p[i];
				}
				res = (res + mult * r[i]) % mod;
				mult *= p[i];
			}
			return (res < 0 ? res + mod : res);
		}
};
