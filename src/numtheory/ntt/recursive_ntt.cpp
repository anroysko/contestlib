#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const int P = 998244353; // 2^21 | P-1
const int G = 3; // 3 is a generator of P

void ntt(vector<int>& v, int x_n) {
	int h = v.size()/2;
	vector<int> even(h);
	vector<int> odd(h);
	for (int i = 0; i < h; ++i) {
		even[i] = v[2*i];
		odd[i] = v[2*i+1];
	}

	if (h > 1) {
		int x_h = (ll)x_n*x_n % P;
		ntt(even, x_h);
		ntt(odd, x_h);
	}

	ll mult = 1; // (x_n)^i
	for (int i = 0; i < h; ++i) {
		v[i] = (even[i] + mult * odd[i]) % P;
		v[i+h] = (even[i] - mult * odd[i]) % P;
		if (v[i+h] < 0) v[i+h] += P;
		mult = mult*x_n % P;
	}
}

ll modPow(ll a, ll b) {
	if (b & 1) return a * modPow(a, b-1) % P;
	if (b == 0) return 1;
	return modPow(a*a % P, b / 2);
}

vector<int> polyMult(const vector<int>& a, const vector<int>& b) {
	int as = a.size();
	int bs = b.size();
	int n = 1;
	while(n < (as + bs)) n <<= 1;
	int x_n = modPow(G, (P-1)/n);
	int inv_x_n = modPow(x_n, P-2);
	int inv_n = modPow(n, P-2);

	vector<int> ap (n, 0);
	vector<int> bp (n, 0);
	for (int i = 0; i < as; ++i) ap[i] = a[i] % P;
	for (int i = 0; i < bs; ++i) bp[i] = b[i] % P;
	
	ntt(ap, x_n);
	ntt(bp, x_n);

	vector<int> cp(n);
	for (int i = 0; i < n; ++i) {
		ll prod = (ll)ap[i] * bp[i] % P;
		cp[i] = prod * inv_n % P;
	}

	ntt(cp, inv_x_n);

	cp.resize(as + bs - 1);
	return cp;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	int as, bs;
	cin >> as >> bs;
	vector<int> a(as);
	vector<int> b(bs);
	for (int i = 0; i < as; ++i) cin >> a[i];
	for (int i = 0; i < bs; ++i) cin >> b[i];
	
	vector<int> ab = polyMult(a, b);
	for (int i = 0; i < ab.size(); ++i) {
		cout << ab[i] << ' '; cout << '\n';
	}
}
