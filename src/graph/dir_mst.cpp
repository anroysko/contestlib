// Skew Heap
class SkewHeap {
	private:
		struct Node {
			ll val, inc = 0;
			int ch[2] = {-1, -1};
			Node(ll v) : val(v) {}
		};
		vector<Node> nodes;
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
		int merge(int a, int b) {
			if (a == -1 || b == -1) return a + b + 1;
			if (nodes[a].val > nodes[b].val) swap(a, b);
			if (nodes[a].inc) {
				add(nodes[a].ch[0], nodes[a].inc);
				add(nodes[a].ch[1], nodes[a].inc);
				nodes[a].inc = 0;
			}
			swap(nodes[a].ch[0], nodes[a].ch[1]);
			nodes[a].ch[0] = merge(nodes[a].ch[0], b);
			return a;
		}
		// "top" and pop -functions
		const Node& operator[](int i) const { return nodes[i]; }
		void pop(int& p) { p = merge(nodes[p].ch[0], nodes[p].ch[1]); }
};

struct Edge {
	int s, t;
	ll w;
	Edge(int src, int tar, ll weight) : s(src), t(tar), w(weight) {}
};

class DirMst {
	private:
		const vector<Edge> ed;
		vector<int> par, cmp, ins, vis;
		SkewHeap heap;
		int n, x = 0;
		
		void setp(int i, int p) { par[i] = p; cmp[i] = p; }
		int nxt(int x) { return getc(ed[ins[x]].s); }

		int getc(int i) {
			if (cmp[i] != i) cmp[i] = getc(cmp[i]);
			return cmp[i];
		}
		void setDI(int i, int t) {
			for (; vis[i] < t; i = par[i]) vis[i] = t;
		}
	public:
		// INPUT GRAPH MUST BE FULLY CONNECTED, AND CONTAIN NO SELF-LOOPS
		DirMst(int n_, const vector<Edge>& e) : n(n_), ed(e), par(2*n_), cmp(2*n_), ins(2*n_, -1), vis(2*n_) {
			for (int i = 0; i < 2*n; ++i) setp(i, i);
			for (int j = 0; j < ed.size(); ++j) ins[ed[j].t] = heap.merge(ins[ed[j].t], heap.makeNode(ed[j].w));
			for (int j = n; ins[x] != -1; ++j) {
				for (; !vis[x]; x = nxt(x)) vis[x] = 1;
				for (; x != j; x = nxt(x)) {
					heap.add(ins[x], -heap[ins[x]].val);
					ins[j] = heap.merge(ins[j], ins[x]);
					setp(x, j);
				}
				while(ins[x] != -1 && nxt(x) == x) heap.pop(ins[x]);
			}
		}

		// Returns the total cost of and the edges in the minimum directed spanning tree rooted at r
		pair<ll, vector<int>> unpack(int r) {
			setDI(r, vis[x] + 2);
			pair<ll, vector<int>> res;
			for (int i = x; i >= 0; --i) {
				if (vis[i] < vis[x]) {
					res.second.push_back(ins[i]);
					res.first += ed[ins[i]].w;
					setDI(ed[ins[i]].t, vis[x]);
				}
			}
			return res;
		}
};
