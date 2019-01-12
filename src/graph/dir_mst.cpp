#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
using namespace std;
typedef long long ll;

struct Edge {
	int s; // source
	int t; // target
	ll c; // cost
};

// Finds minimum directed spanning tree of input graph ( http://www.cs.tau.ac.il/~zwick/grad-algo-13/directed-mst.pdf )
// Contracting runs in O(m log(n)^2), but expanding is O(n).
// Therefore you can construct k MST's each with arbitrary root in O(m log(n)^2 + kn)
// To do this, make copies of this structure and call expand on them.
class DirMST {
	const static int MAX_NODES = 1e4;
	const static int MAX_EDGE_COST = 1e9;

	int n, cn; // Initial and current node count
	vector<int> ined; // Edge leading to this node
	vector<int> par; // Parent component
	vector<int> group; // Highest-up supernode that contains this
	vector<ll> base; // Base cost for edges into node. Added to all costs in "ins"
	vector<vector<int>> childs; // Child components
	vector<priority_queue<pair<ll, int>>> ins; // All edges into this node, by cost

	int coll(int i) {
		if (group[i] != i) group[i] = coll(group[i]);
		return group[i];
	}
	void joinQues(int a, int b) {
		ll dif = base[b] - base[a];
		if (ins[b].size() > ins[a].size()) {
			dif = base[a] - base[b];
			base[a] = base[b];
			swap(ins[b], ins[a]);
		}
		while(! ins[b].empty()) {
			auto el = ins[b].top();
			ins[b].pop();
			ins[a].emplace(el.first - dif, el.second);
		}
	}
	void contract(const vector<Edge>& ed) {
		int a = 0; // Active node
		while(! ins[a].empty()) {
			auto el = ins[a].top();
			ins[a].pop();
			int ei = el.second;

			int b = coll(ed[ei].s);
			if (a == b) continue;

			ined[a] = ei;
			base[a] = el.first;
			if (ined[b] == -1) a = b;
			else {
				a = cn;
				++cn;
				do {
					par[b] = a;
					group[b] = a;
					childs[a].push_back(b);
					joinQues(a, b);
					b = coll(ed[ined[b]].s);
				} while(b != a);
			}
		}
	}
	void dismantle(int r, vector<int>& vec) {
		while(par[r] != r) {
			for (auto c : childs[par[r]]) {
				if (c == r) continue;
				par[c] = c;
				if (! childs[c].empty()) {
					vec.push_back(c);
				}
			}
			r = par[r];
		}
	}

	public:
	const static ll INF = (ll)MAX_NODES*MAX_EDGE_COST + 1; // cost assigned to nonexistent edges

	DirMST(int nn, vector<Edge> ed) : n(nn), cn(nn), ined(2*nn, -1),
			par(2*nn), base(2*nn, 0), childs(2*nn), ins(2*nn) {
		for (int i = 0; i < n; ++i) ed.push_back({(i+1)%n, i, INF+1});
		for (int i = 0; i < ed.size(); ++i) ins[ed[i].t].emplace(-ed[i].c, i);
		for (int i = 0; i < 2*n; ++i) par[i] = i;
		group = par;
		contract(ed);
	}
	// Find min directed spanning tree rooted at r. Returns {} if none exists
	vector<int> expand(int r, const vector<Edge>& ed) {
		vector<int> vec;
		dismantle(r, vec);
		int m = ed.size();
		for (int i = 0; i < vec.size(); ++i) {
			int ei = ined[vec[i]];
			int t = (ei >= m ? ei-m : ed[ei].t);
			ined[t] = ei;
			dismantle(t, vec);
		}

		vector<int> res;
		for (int i = 0; i < n; ++i) {
			if (i == r) continue;
			if (ined[i] >= m) return {};
			res.push_back(ined[i]);
		}
		return res;
	}
};

// Example usage
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	bool all;
	int n, m;
	cin >> n >> m >> all;
	vector<Edge> edges(m);
	for (int i = 0; i < m; ++i) {
		cin >> edges[i].s >> edges[i].t >> edges[i].c;
		--edges[i].s; --edges[i].t;
	}
	
	if (all) {
		// Find cost for all spanning trees (O(m log(n)^2 + n^2))
		DirMST mst(n, edges);
		for (int i = 0; i < n; ++i) {
			DirMST cpy = mst;
			vector<int> ans = cpy.expand(i, edges);

			if (ans.empty()) cout << "QAQ\n";
			else {
				ll res = 0;
				for (auto i : ans) res += edges[i].c;
				cout << res << '\n';
			}
		}
	} else {
		// Find cost of minimum spanning tree with arbitrary root (O(m log(n)^2))
		// to do this, add new node with inf-cost edges to all nodes, and expand on it.
		// For implementation details, note that this usecase is why edges in the struct are inited to INF+1.
		for (int i = 0; i < n; ++i) {
			edges.push_back({n, i, DirMST::INF});
		}
		DirMST mst(n+1, edges);
		vector<int> ans = mst.expand(n, edges);

		ll res = 0;
		int root = -1;
		if (ans.empty()) res = 2 * DirMST::INF;
		for (auto i : ans) {
			res += edges[i].c;
			if (res >= 2 * DirMST::INF) break;
			if (i >= m) root = i - m;
		}
		res -= DirMST::INF;

		if (res >= DirMST::INF) cout << "QAQ\n";
		else cout << res << ' ' << root+1 << '\n';
	}
}
