#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
typedef long long ll;
using namespace std;
const ll INF = 1e18;

// Li Chao tree for convex hull trick.
// Supports undoing. Is slower than the regular version though
// https://cp-algorithms.com/geometry/convex_hull_trick.html
// Lines are of form first + second * x
struct DynamicHull {
	vector<vector<pair<ll, ll>>> tree; // Tree of stacks of lines
	vector<ll> xcd; // x-coordinate of point i
	vector<int> changes;
	int h;

	void init(const vector<ll>& points) {
		int n = points.size();
		h = 1;
		while(h <= n) h <<= 1;

		xcd.resize(h);
		tree.resize(h);
		for (int i = 0; i < n; ++i) xcd[i] = points[i];
		for (int i = n; i < h; ++i) xcd[i] = points.back();
		for (int i = 0; i < h; ++i) tree[i] = {{INF, 0}};
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
			auto cur = tree[i].back();
			if (comp(line, cur, xcd[mid])) {
				tree[i].push_back(line);
				swap(line, cur);
				changes.push_back(i);
			}
			if (ia != ib) {
				if (comp(line, cur, xcd[ia])) {
					i = 2*i;
					ib = mid - 1;
					continue;
				} else if (comp(line, cur, xcd[ib])) {
					i = 2*i+1;
					ia = mid + 1;
					continue;
				}
			}
			break;
		}
	}
	ll getVal(ll x) const {
		int i = 1;
		int ia = 0;
		int ib = h-2;
		ll res = INF;
		while(true) {
			int mid = (ia + ib) >> 1;
			res = min(res, eval(tree[i].back(), xcd[x]));
			if (x < mid) {
				i = i<<1;
				ib = mid - 1;
			} else if (mid < x) {
				i = (i<<1)^1;
				ia = mid + 1;
			} else {
				break;
			}
		}
		return res;
	}

	// Undo all changes but the first s.
	// See example of usage in main.
	void undo(int s) {
		for (int i = s; i < changes.size(); ++i) {
			tree[changes[i]].pop_back();
		}
		changes.resize(s);
	}
};

int main() {
	vector<ll> xcd = {0, 2, 5, 11};
	DynamicHull hull;
	hull.init(xcd);

	// Should print: 4 13 25 49
	hull.addLine({4, 5});
	hull.addLine({5, 4});
	cout << hull.getVal(0) << ' ' << hull.getVal(1) << ' ' << hull.getVal(2) << ' ' << hull.getVal(3) << '\n';
	int os = hull.changes.size();
	
	// Should print: 4 13 24 37
	hull.addLine({9, 3});
	hull.addLine({15, 2});
	cout << hull.getVal(0) << ' ' << hull.getVal(1) << ' ' << hull.getVal(2) << ' ' << hull.getVal(3) << '\n';
	
	// Should print: 4 13 25 49
	hull.undo(os);
	cout << hull.getVal(0) << ' ' << hull.getVal(1) << ' ' << hull.getVal(2) << ' ' << hull.getVal(3) << '\n';
}
