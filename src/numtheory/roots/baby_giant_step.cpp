#include <iostream>
#include <cmath>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll = long long;
constexpr int MOD = (int)1e9 + 7;

ll modPow(ll a, ll b, ll c) {
	if (b & 1) return (a * modPow(a, b-1, c)) % c;
	if (b == 0) return 1;
	return modPow((a*a) % c, b/2, c);
}

// Baby-step Giant-step algorithm for discrete logarithm ( https://en.wikipedia.org/wiki/Baby-step_giant-step )
template<int P = MOD>
struct BabyGiantStep {
	gp_hash_table<int, int> vals;
	int ord = P-1;
	int ias;

	// O(P/k). Finds smallest t>0 s.t. a^t = b. If none exists, returns -1.
	int ask(int b) {
		for (int bi = 0; bi <= ord; bi += vals.size()) {
			auto it = vals.find(b);
			if (it != vals.end()) {
				return bi + it->second;
			}
			b = ((ll)b * ias) % P; // Giant step
		}
		return -1;
	}

	// O(k). Must have 0 < a < P.
	BabyGiantStep(int a, int k = sqrt(P))  {
		int mult = 1;
		for (int i = 1; i <= k; ++i) {
			mult = ((ll)mult * a) % P;
			vals[mult] = i;
			if (mult == 1) break;
		}
		ias = modPow(mult, P-2, P);
		ord = ask(1);
	}
};

int main() {
	int a, b;
	cin >> a >> b;

	BabyGiantStep<> step(a);
	cout << "ord(a): " << step.ord << '\n';

	int t = step.ask(b);
	if (t == -1) {
		cout << "IMPOSSIBLE\n";
	} else {
		cout << a << "^" << t << " = " << modPow(a, t, MOD) << " <-> " << b << '\n';
	}
}
