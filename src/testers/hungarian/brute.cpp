#include <iostream>
using namespace std;
typedef long long ll;

const ll INF = 8 * (ll)1e18;
const int N = 20;
ll weight[N][N];
bool used[N];

ll brute(int i, int n) {
	if (i == n) return 0;
	ll res = INF;
	for (int t = 0; t < n; ++t) {
		if (! used[t]) {
			used[t] = true;
			res = min(res, weight[i][t] + brute(i+1, n));
			used[t] = false;
		}
	}
	return res;
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) cin >> weight[i][j];
	}

	cout << brute(0, n) << '\n';
}
