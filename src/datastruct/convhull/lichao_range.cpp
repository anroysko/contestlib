#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 1e18;

struct Line {
	ll a, b;
	ll eval(ll x) { return a*x + b; }
};
bool comp(const Line& l1, const Line& l2, ll x) {
	return l1.eval(x) < l2.eval(x);
}

// Li Chao tree for convex hull trick. Supports range updates and clearing ranges.
struct LiChao {
	vector<Line> tree; // Tree of lines
	vector<ll> xs; // x-coordinate of point i
	vector<bool> clear; // Should this subtree be cleared
	int k = 1; // Depth of the tree

	void init(const vector<ll>& points) : xs(points) {
		int n = points.size();
		while((1<<k) <= n) ++k;

		tree.resize(1 << k, {INF, 0});
		xs.resize(1 << k, points.back());
		clear.resize(1 << k, false);
	}
	void push(int i) {
		if (clear[i]) {
			clear[i] = false;
			line[i] = {INF, 0};
			if ((i<<1) < (1<<k)) {
				clear[2*i] = true;
				clear[2*i+1] = true;
			}
		}
	}
	int mapInd(int j) const {
		int z = __builtin_ctz(j);
		return ((1<<(k-z)) | (j>>z)) >> 1;
	}
	void addLine(Line line, int i, int ia, int ib) {
		while(true) {
			push(i);
			int mid = (ia + ib) >> 1;
			if (comp(line, tree[i], xs[mid])) swap(line, tree[i]);
			if (comp(line, tree[i], xs[ia])) {
				i = 2*i;
				ib = mid - 1;
			} else if (comp(line, tree[i], xs[ib])) {
				i = 2*i+1;
				ia = mid + 1;
			} else break;
		}
	}
	// if a <= ia && ib <= b, only one of the recursions can happen
	// Complexity: O(n log^2 n)
	void recAddLine(Line line, int a, int b, int i, int ia, int ib) {
		if (ib < a || b < ia || ib < ia) return;
		if (a <= ia && ib <= b) addLine(line, a, b, i, ia, ib);
		else {
			push(i);
			int mid = (ia + ib) >> 1;
			if (comp(line, tree[i], xs[mid]) && a <= mid && mid <= b) {
				swap(line, tree[i]);
			}
			if (comp(line, tree[i], xs[ia])) {
				addLine(line, a, b, 2*i, ia, mid-1);
			}
			if (comp(line, tree[i], xs[ib])) {
				addLine(line, a, b, 2*i+1, mid+1, ib);
			}
		}
	}

	// Given integer ib, Denote the segment tree intervals in [0, ib] by s_{1}, \dots, s_{k}.
	// We are also given m lines, m <= k, all of which start from s_{i} for some i, and end at ib.
	// We should add these lines to the tree

	// This is easy by binary searching 
	// running time is O(log^2(n) + m log(n))
	void addManyLines(vector<int, Line> lines, int ib) {
		// Build convex hull of lines, find which line is active in which range, and 
	}

	// Complexity: O(n log^2 n)
	void recClearRange(int a, int b, int i, int ia, int ib) {
		if (ib < a || b < ia) return;
		if (a <= ia && ib <= b) clear[i] = true;
		else {
			int mid = (ia + ib) >> 1;
			if (a <= mid && mid <= b) 
			// if mid contains a line, we need to be able to move it below
			// log^3 n 
		}
	}


	void clearRange(int a, int b) {
		recClearRange(a, b, 
	}
	void addLine(Line line, int a, int b) {
		recAddLine(line, a, b, 1, 0, (1<<k)-2);
	}
	// Returns min(line.eval(xs[j])) over all lines added to the hull
	ll minVal(int j) const {
		ll res = INF;
		for (int i = mapInd(j+1); i > 0; i /= 2) {
			res = min(res, eval(tree[i], xs[j]));
			if (clear[i]) res = INF;
		}
		return res;
	}
};

// Do we need range clearing?

int main() {
	int n, q;
	cin >> n >> q;

	vector<ll> xs(n);
	for (int i = 0; i < n; ++i) cin >> xs[i];
	LiChao hull(xs);

	for (int j = 0; j < q; ++j) {
		string op;
		cin >> op;
		if (op == "add_line") {
			ll a, b;
			cin >> a >> b; // ax + b
			hull.addLine({a, b});
		} else if (op == "min_val") {
			int i;
			cin >> i; // Zero-indexed position in xs[i]
			ll res = hull.minVal(i);
			cout << res << '\n';
		}
	}
}
