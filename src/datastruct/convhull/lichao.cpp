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

// Li Chao tree for convex hull trick
// https://cp-algorithms.com/geometry/convex_hull_trick.html
struct LiChao {
	vector<Line> tree; // Tree of lines
	vector<ll> xs; // x-coordinate of point i
	int k = 1; // Depth of the tree

	void init(const vector<ll>& points) : xs(points) {
		int n = points.size();
		while((1<<k) <= n) ++k;

		tree.resize(1 << k, {INF, 0});
		xs.resize(1 << k, points.back());
	}
	int mapInd(int j) const {
		int z = __builtin_ctz(j);
		return ((1<<(k-z)) | (j>>z)) >> 1;
	}
	void addLine(Line line) {
		int i = 1;
		int ia = 0;
		int ib = (1<<k)-2;
		while(true) {
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
	// Returns min(line.eval(xs[j])) over all lines added to the hull
	ll minVal(int j) const {
		ll res = INF;
		for (int i = mapInd(j+1); i > 0; i /= 2) {
			res = min(res, eval(tree[i], xs[j]));
		}
		return res;
	}
};


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
