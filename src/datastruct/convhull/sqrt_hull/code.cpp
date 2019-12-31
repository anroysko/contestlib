
struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
	bool operator<(const Line& rhs) const { return a < rhs.a; }
};
class ConvHull {
	private:
		vector<Line> hull;

		static bool redundant(const Line& x, const Line& y, const Line& z) {
			return (x.b - y.b)*(z.a - y.a) <= (y.b - z.b)*(y.a - x.a);
		}
	public:
		// lines must be added in order of nonincreasing a
		void addLine(Line li) {
			while(hull.size() >= 2 && hull.back().a <= li.a) {
				if (redundant(hull[hull.size() - 2], hull.back(), li)) hull.pop_back();
				else break;
			}
			if ((! hull.empty()) && (li.a == hull.back().a)) {
				if (li.b < hull.back().b) hull.back() = li;
			} else hull.push_back(li);
		}
		ll min(ll x) const {
			int low = 0;
			int high = (int)hull.size() - 1;
			while(low != high) {
				int mid = (low + high) >> 1;
				if (hull[mid].eval(x) < hull[mid+1].eval(x)) high = mid;
				else low = mid + 1;
			}
			return hull[low].eval(x);
		}
		void clear() { hull.clear(); }
};

// Stores an array of functions f_i(x) = a_i (x + y_i) + b_i
// Does operations in O(sqrt(n) log n) time
class SqrtHull {
	private:
		const ll INF = 4 * (ll)1e18;
		vector<Line> lines;
		vector<ConvHull> blocks;
		vector<ll> adds;
		int n, k;

		void build(int j) {
			int a = j*k;
			int b = min(n, a+k);
			vector<Line> ord(b-a);
			for (int i = a; i < b; ++i) ord[i-a] = lines[i];
			sort(ord.begin(), ord.end());

			blocks[j].clear();
			for (auto ln : ord) blocks[j].addLine(ln);
		}
	public:
		SqrtHull(const vector<Line>& ini) : n(ini.size()), lines(ini) {
			for (k = 1; k*k < n;) ++k;
			int m = (n+k-1) / k;
			blocks.resize(m);
			adds.resize(m, 0);
			for (int j = 0; j < m; ++j) build(j);
		}

		// Adds y_{i} += dy for a <= i <= b
		void rangeAdd(int a, int b, ll dy) {
			int aj = a / k + 1;
			int bj = b / k;
			for (int j = aj; j < bj; ++j) adds[j] += dy;
			for (; a < min(b+1, aj*k); ++a) lines[a].b += lines[a].a * dy;
			for (; b >= max(a, bj*k); --b) lines[b].b += lines[b].a * dy;
			build(aj-1);
			build(bj);
		}

		// Returns \min_{a \leq i \leq b} f_i(x)
		ll rangeMin(int a, int b, ll x) const {
			ll res = INF;
			int aj = a / k + 1;
			int bj = b / k;
			for (int j = aj; j < bj; ++j) res = min(res, blocks[j].min(x + adds[j]));
			for (; a < min(b+1, aj*k); ++a) res = min(res, lines[a].eval(x + adds[aj-1]));
			for (; b >= max(a, bj*k); --b) res = min(res, lines[b].eval(x + adds[bj]));
			return res;
		}
};
