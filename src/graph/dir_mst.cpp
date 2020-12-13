#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

// Meldable Heap
class MeldHeap {
	private:
		struct Node {
			ll val, inc = 0;
			int ch[2] = {-1, -1};
			Node(ll v) : val(v) {}
		};
		vector<Node> nodes;

		void push(int i) {
			add(nodes[i].ch[0], nodes[i].inc);
			add(nodes[i].ch[1], nodes[i].inc);
			nodes[i].inc = 0;
		}
	public:
		// Make new node. Returns its index. Indexes go 0, 1, ...
		int makeNode(ll v) {
			nodes.emplace_back(v);
			return (int)nodes.size() - 1;
		}
		// Increment all values in heap p by v
		void add(int i, ll v) {
			if (i == -1) return;
			nodes[i].val += v;
			nodes[i].inc += v;
		}
		// Merge heaps a and b
		int meld(int a, int b) {
			if (a == -1 || b == -1) return a + b + 1;
			if (nodes[a].val > nodes[b].val) swap(a, b);

			push(a);
			int x = rand(0, 1);
			nodes[a].ch[x] = meld(nodes[a].ch[x], b);
			return a;
		}
		pair<int, ll> top(int p) const {
			return {p, nodes[p].val};
		}
		pair<int, ll> pop(int& p) {
			pair<int, ll> res = top(p);
			p = meld(nodes[p].ch[0], nodes[p].ch[1]);
			return res;
		}
};

struct Edge {
	int s, t;
	ll w;
	Edge(int src, int tar, ll weight) : s(src), t(tar), w(weight) {}
};

class DirMst {
	private:
		const vector<Edge> ed;
		vector<int> par, cmp, pick, ins;
		MeldHeap heap;
		int n, x = 0;
		
		void setp(int i, int p) { par[i] = p; cmp[i] = p; }
		int getc(int i) {
			if (cmp[i] != i) cmp[i] = getc(cmp[i]);
			return cmp[i];
		}
		int adv(int i) {
			while(ins[i] != -1) {
				int r = getc(ed[ins[i]].s);
				if (r != i) return r;
				else heap.pop(ins[i]);
			}
			return -1;
		}
		void setDI(int i, int t) {
			for (; cmp[i] < t; i = par[i]) cmp[i] = t;
		}
	public:
		// INPUT GRAPH MUST BE FULLY CONNECTED!
		DirMst(int n_, const vector<Edge>& e) : n(n_), ed(e), par(2*n_), cmp(2*n_), pick(2*n_), ins(2*n_, -1) {
			for (int i = 0; i < 2*n; ++i) setp(i, i);
			for (int j = 0; j < ed.size(); ++j) ins[ed[j].t] = heap.meld(ins[ed[j].t], heap.makeNode(ed[j].w));
			for (int j = n; adv(x) != -1; ++j) {
				for (; pick[x] < j; x = adv(x)) pick[x] = j;
				for (; x != j; x = getc(ed[pick[x]].s)) {
					ll w;
					tie(pick[x], w) = heap.pop(ins[x]);
					heap.add(ins[x], -w);
					ins[j] = heap.meld(ins[j], ins[x]);
					setp(x, j);
				}
			}
			for (int& v : cmp) v = -1; // reuse for unpack
		}

		// Returns the total cost of and the edges in the minimum directed spanning tree rooted at r
		pair<ll, vector<int>> unpack(int r) {
			setDI(r, cmp[x] + 1);
			pair<ll, vector<int>> res;
			for (int i = x; i >= 0; --i) {
				if (cmp[i] < cmp[x]) {
					res.second.push_back(pick[i]);
					res.first += ed[pick[i]].w;
					setDI(ed[pick[i]].t, cmp[x]);
				}
			}
			return res;
		}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, r;
	cin >> n >> m >> r;
	vector<Edge> edges;
	for (int i = 0; i < m; ++i) {
		int a, b;
		ll w;
		cin >> a >> b >> w;
		edges.emplace_back(a, b, w);
	}
	for (int i = 0; i < n; ++i) edges.emplace_back(i, r, 0); // make sure the graph is fully connected

	DirMst mst(n, edges);
	auto res = mst.unpack(r);

	cout << res.first << '\n';

	vector<int> ans(n);
	ans[r] = r;
	for (auto i : res.second) ans[edges[i].t] = edges[i].s;
	for (auto i : ans) cout << i << ' '; cout << '\n';
}
