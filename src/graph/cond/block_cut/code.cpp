void addEdge(int a, int b, vector<vector<int>>& g) {
	g[a].push_back(b);
	g[b].push_back(a);
}

pair<int, int> dfsBC(int i, vector<int>& dep, vector<vector<int>>& ans, const vector<vector<int>>& g) {
	pair<int, int> res = {-1, g.size()};
	for (auto t : g[i]) {
		if (dep[t] == -1) {
			dep[t] = dep[i] + 1;
			auto off = dfsBC(t, dep, ans, g);
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

// Given connected undirected graph, returns a tree s.t. x is on the path
// between y and z in the tree iff x is on every path between y and z in the input graph.
// A vertex is a cut vertex iff it has degree >= 2 in the tree.
// The tree has size <= |E| + 1. Complexity O(|E|)
vector<vector<int>> blockCut(const vector<vector<int>>& g) {
	vector<vector<int>> res(g.size());
	vector<int> dep(g.size(), -1);
	dep[0] = 0;
	dfsCD(0, dep, res, g);
	return res;
}
