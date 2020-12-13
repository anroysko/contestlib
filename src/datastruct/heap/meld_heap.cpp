mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) { return uniform_int_distribution<T>(a, b)(rng); }
template<class T>
T rand() { return uniform_int_distribution<T>()(rng); }

// Meldable Heap
class MeldHeap {
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
		int meld(int a, int b) {
			if (a == -1 || b == -1) return a + b + 1;
			if (nodes[a].val > nodes[b].val) swap(a, b);
			if (nodes[a].inc) {
				add(nodes[a].ch[0], nodes[a].inc);
				add(nodes[a].ch[1], nodes[a].inc);
				nodes[a].inc = 0;
			}
			int x = rand(0, 1);
			nodes[a].ch[x] = meld(nodes[a].ch[x], b);
			return a;
		}
		pair<int, ll> top(int p) const { return {p, nodes[p].val}; }
		void pop(int& p) { p = meld(nodes[p].ch[0], nodes[p].ch[1]); }
};
