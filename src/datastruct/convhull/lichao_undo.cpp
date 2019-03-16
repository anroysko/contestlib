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
	int k;

	void init(const vector<ll>& points) {
		int n = points.size();
		k = 0;
		while((1<<k) <= n) ++k;
		int h = (1<<k);

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
        inline int mapInd(int j) const {
                int z = __builtin_ctz(j);
                return ((1<<(k-z)) | (j>>z)) >> 1;
        }
	void addLine(pair<ll, ll> line) {
		int i = 1;
		int ia = 0;
		int ib = (1<<k)-2;
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
	ll getVal(int j) const {
		int i = mapInd(j + 1);
		ll res = INF;
		while(i != 0) {
			res = min(res, eval(tree[i].back(), xcd[j]));
			i >>= 1;
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

// Example usage
// Solves https://cses.fi/ioi18/task/1424
typedef pair<pair<int, int>, pair<int, int>> Event;

int bins(const vector<ll>& vec, ll v) {
	int low = 0;
	int high = (int)vec.size() - 1;
	while(low < high) {
		int mid = (low + high) >> 1;
		if (vec[mid] < v) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return high;
}

const int Q = 4 * (int)1e5;
vector<int> asks[Q];
DynamicHull hull;

void dynCon(int ia, int ib, const vector<Event>& events) {
	vector<Event> left_events;
	vector<Event> right_events;
	int ics = hull.changes.size();
	int mid = (ia + ib) >> 1;
	for (auto it : events) {
		int itia = it.first.first;
		int itib = it.first.second;
		if (itia <= ia && ib <= itib) {
			hull.addLine(it.second);
		} else {
			if (itia <= mid) left_events.push_back(it);
			if (itib > mid) right_events.push_back(it);
		}
	}
	
	if (ia == ib) {
		for (auto it : asks[ia]) cout << hull.getVal(it) << '\n';
	} else {
		dynCon(ia, mid, left_events);
		dynCon(mid+1, ib, right_events);
	}
	hull.undo(ics);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, q;
	cin >> n >> q;
	vector<pair<ll, ll>> lines(n);
	for (int i = 0; i < n; ++i) cin >> lines[i].first;
	for (int i = 0; i < n; ++i) cin >> lines[i].second;
	
	vector<ll> xs;
	xs.reserve(q);
	vector<pair<int, int>> ques(q);
	for (int i = 0; i < q; ++i) {
		int t, v;
		cin >> t >> v;
		if (t == 3) {
			xs.push_back(v);
		} else {
			--v;
		}
		ques[i] = {t, v};
	}

	sort(xs.begin(), xs.end());
	xs.erase(unique(xs.begin(), xs.end()), xs.end());
	hull.init(xs);

	// Prepare events for offline dynamic connectivity
	int j = 0;
	bool lq = false; // Was last operation a query? Used to pack query indexes.
	vector<int> lap(n, 0); // Last Appearance
	vector<Event> events;
	events.reserve(q);
	for (int i = 0; i < q; ++i) {
		int v = ques[i].second;
		if (ques[i].first == 3) {
			lq = true;
			asks[j].push_back(bins(xs, v));
		} else {
			if (lq) ++j;
			lq = false;
			if (ques[i].first == 2) {
				lap[v] = j;
			} else {
				if (j > lap[v]) events.push_back({{lap[v], j-1}, lines[v]});
				lap[v] = Q;
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		if (lap[i] <= j) events.push_back({{lap[i], j}, lines[i]});
	}
	
	dynCon(0, j, events);
}
