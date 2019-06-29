#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

class EulerTour {
	private:
		vector<vector<int>> g;
		vector<int> ei;
		bool d;

		void recTour(int i, vector<int>& que) {
			while(ei[i] < g[i].size()) {
				int t = g[i][ei[i]];
				++ei[i];
				if ((!d) && (ei[t] >= g[t].size() || g[t][ei[t]] > i)) continue;
				recTour(t, que);
			}
			que.push_back(i);
		}
	public:
		EulerTour(const vector<vector<int>>& conns, bool dir = false) : g(conns), ei(conns.size(), 0), d(dir) {
			for (int i = 0; i < g.size(); ++i) sort(g[i].begin(), g[i].end());
		}

		vector<int> getTour(int i = 0) {
			vector<int> res;
			recTour(i, res);
			return res;
		}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;

	vector<vector<int>> g(n);
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	EulerTour et(g);
	vector<int> tour = et.getTour(); // First node appears at beginning and end of tour
	for (auto i : tour) cout << i << ' '; cout << '\n';
}
