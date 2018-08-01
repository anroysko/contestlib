#include <iostream>
#include <vector>
using namespace std;

// Merging segment tree ( https://codeforces.com/blog/fjzzq2002 )
// 0 is reserved as the "NONE" value.
struct MergSeg {
	struct Node {
		int childs [2];
		int siz;
		inline int& operator[](int i) { return childs[i]; }
	};

	vector<Node> seg; // Memory for nodes
	vector<int> empt; // Unassigned nodes

	// You can use this as a sparse segment tree, by setting mem yourself.
	void init(int n, int mem = -1) {
		if (mem == -1) {
			int h = 1;
			while((1<<(h-1)) < n) ++h;
			mem = n*h;
		}

		seg.resize(mem + 1);
		empt.reserve(mem);
		for (int i = mem; i >= 1; --i) empt.push_back(i);

		seg[0][0] = 0;
		seg[0][1] = 0;
		seg[0].siz = 0;
	}

	int makeNode() {
		// if (empt.empty()) cerr << "ALL NODES USED!\n";
		int i = empt.back();
		empt.pop_back();
		seg[i][0] = 0;
		seg[i][1] = 0;
		seg[i].siz = 0;
		return i;
	}

	// Joins segment trees a and b.
	// Amortized O(1)
	int join(int a, int b) {
		if ((a && b) == false) return a^b;
		seg[a][0] = join(seg[a][0], seg[b][0]);
		seg[a][1] = join(seg[a][1], seg[b][1]);
		seg[a].siz += seg[b].siz;
		empt.push_back(b);
		return a;
	}

	// Splits segment tree i into segment trees a and b such that a contains the leftmost k elements
	// Amortized O(log(n))
	void split(int i, int k, int& a, int& b) {
		if (k <= 0) {
			a = 0;
			b = i;
		} else if (k >= seg[i].siz) {
			a = i;
			b = 0;
		} else {
			int ls = seg[seg[i][0]].siz; // seg[0].siz = 0
			a = i;
			b = makeNode();
			split(seg[a][0], k, seg[a][0], seg[b][0]);
			split(seg[a][1], k-ls, seg[a][1], seg[b][1]);
			seg[b].siz = seg[i].siz - k;
			seg[a].siz = k;
		}
	}

	// Build segment tree with single node.
	// Can't give default arguments in struct functions :(
	int build(int v, int ia, int ib) {
		if (v < ia || ib < v) return 0;
		int i = makeNode();
		if (ia != ib) {
			int mid = (ia + ib) >> 1;
			seg[i][0] = build(v, ia, mid);
			seg[i][1] = build(v, mid+1, ib);
		}
		seg[i].siz = 1;
		return i;
	}

	// The query for this example usage
	int ask(int i, int x, int ia, int ib) {
		if (i == 0 || ia > x) return 0;
		if (ib <= x) return seg[i].siz;
		int mid = (ia + ib) >> 1;
		return ask(seg[i][0], x, ia, mid) + ask(seg[i][1], x, mid+1, ib);
	}
};

// Example usage, to solve TODO
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, q;
	cin >> n >> q;

	MergSeg seg;
	seg.init(n);

	vector<int> roots(n);
	for (int i = 0; i < n; ++i) roots[i] = seg.build(i, 0, n-1);
	
	for (int j = 0; j < q; ++j) {
		int t;
		cin >> t;
		if (t == 1) {
			int a, b;
			cin >> a >> b;
			--a; --b;
			
			roots[a] = seg.join(roots[a], roots[b]);
			roots[b] = -1;

		} else if (t == 2) {
			int a, b, k;
			cin >> a >> b >> k;
			--a; --b;
			seg.split(roots[a], k, roots[a], roots[b]);
		} else if (t == 3) {
			int a, x;
			cin >> a >> x;
			--a; --x;
			cout << seg.ask(roots[a], x, 0, n-1) << ' ';
		}
	}
	cout << '\n';
}
