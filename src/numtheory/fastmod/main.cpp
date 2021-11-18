#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using uint = unsigned int;

#include "code.cpp"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	const int D = 131313;

	uint d;
	// d = D;
	cin >> d;
	FastMod mod(d);

	ull ans = 0;
	for (uint x = 0; x < (uint)1e9; ++x) {
		ans += mod(x);
		// ans += x % d;
	}
	cout << ans << '\n';
}
