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
// Contracting runs in O(m log(n)), but expanding is O(n).
// Therefore you can construct k MST's each with arbitrary root in O(m log n + kn)
// To do this, make a copy of this data structure after calling contract.
struct DirMST {
	// MAX_NODES^2 * MAX_COST must not overflow!
	const int MAX_NODES = 1e4; 
	const int MAX_COST = 1e9;
	const ll INF = (ll)MAX_NODES*MAX_COST + 1;

	int n, cn; // Initial and current node count
	vector<int> ined; // Edge leading to this node
	vector<int> par; // Parent component
	vector<int> group; // Highest-up supernode that contains this
	vector<ll> base; // Base cost for edges into node. Added to all costs in "ins"
	vector<vector<int>> childs; // Child components
	vector<priority_queue<pair<ll, int>>> ins; // All edges into this node, by cost
	shared_ptr<Edge[]> ed; // Edge data. Done like this for copying to work and be O(n).

	DirMST(int nn, const vector<Edge>& g) : n(nn), cn(nn), ined(2*nn, -1), par(2*nn), base(2*nn, 0),
			childs(2*nn), ins(2*nn), ed(new Edge[nn+g.size()], default_delete<Edge[]>()) {
		int m = n + g.size();
		for (int i = 0; i < n; ++i) ed[i] = {i, (i+1)%n, INF};
		for (int i = n; i < m; ++i) ed[i] = g[i-n];
		for (int i = 0; i < m; ++i) ins[ed[i].t].emplace(-ed[i].c, i);
		for (int i = 0; i < 2*n; ++i) par[i] = i;
		group = par;
	}
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
	void contract() {
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
				// Contract the cycle
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

	// Find min directed spanning tree rooted at r.
	// Returns {} if no tree exists.
	// To find minimum spanning tree with any root, add a new node
	// with edges of cost INF to all other nodes, then subtract
	// INF from the result.
	vector<int> expand(int r) {
		vector<int> vec;
		dismantle(r, vec);
		for (int i = 0; i < vec.size(); ++i) {
			int ei = ined[vec[i]];
			int t = ed[ei].t;
			ined[t] = ei;
			dismantle(t, vec);
		}

		vector<int> res;
		for (int i = 0; i < n; ++i) {
			if (i == r) continue;
			if (ined[i] < n) return {};
			res.push_back(ined[i]-n);
		}
		return res;
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;
	vector<Edge> edges(m);
	for (int i = 0; i < m; ++i) {
		cin >> edges[i].s >> edges[i].t >> edges[i].c;
		--edges[i].s;
		--edges[i].t;
	}
	
	/*
	// Find cost of minimum spanning tree with arbitrary root (O(m log(n)^2))
	edges.resize(m + n);
	for (int i = 0; i < n; ++i) edges[i + m] = makeEdge(n, i, INF);
	m += n;

	DirMST mst(n + 1, edges);
	mst.contract();
	vector<int> ans = mst.expand(n);

	ll res = 0;
	if (ans.empty()) res = 2 * INF;
	for (int i = 0; (res < 2 * INF) && (i < ans.size()); ++i) res += edges[ans[i]].cost;
	res -= INF;
	if (res >= INF) cout << "QAQ\n";
	else cout << res << '\n';
	*/

	/*
	// Find cost for all spanning trees (O(m log(n)^2 + n^2))
	DirMST mst(n, edges);
	mst.contract();

	for (int i = 0; i < n; ++i) {
		DirMST cpy = mst;
		vector<int> ans = cpy.expand(i);
		
		if (ans.empty()) {
			cout << "QAQ ";
		} else {
			ll res = 0;
			for (auto i : ans) res += edges[i].cost;
			cout << res << ' ';
		}
	}
	cout << '\n';
	*/
}
