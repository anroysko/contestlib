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
		vector<ll> p; // potential

		ll pushFlow(int ei, ll f) {
			edges[ei].u -= f;
			edges[ei ^ 1].u += f;
			return edges[ei].c * f;
		}

		vector<int> djikstra(PriQue& que, vector<ll>& dist) {
			vector<int> pre(p.size(), -1);
			while(true) {
				int i = que.pop().second;
				if (i == -1) return pre;
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
		}

		// Updates potentials to ensure that no negative cost arcs form when we push flow along the cycle
		vector<int> updatePotentials(int s) {
			PriQue que(n);
			vector<ll> dist(n, INF);
			dist[s] = 0;
			que.decKey(s, 0);

			auto res = djikstra(que, dist);
			for (int i = 0; i < n; ++i) p[i] += dist[i];
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
				dist[i] = -p[i];
				que.decKey(i, dist[i]);
			}
			djikstra(que, dist);
			for (int i = 0; i < n; ++i) p[i] += dist[i]; // |p[i] + dist[i]| <= nC
		}
	public:
		MinCostCirc(int nc) : n(nc), g(nc), p(nc, 0) {}
		
		void addEdge(int s, int t, ll u, ll c) {
			int i = edges.size();
			edges.emplace_back(t, u, c);
			edges.emplace_back(s, 0, -c);
			g[s].push_back(i);
			g[t].push_back(i^1);
		}
		
		ll solve() {
			ll res = 0;
			for (ll h = 1ll << 62; h > 0; h >>= 1) {
				for (int ei = 0; ei < edges.size(); ++ei) {
					Edge& ed = edges[ei];
					if (! (ed.ru & h)) continue;
					if (ed.u) ed.u += h;
					else {
						int s = edges[ei ^ 1].t;
						auto bp = updatePotentials(ed.t);
						ed.u += h;

						if (ed.c + p[s] - p[ed.t] < 0) {
							bp[ed.t] = ei;
							for (int i = s; ed.u > 0;) {
								res += pushFlow(bp[i], h);
								i = edges[bp[i] ^ 1].t;
							}
						}
						fixPotentials();
					}
				}
			}
			return res;
		}
		ll flow(int ei) const { return edges[2*ei ^ 1].u; }
};
