#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

int main() {
	int n, m;
	cin >> n >> m;
	
	MinCostCirc mcc(n);
	for (int i = 0; i < m; ++i) {
		int s, t;
		ll u, c;
		cin >> s >> t >> u >> c;
		mcc.addEdge(s, t, u, c);
	}

	ll res = mcc.minCostCirc();
	cout << res << '\n';

	for (int i = 0; i < m; ++i) cout << mcc.edgeFlow(i) << ' '; cout << '\n';
}
