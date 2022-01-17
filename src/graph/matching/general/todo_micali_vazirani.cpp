
// Micali-Vazirani algorithm: O(m sqrt(n)) max cardinality matching
class GeneralMatching {
	private:
		const static int INF = (int)1e9 + 7;
		
		struct Edge {
			const int a, b;
			int bridge = 0; // (-1, 0, 1) -> not bridge / unknown / bridge
			bool used;

			Edge(int a_, int b_, bool used_) : a(a_), b(b_), used(used_) {}
			int oth(int x) const { return a ^ b ^ x; }
		};

		struct Node {
			vector<int> g, pre;
			int even, odd, root;

			void clear(bool used) {
				pre.clear();
				pre.shrink_to_fit();
				even = (used ? INF : 0);
				odd = INF;
				root = -1;
			}
			int minv() const { return min(even, odd); }
			int maxv() const { return max(even, odd); }
		};

		const int n;
		vector<Edge> ed;
		vector<Node> ns;
		vector<vector<int>> bridges;

		void checkEdge(int j) {
			int a = ed[j].a;
			int b = ed[j].b;

			int d = INF;
			if (ed[j].used) d = ns[a].odd + ns[b].odd + 1;
			else d = ns[a].even + ns[b].even + 1;

			if (d < INF) {
				bridges[d].push_back(j);
				ed[j].bridge = 1;
			}
		}

		int findPaths() {
			for (auto& e : ed) e.bridge = 0;
			for (auto& vec : bridges) {
				vec.clear();
				vec.shrink_to_fit();
			}

			vector<int> que;
			for (int i = 0; i < n; ++i) {
				bool unused = 1;
				for (int j : ns[i].g) unused &= ed[j].used ^ 1;

				if (unused) que.push_back(i);
				ns[i].clear(! unused);
			}

			for (int d = 0; d < n; ++d) {
				// MIN-step
				vector<int> nxt;
				for (int i : que) {
					cerr << "Vertex " << i << " at d = " << d << endl;
					for (int j : ns[i].g) {
						if ((ed[j].used != (d & 1)) || (ed[j].bridge)) continue;

						int t = ed[j].oth(i);
						if (ns[t].minv() <= d) checkEdge(j);
						else {
							ed[j].bridge = -1;
							ns[t].pre.push_back(j);
							if (ns[t].pre.size() == 1) {
								if (d & 1) ns[t].even = d + 1;
								else ns[t].odd = d + 1;
								nxt.push_back(t);
							}
						}
					}
				}
				que = nxt;
				
				// MAX-step
				for (int j : bridges[2*d+1]) {
					cerr << "Bridge " << ed[j].a << ' ' << ed[j].b << " at d = " << d << endl;

					/*
					vector<pair<int, int>> sta_a, sta_b;
					sta_a.emplace_back(root[pr.first], -1);
					sta_b.emplace_back(root[pr.second], -1);
					while(true) {
						int a = sta_a.back().first;
						int b = sta_b.back().first;
						int da = minv(a), db = minv(b);
						if (a == b) {
							// Petal case
						} else if (max(da, db) == 0) {
							// Found augmenting path
						} else if (da < db) {
							// Move b down
						} else {
							// Move a down
						}
					}
					*/
				}
			}
			return 0; // TODO
		}
	public:
		GeneralMatching(int n_) : n(n_), ns(n_), bridges(2*n_) {}
		void addEdge(int a, int b, bool matched = 0) {
			ns[a].g.push_back(ed.size());
			ns[b].g.push_back(ed.size());
			ed.emplace_back(a, b, matched);
		}

		vector<int> getMatching() {
			int siz = 0;
			for (int pre = -1; pre < siz; siz += findPaths()) pre = siz;

			vector<int> res;
			for (int i = 0; i < ed.size(); ++i) {
				if (ed[i].used) res.push_back(i);
			}
			return res;
		}
};
