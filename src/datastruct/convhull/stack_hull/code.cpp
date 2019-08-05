struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};
class ConvHull {
	private:
		vector<Line> hull;

		static bool redundant(const Line& x, const Line& y, const Line& z) {
			return (y.b - z.b)*(y.a - x.a) <= (x.b - y.b)*(z.a - y.a);
		}
	public:
		// Must have li.a >= pre.a for all existing lines pre
		void addLine(Line li) {
			while(hull.size() >= 2 && hull.back().a < li.a) {
				if (redundant(hull[hull.size() - 2], hull.back(), li)) hull.pop_back();
				else break;
			}
			if ((! hull.empty()) && (li.a == hull.back().a)) {
				if (li.b < hull.back().b) swap(li, hull.back());
			} else hull.push_back(li);
		}
		Line minLine(ll x) const {
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
