// Struct for priority queue operations on index set [1, n]
// push(i, v) overwrites value at position i if one already exists
// decKey is faster but requires that the new key is smaller than the old one
struct PriQue {
	const ll INF = 4 * (ll)1e18;
	vector<pair<ll, int>> data;
	const int n;
 
	PriQue(int siz) : n(siz), data(2*siz, {INF, -1}) { data[0] = {-INF, -1}; }
	void push(int i, ll v) {
		data[i+n] = {v, (v >= INF ? -1 : i)};
		for (i += n; i > 1; i >>= 1) data[i>>1] = min(data[i], data[i^1]);
	}
	void decKey(int i, ll v) {
		for (int j = i+n; data[j].first > v; j >>= 1) data[j] = {v, i};
	}
	pair<ll, int> pop() {
		auto res = data[1];
		push(res.second, INF);
		return res;
	}
};
 
// Capacity scaling algorithm for min cost circulation
// To solve minimum cost flow, add an edge from sink to source with cost -nC
// Complexity: O(m^2 log U log n), Space: O(m)
class MinCostCirc {
	private:
		struct Edge {
			const int t; // other endpoint
			const ll ru, c; // real capacity, cost
			ll u = 0; // capacity
			Edge(int tar, ll cap, ll cost) : t(tar), ru(cap), c(cost) {}
		};
 
		const ll INF = (ll)1e18; // must be > nC
		const int n;
		vector<Edge> edges;
		vector<vector<int>> g;
		vector<ll> p, dist; // potential
		vector<int> pre;
		PriQue que;
 
		void djikstra() {
			while(true) {
				int i = que.pop().second;
				if (i == -1) break;
				for (int ei : g[i]) {
					Edge& ed = edges[ei];
					ll off = dist[i] + (ed.c + p[i] - p[ed.t]);
					if (ed.u && off < dist[ed.t]) {
						pre[ed.t] = ei;
						dist[ed.t] = off;
						que.decKey(ed.t, off);
					}
				}
			}
			for (int i = 0; i < n; ++i) p[i] += dist[i];
		}
 
		// Updates potentials to ensure that no negative cost arcs form when we push flow along the cycle
		void updatePotentials(int s) {
			for (ll& v : dist) v = INF;
			dist[s] = 0;
			que.decKey(s, 0);
			djikstra();
		}
 
		// Fixes potentials to range [-nC, nC]
		// Add auxiliary node with distance -p[i] to node i, and apply p[i] += dist[i]
		// Note that -nC <= dist[i] + p[i] <= nC for all i at every point in the algorithm
		// No negative-weight cycles -> distances exist. Then p[t] <= p[i] + c, hence p[i] - p[t] + c >= 0
		void fixPotentials() {
			for (int i = 0; i < n; ++i) {
				dist[i] = -p[i];
				que.decKey(i, dist[i]);
			}
			djikstra();
		}

		ll incEdge(int ei, ll h) {
			ll res = 0;
			Edge& ed = edges[ei];
			if (ed.u) ed.u += h;
			else {
				int s = edges[ei ^ 1].t;
				updatePotentials(ed.t);
				ed.u += h;
 
				res = min(res, ed.c + p[s] - p[ed.t]);
				if (res < 0) {
					pre[ed.t] = ei;
					for (int i = s; ed.u > 0;) {
						edges[pre[i]].u -= h; // push flow
						edges[pre[i] ^ 1].u += h;
						i = edges[pre[i] ^ 1].t;
					}
				}
				fixPotentials();
			}
			return res;
		}
	public:
		MinCostCirc(int nc) : n(nc), g(nc), p(nc, 0), dist(n), pre(n), que(n) {}
		
		int addEdge(int s, int t, ll u, ll c) {
			int i = edges.size();
			edges.emplace_back(t, u, c);
			edges.emplace_back(s, 0, -c);
			g[s].push_back(i);
			g[t].push_back(i^1);
			return i >> 1;
		}
		ll edgeFlow(int ei) const { return edges[2*ei ^ 1].u; }
		
		// Finds min-cost circulation in O(m^2 log U log n)
		ll minCostCirc() {
			ll res = 0;
			for (ll h = 1ll << 62; h > 0; h >>= 1) {
				for (int ei = 0; ei < edges.size(); ++ei) {
					if (edges[ei].ru & h) res += incEdge(ei, h);
				}
			}
			return res;
		}
 
		// Finds min-cost maximum flow in O(nm + Fm log n) time (where F is the flow amount), assuming there are no negative-weight cycles
		pair<int, ll> minCostFlow(int so, int si) {
			for (auto& ed : edges) ed.u = ed.ru;
			for (int round = 0; round < n; ++round) {
				for (int ei = 0; ei < edges.size(); ++ei) {
					if (! edges[ei].u) continue;
					p[edges[ei].t] = min(p[edges[ei].t], p[edges[ei ^ 1].t] + edges[ei].c);
				}
			}
			int sse = 2 * addEdge(si, so, INF, -INF);
 
			ll off = 0;
			pair<int, ll> res = {0, 0};
			while(true) {
				off = incEdge(sse, 1);
				if (off >= 0) return res;
				else res = {res.first + 1, res.second + off + INF};
			}
		}
};
