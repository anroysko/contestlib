#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
	bool operator<(const Line& rhs) const {
		return a == rhs.a ? b < rhs.b : a < rhs.a;
	}
};

// Returns a1/b1 <= a2/b2 with no overflows assuming b1b2 doesn't overflow.
// Requires b1, b2 > 0. If you aren't scared of overflows, return a1b2 <= a2b1
bool divComp(ll a1, ll b1, ll a2, ll b2) {
	// return a1*b2 <= a2*b1;
	if (a1/b1 != a2/b2) return a1/b1 < a2/b2;
	else return (a1 % b1)*b2 <= (a2 % b2)*b1;
}

// Assumes le < mi <= ri and that mi isn't below le.
// Returns true if mi.eval(x) >= min(le.eval(x), ri.eval(x))
bool redundant(const Line& le, const Line& mi, const Line& ri) {
	if (mi.a == ri.a) return false;
	return divComp(le.b-mi.b, mi.a-le.a, mi.b-ri.b, ri.a-mi.a);
}

// Builds lower convex hull (line with minimum eval is in the hull)
vector<Line> buildHull(vector<Line> lines) {
	sort(lines.begin(), lines.end());
	vector<Line> res;
	for (auto li : lines) {
		for (int n = res.size(); n >= 2; --n) {
			if (redundant(res[n-2], res[n-1], li)) res.pop_back();
			else break;
		}
		if (res.empty() || li.a > res.back().a) res.push_back(li);
	}
	return res;
}

// Find line from convex hull minimizing line.eval(x)
Line binHull(const vector<Line>& hull, ll x) {
	int low = 0;
	int high = (int)hull.size() - 1;
	while(low != high) {
		int mid = (low + high) >> 1;
		if (hull[mid].eval(x) < hull[mid+1].eval(x)) high = mid;
		else low = mid + 1;
	}
	return hull[low];
}

int main() {
	int n, q;
	cin >> n >> q;

	vector<Line> lines(n);
	for (int i = 0; i < n; ++i) cin >> lines[i].a >> lines[i].b;
	vector<Line> hull = buildHull(lines);

	for (int qi = 0; qi < q; ++qi) {
		ll x;
		cin >> x;
		Line res = binHull(hull, x);
		cout << res.eval(x) << '\n';
	}
}
