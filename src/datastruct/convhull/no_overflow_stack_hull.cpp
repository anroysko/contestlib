struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
	bool operator<(const Line& rhs) const {
		return a == rhs.a ? b < rhs.b : a < rhs.a;
	}
};

// Returns a1/b1 <= a2/b2 with no overflows
// Requires b1, b2 > 0 and that b1 * b2 doesn't overflow
bool divComp(ll a1, ll b1, ll a2, ll b2) {
	if (a1/b1 != a2/b2) return a1/b1 < a2/b2;
	else return (a1 % b1)*b2 <= (a2 % b2)*b1;
}

// Returns true if mi.eval(x) >= min(le.eval(x), ri.eval(x))
// Assumes le < mi <= ri and that mi isn't below le.
bool redundant(const Line& le, const Line& mi, const Line& ri) {
	if (mi.a == ri.a) return false;
	return divComp(le.b-mi.b, mi.a-le.a, mi.b-ri.b, ri.a-mi.a);
}

struct ConvHull {
	vector<Line> hull;

	void pushLine(Line li) {
		for (int n = hull.size(); n >= 2; --n) {
			if (redundant(hull[n-2], hull[n-1], li)) hull.pop_back();
			else break;
		}
		if ((! hull.empty()) && (li.a == hull.back().a)) {
			if (li.b < hull.back().b) swap(li, hull.back());
		} else hull.push_back(li);
	}
	ConvHull(){}
	ConvHull(vector<Line> lines) {
		sort(lines.begin(), lines.end());
		for (auto li : lines) pushLine(li);
	}
	Line getMinLine(ll x) const {
		int low = 0;
		int high = (int)hull.size() - 1;
		while(low != high) {
			int mid = (low + high) >> 1;
			if (hull[mid].eval(x) < hull[mid+1].eval(x)) high = mid;
			else low = mid + 1;
		}
		return hull[low];
	}
};

int main() {
	int n, q;
	cin >> n >> q;

	vector<Line> lines(n);
	for (int i = 0; i < n; ++i) cin >> lines[i].a >> lines[i].b;
	ConvHull hull(lines);

	for (int qi = 0; qi < q; ++qi) {
		ll x;
		cin >> x;
		Line res = hull.getMinLine(x);
		cout << res.eval(x) << '\n';
	}
}
