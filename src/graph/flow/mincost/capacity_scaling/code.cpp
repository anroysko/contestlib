// Struct for priority queue operations on index set [1, n]
// push(i, v) overwrites value at position i if one already exists
// decKey is faster but requires that the new key is smaller than the old one
struct Prique {
	const ll INF = 4 * (ll)1e18;
	vector<pair<ll, int>> data;
	const int n;

	Prique(int siz) : n(siz), data(2*siz, {INF, -1}) { data[0] = {-INF, -1}; }
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
			const int s, t; // from, to
			const ll u, c; // capacity, cost
			ll cu = 0, f = 0; // current capacity, flow

			Edge(int src, int tar, ll cap, ll cost)
				: s(src), t(tar), u(cap), c(cost) {}
		};

		const int n;
		vector<Edge> edges;
		vector<vector<Edge>> g;
		vector<ll> p; // potential
		ll max_u = 0, max_c = 0; // Maximum edge capacity and absolute cost

		// Returns {other endpoint of edge, capacity in that direction, modified cost in that direction}
		tuple<int, ll, ll> getEdge(int i, const Edge& ed) const {
			if (i == ed.s) return {ed.t, ed.cu-ed.f, ed.c + p[ed.s] - p[ed.t]};
			else return {ed.s, ed.f, ed.c + p[ed.t] - p[ed.s]};
		}

		vector<int> djikstra(PriQue& que, vector<ll>& dist) {
			vector<int> pre(p.size(), -1);
			for (int i = que.pop().second; i != -1; i = que.pop().second) {
				for (int ei : g[i]) {
					int t;
					ll u, c;
					tie(t, u, c) = getEdge(i, edges[ei]);
					if (u > 0 && dist[i] + c < dist[t]) {
						pre[t] = ei;
						dist[t] = dist[i] + c;
						que.decKey(t, dist[i] + c);
					}
				}
			}
			return pre;
		}

		// Updates potentials to ensure that no negative cost arcs form when we push flow along the cycle
		// If potentials are at most M, they are at most 3M + nC afterwards.
		vector<int> updatePotentials(int s) {
			PriQue que(n);
			vector<ll> dist(n, INF);
			dist[s] = 0;
			que.decKey(s, 0);
			auto res = djikstra(que, dist);
			
			ll max_add = 0;
			for (auto v : dist) max_add = max(max_add, (v < INF ? v : 0));
			for (int i = 0; i < n; ++i) p[i] += min(max_add + max_c, dist[i]);
			return res;
		}

		// Fixes potentials to range [-nC, nC]
		// Add auxiliary node with distance -p[i] to node i, and apply p[i] += dist[i]
		// Note that -nC <= dist[i] + p[i] <= nC for all i at every point in the algorithm
		// No negative-weight cycles -> distances exist. Then p[t] <= p[i] + c, hence p[i] - p[t] + c >= 0
		void fixPotentials() {
			PriQue que(n);
			vector<ll> dist(n);
			for (int i = 0; i < n; ++i) {
				dist[i] -= p[i];
				que.decKey(i, dist[i]);
			}
			djikstra(que, dist);
			for (int i = 0; i < n; ++i) p[i] += dist[i]; // |p[i] + dist[i]| <= nC
		}
	public:
		/*
		MinCostCirc(int n, int s, int t) {}
		void addEdge() {} // update max_u
		*/
		
		ll solve() {
			ll res = 0;
			for (ll h = 1ll << (63 - __builtin_clzll(max_u)); h > 0; h >>= 1) {
				for (int ei = 0; ei < edges.size(); ++ei) {
					Edge& ed = edges[ei];
					if (! (ed.u & h)) continue;
					if (ed.f < ed.cu) ed.cu |= h;
					else {
						updatePotentials(ed.t);
						ed.cu |= h;
						if (p[ed.s] - p[ed.t] + ed.c < 0) {
							
						}
						fixPotentials();
					}
				}
			}
			return res;
		}
		ll getFlow(int ei) const { return edges[ei].f; }
};
