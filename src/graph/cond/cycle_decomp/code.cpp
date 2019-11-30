#include <iostream>
#include <vector>
using namespace std;

void addEdge(int a, int b, vector<vector<int>>& g) {
	g[a].push_back(b);
	g[b].push_back(a);
}

pair<int, int> dfsCD(int i, vector<int>& dep, vector<vector<int>>& ans, const vector<vector<int>>& g) {
	pair<int, int> res = {-1, g.size()};
	for (auto t : g[i]) {
		if (dep[t] == -1) {
			dep[t] = dep[i] + 1;
			auto off = dfsCD(t, dep, ans, g);
			if (off.first == -1) addEdge(i, t, ans);
			else {
				if (off.second < dep[i]) {
					if (res.first != -1) addEdge(res.first, off.first, ans);
					if (off.second < res.second) res = off;
				} else addEdge(i, off.first, ans);
			}
		} else if (dep[t] < dep[i] - 1) {
			if (res.first != -1) res.second = min(res.second, dep[t]);
			else {
				res = {ans.size(), dep[t]};
				ans.emplace_back();
			}
		}
	}
	if (res.first != -1) addEdge(i, res.first, ans);
	return res;
}

vector<vector<int>> cycleDecomp(const vector<vector<int>>& g) {
	vector<vector<int>> res(g.size());
	vector<int> dep(g.size(), -1);
	dep[0] = 0;
	dfsCD(0, dep, res, g);
	return res;
}

int main() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	vector<vector<int>> ans = cycleDecomp(g);
	int nn = ans.size();

	cout << nn << '\n';
	for (int i = 0; i < nn; ++i) {
		cout << i << ": ";
		for (auto t : ans[i]) cout << t << ' '; cout << '\n';
	}
}
