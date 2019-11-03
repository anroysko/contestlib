#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll MOD = 1e9 + 7;

constexpr ll modPow(ll a, ll b, ll c) {
	return (b&1) ? (a * modPow(a, b-1, c) % c)
		: (b ? modPow(a*a % c, b/2, c) : 1);
}

// Struct for precalculating factorials, inverse factorials and nCr
template<int P>
struct FactPrecalc {
	private:
		vector<int> fact;
		vector<int> inv_fact;
	public:
		FactPrecalc(int n) : fact(n+1, 1), inv_fact(n+1, 1) {
			for (int i = 2; i <= n; ++i) fact[i] = (ll)fact[i-1] * i % P;
			inv_fact[n] = modPow(fact[n], P-2, P);
			for (int i = n-1; i >= 0; --i) inv_fact[i] = (ll)inv_fact[i+1] * (i+1) % P;
		}
		const int operator()(int a, int b) const {
			if (a < b) return 0;
			ll div = (ll)inv_fact[a-b] * inv_fact[b] % P;
			return fact[a] * div % P;
		}
		const int operator[](int i) const { return fact[i]; }
		int inv(int i) const { return inv_fact[i]; }
};

int main() {
	int n, m;
	cin >> n >> m;

	// fc[i]: i! mod P
	FactPrecalc<MOD> fc(n);
	for (int i = 0; i < n; ++i) cout << fc[i] << ' '; cout << '\n';

	// fc(a, b) : (a choose b) mod P
	for (int a = 0; a <= m; ++a) {
		for (int b = 0; b <= a; ++b) {
			cout << fc(a, b) << '\t';
		}
		cout << '\n';
	}
}
