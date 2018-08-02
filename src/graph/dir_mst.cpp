#include <iostream>
#include <utility>
#include <vector>
#include <queue>
using namespace std;
typedef long long ll;

// Must be N*N*V < 2e19, to avoid overflows.
// typedef __int128 ll for larger cases.
const int N = 1e5; // Node count
const int V = 1e9; // Max edge cost
const ll INF = (ll)N*V + 1;

struct Edge {
	int src;
	int tar;
	ll cost;
};
Edge makeEdge(int s, int t, ll c) {
	Edge res;
	res.src = s;
	res.tar = t;
	res.cost = c;
	return res;
}

// Finds minimum directed spanning tree of input graph ( http://www.cs.tau.ac.il/~zwick/grad-algo-13/directed-mst.pdf )
// Contracting runs in O(m log(n)), but expanding is O(n).
// Therefore you can construct k MST's each with arbitrary root in O(m log n + kn)
// To do this, make a copy of this data structure after calling contract.
struct DirMST {
	Edge* ed; // Edge data. Stored like this so copying is O(n)
	int n, m; // Edge and node count
	int cn; // Current node count

	vector<int> ined; // in edge, Edge leading to this node
	vector<int> par; // Parent component
	vector<int> group; // Highest-up supernode that contains this
	vector<ll> base; // Base cost for edges into node. Added to all costs in "ins".
	vector<vector<int>> childs; // Child components
	vector<priority_queue<pair<ll, int>>> ins; // All edges into this node, by cost.

	DirMST() { ed = nullptr; }
	~DirMST() { if (ed != nullptr) delete[] ed; }

	void init(int nodes, const vector<Edge>& edges) {
		n = nodes;
		m = edges.size() + n;
		ed = new Edge[m];
		for (int i = 0; i < edges.size(); ++i) ed[i] = edges[i];
		for (int i = 0; i < n; ++i) {
			ed[i + edges.size()] = makeEdge(i, (i+1)%n, INF);
		}
		cn = n;

		ined.resize(2*n);
		par.resize(2*n);
		group.resize(2*n);
		base.resize(2*n);
		childs.resize(2*n);
		ins.resize(2*n);

		for (int i = 0; i < 2*n; ++i) {
			ined[i] = -1;
			par[i] = i;
			base[i] = 0;
			group[i] = i;
		}
		for (int i = 0; i < m; ++i) {
			ins[ed[i].tar].push({-ed[i].cost, i});
		}
	}

	int coll(int i) {
		if (group[i] != i) group[i] = coll(group[i]);
		return group[i];
	}

	void contract() {
		int a = 0;
		while(ins[a].size() != 0) {
			auto el = ins[a].top();
			ins[a].pop();
			base[a] = el.first;
			// base[a] += el.first - base[a];
			int ei = el.second;

			int b = coll(ed[ei].src);
			if (a == b) continue;

			ined[a] = ei;
			if (ined[b] == -1) {
				a = b;
			} else {
				a = cn;
				++cn;
				for (int j = b;;) {
					par[j] = a;
					group[j] = a;
					childs[a].push_back(j);
					
					// Combine smaller into larger
					ll dif = base[j] - base[a];
					if (ins[j].size() > ins[a].size()) {
						dif = base[a] - base[j];
						base[a] = base[j];
						swap(ins[j], ins[a]);
					}
					while(! ins[j].empty()) {
						auto el = ins[j].top();
						ins[j].pop();
						ins[a].push({el.first - dif, el.second});
					}

					j = coll(ed[ined[j]].src);
					if (j == a) break;
				}
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
			int t = ed[ei].tar;
			ined[t] = ei;
			dismantle(t, vec);
		}

		vector<int> res (n-1);
		for (int i = 0; i < n; ++i) {
			if (i != r) {
				if (ined[i] > m-n) return {};
				res[i - (i > r)] = ined[i];
			}
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
		cin >> edges[i].src >> edges[i].tar >> edges[i].cost;
		--edges[i].src;
		--edges[i].tar;
	}
	
	/*
	// Find cost of minimum spanning tree with arbitrary root (O(m log(n)^2))
	edges.resize(m + n);
	for (int i = 0; i < n; ++i) edges[i + m] = makeEdge(n, i, INF);
	m += n;

	DirMST mst;
	mst.init(n + 1, edges);
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
	DirMST mst;
	mst.init(n, edges);
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
