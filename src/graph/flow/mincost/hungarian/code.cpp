#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
const ll INF = 8 * (ll)1e18;

// Finds the minimum cost perfect matching in a full bipartite graph
// Time complexity: O(n^3)
struct Hungarian {
	// lp[i] + rp[j] <= weight[i][j]
	vector<ll> lp; // Left potential
	vector<ll> rp; // Right potential
	vector<vector<ll>> weight; // Weight of edge from i to j
	vector<int> lm; // Left match
	vector<int> rm; // Right match
	int n; // |L| = |R| = n

	bool extend(int j, const vector<ll>& sa, const vector<int>& pre, vector<int>& que) {
		if (sa[j] == 0) {
			if (rm[j] == -1) {
				// Augment the path
				while(j != -1) {
					int i = pre[j];
					rm[j] = i;
					swap(lm[i], j);
				}
				return true;
			}
			que.push_back(rm[j]);
		}
		return false;
	}
	void step() {
		vector<int> que; // Exposed left nodes, or left nodes reachable by alt. paths
		for (int i = 0; i < n; ++i) {
			if (lm[i] == -1) que.push_back(i);
		}
		vector<int> pre(n, -1); // Back pointers
		vector<ll> sa(n, INF); // Slack array; Min slack from node in que
		for (int ind = 0; ind < n; ++ind) {
			// BFS to new nodes
			int i = que[ind];
			for (int j = 0; j < n; ++j) {
				if (j == lm[i]) continue;
				ll off = weight[i][j] - (lp[i] + rp[j]); // Slack in edge
				if (off < sa[j]) {
					pre[j] = i;
					sa[j] = off;
					if (extend(j, sa, pre, que)) return;
				}
			}
			if (que.size() <= ind+1) {
				// Update potentials
				ll d = INF;
				for (int j = 0; j < n; ++j) {
					if (sa[j]) d = min(d, sa[j]);
				}
				bool found = false;
				for (auto i : que) lp[i] += d;
				for (int j = 0; j < n; ++j) {
					if (sa[j]) {
						sa[j] -= d;
						if (! found) found |= extend(j, sa, pre, que);
					} else rp[j] -= d;
				}
				if (found) return;
			}
		}
	}

	// Use the functions below:
	void init(const vector<vector<ll>>& weights) {
		n = weights.size();
		weight = weights;
		lp.resize(n, 0);
		rp.resize(n, 0);
		lm.resize(n, -1);
		rm.resize(n, -1);
	}
	vector<int> solve() {
		for (int i = 0; i < n; ++i) step();
		return lm;
	}
};

// Example usage
int main() {
	int n;
	cin >> n;

	vector<vector<ll>> weights(n);
	for (int i = 0; i < n; ++i) {
		weights[i].resize(n);
		for (int j = 0; j < n; ++j) {
			cin >> weights[i][j];
		}
	}

	Hungarian hung;
	hung.init(weights);

	vector<int> prs = hung.solve();
	ll ans = 0;
	for (int i = 0; i < n; ++i) ans += weights[i][prs[i]];
	cout << ans << '\n';
}
