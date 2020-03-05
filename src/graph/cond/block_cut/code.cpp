void addEdge(int i, int j, vector<vector<int>>& ans) {
	ans[i].push_back(j);
	ans[j].push_back(i);
}

pair<int, int> dfsBC(int i, int p, vector<int>& dep, vector<vector<int>>& ans, const vector<vector<int>>& g) {
	if (dep[i] != -1) return {dep[i], -1};
	else dep[i] = dep[p] + 1;
	addEdge(i, i + g.size(), ans);

	int res = dep[i];
	for (auto t : g[i]) {
		auto sub = dfsBC(t, i, dep, ans, g);
		res = min(res, sub.first);
		if (sub.second != -1) addEdge(sub.second, t + g.size(), ans);
	}
	return {res, p + g.size() * (res < dep[p])};
}

// Given connected undirected graph, returns a tree s.t. x is on the path
// between y and z in the tree iff x is on every path between y and z in the input graph.
// A vertex is a cut vertex iff it has degree >= 2 in the tree.
// The tree has size 2|V|. Complexity O(|E|)
vector<vector<int>> blockCut(const vector<vector<int>>& g) {
	vector<vector<int>> res(2 * (int)g.size());
	vector<int> dep(g.size(), -1);
	dfsBC(0, 0, dep, res, g);
	return res;
}
