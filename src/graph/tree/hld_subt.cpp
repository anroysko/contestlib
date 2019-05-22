#include <iostream>
#include <vector>
using namespace std;

// slight modification of Adamant's HLD code ( https://codeforces.com/blog/entry/53170 )
struct HLD {
	vector<int> par; // Parent
	vector<int> nxt; // Path parent
	vector<int> ind; // HLD index
	vector<int> siz; // Subtree size

	// Need equality in size check or heavy edge could go to parent
	void dfs1(vector<vector<int>>& ad, int i = 0) {
		for (int& t : ad[i]) {
			if (t == par[i]) continue;
			par[t] = i;
			dfs1(ad, t);
			siz[i] += siz[t];
			if (siz[t] >= siz[ad[i][0]]) swap(ad[i][0], t);
		}
	}
	void dfs2(vector<vector<int>>& ad, int& ti, int i = 0) {
		ind[i] = ti;
		++ti;
		for (auto t : ad[i]) {
			if (t == par[i]) continue;
			nxt[t] = (t == ad[i][0] ? nxt[i] : t);
			dfs2(ad, ti, t);
		}
	}
	// ad: nodes adjacent to node i. Can make it a reference if you don't care about changes
	HLD(vector<vector<int>> ad) : par(ad.size(), -1), nxt(ad.size(), 0), ind(ad.size(), 0), siz(ad.size(), 1) {
		dfs1(ad);
		int ti = 0;
		dfs2(ad, ti);
	}
	// Get intervals corresponding to path between a and b
	vector<pair<int, int>> pathQuery(int a, int b) {
		vector<pair<int, int>> res;
		while(true) {
			if (ind[b] < ind[a]) swap(a, b);
			if (ind[nxt[b]] <= ind[a]) {
				res.push_back({ind[a], ind[b]});
				return res;
			} else {
				res.push_back({ind[nxt[b]], ind[b]});
				b = par[nxt[b]];
			}
		}
	}
	// Get interval corresponding to subtree of node i
	pair<int, int> subTreeQuery(int i) {
		return {ind[i], ind[i] + siz[i] - 1};
	}
	// Get LCA of a and b
	int lca(int a, int b) const {
		while(true) {
			if (ind[b] < ind[a]) swap(a, b);
			if (ind[nxt[b]] <= ind[a]) return a;
			else b = par[nxt[b]];
		}
	}
};

// Example usage. Answers LCA-queries.
int main() {
	int n, q;
	cin >> n >> q;

	vector<vector<int>> conns(n);
	for (int i = 0; i < n-1; ++i) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		conns[a].push_back(b);
		conns[b].push_back(a);
	}
	HLD hld(conns);

	vector<int> rev(n);
	for (int i = 0; i < n; ++i) rev[hld.ind[i]] = i;

	for (int j = 0; j < q; ++j) {
		int a, b;
		cin >> a >> b;
		--a; --b;

		int res1 = rev[hld.pathQuery(a, b).back().first];
		int res2 = hld.lca(a, b);
		cout << res1 << ' ' << res2 << '\n';
	}
}
