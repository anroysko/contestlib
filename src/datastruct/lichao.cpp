#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
typedef long long ll;
using namespace std;
const ll INF = 1e18;

// Li Chao tree for convex hull trick.
// https://cp-algorithms.com/geometry/convex_hull_trick.html
// Lines are of form first + second * x
// See also version that supports undoing, in same directory.
struct DynamicHull {
	vector<pair<ll, ll>> tree; // Tree of lines
	vector<ll> xcd; // x-coordinate of point i
	int h;

	void init(const vector<ll>& points) {
		int n = points.size();
		h = 1;
		while(h <= n) h <<= 1;

		xcd.resize(h);
		tree.resize(h);
		for (int i = 0; i < n; ++i) xcd[i] = points[i];
		for (int i = n; i < h; ++i) xcd[i] = points.back();
		for (int i = 0; i < h; ++i) tree[i] = {INF, 0};
	}
	inline static ll eval(pair<ll, ll> line, ll point) {
		return line.first + line.second * point;
	}
	inline static bool comp(pair<ll, ll> l1, pair<ll, ll> l2, ll x) {
		return ((l1.first - l2.first) + x * (l1.second - l2.second)) < 0;
	}
	void addLine(pair<ll, ll> line) {
		int i = 1;
		int ia = 0;
		int ib = h-2;
		while(true) {
			int mid = (ia + ib) >> 1;
			if (comp(line, tree[i], xcd[mid])) swap(tree[i], line);
			if (ia != ib) {
				if (comp(line, tree[i], xcd[ia])) {
					i = 2*i;
					ib = mid - 1;
					continue;
				} else if (comp(line, tree[i], xcd[ib])) {
					i = 2*i+1;
					ia = mid + 1;
					continue;
				}
			}
			break;
		}
	}
	ll getVal(int j) const {
		int i = 1;
		int ia = 0;
		int ib = h-2;
		ll res = INF;
		while(true) {
			int mid = (ia + ib) >> 1;
			res = min(res, eval(tree[i], xcd[j]));
			if (j < mid) {
				i = i<<1;
				ib = mid - 1;
			} else if (mid < j) {
				i = (i<<1)^1;
				ia = mid + 1;
			} else {
				break;
			}
		}
		return res;
	}
};

int main() {
	vector<ll> xcd = {0, 2, 5, 11};
	DynamicHull hull;
	hull.init(xcd);

	hull.addLine({4, 5});
	hull.addLine({5, 4});
	hull.addLine({9, 3});
	hull.addLine({15, 2});
	// Should print: 4 13 24 37
	cout << hull.getVal(0) << ' ' << hull.getVal(1) << ' ' << hull.getVal(2) << ' ' << hull.getVal(3) << '\n';
}
