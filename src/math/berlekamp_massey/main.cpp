#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll P = (ll)1e9 + 7;

#include "code.cpp"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;
	vector<ll> seq(n), rec(n + 1);
	for (ll& v : seq) cin >> v;
	for (ll& v : rec) {
		cin >> v;
		if (v < 0) v += P;
	}
	
	for (int i = n; i < m; ++i) {
		ll val = 0;
		for (int j = 1; j <= n; ++j) val = (val + seq[i-j] * rec[j]) % P;
		seq.push_back((val == 0 ? val : P - val));
	}

	BerlekampMassey<P> bk(seq);
	for (int i = 0; i < m; ++i) assert(seq[i] == bk.kthTerm(i));
}
