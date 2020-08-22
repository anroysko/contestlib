struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};
class LiChao {
	private:
		const static ll INF = 4e18;
		vector<Line> tree; // Tree of lines
		vector<ll> xs; // x-coordinate of point i
		int k = 1; // Log-depth of the tree

		int mapInd(int j) const {
			int z = __builtin_ctz(j);
			return ((1<<(k-z)) | (j>>z)) >> 1;
		}
		bool comp(const Line& a, int i, int j) const {
			return a.eval(xs[j]) < tree[i].eval(xs[j]);
		}
	public:
		LiChao(const vector<ll>& points) {
			while(points.size() >> k) ++k;
			tree.resize(1 << k, {0, INF});
			xs.resize(1 << k, points.back());
			for (int i = 0; i < points.size(); ++i) xs[mapInd(i+1)] = points[i];
		}
		void addLine(Line line) {
			for (int i = 1; i < tree.size();) {
				if (comp(line, i, i)) swap(line, tree[i]);
				if (line.a > tree[i].a) i = 2*i;
				else i = 2*i+1;
			}
		}
		ll minVal(int j) const {
			j = mapInd(j+1);
			ll res = INF;
			for (int i = j; i > 0; i /= 2) res = min(res, tree[i].eval(xs[j]));
			return res;
		}
};
