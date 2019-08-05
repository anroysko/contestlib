struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};
class UndoLiChao {
	private:
		const static ll INF = 4e18;
		vector<vector<Line>> tree; // Tree of lines
		vector<ll> xs; // x-coordinate of point i
		vector<int> lst; // last nodes modified by addLine
		int k = 1; // Log-depth of the tree

		int mapInd(int j) const {
			int z = __builtin_ctz(j);
			return ((1<<(k-z)) | (j>>z)) >> 1;
		}
		bool comp(const Line& a, int i, int j) const {
			return a.eval(xs[j]) < tree[i].back().eval(xs[j]);
		}
	public:
		UndoLiChao(const vector<ll>& points) : xs(points) {
			while(points.size() >> k) ++k;
			tree.resize(1 << k, {{0, INF}});
			xs.resize(1 << k, points.back());
		}
		void addLine(Line line) {
			for (int i = 1, ia = 0, ib = (1<<k)-2;;) {
				int mid = (ia + ib) >> 1;
				tree[i].push_back(tree[i].back());
				if (comp(line, i, mid)) swap(line, tree[i].back());
				if (comp(line, i, ia)) {
					i = 2*i;
					ib = mid - 1;
				} else if (comp(line, i, ib)) {
					i = 2*i+1;
					ia = mid + 1;
				} else {
					lst.push_back(i);
					break;
				}
			}
		}
		Line minLine(int j) const {
			Line res = {0, INF};
			for (int i = mapInd(j+1); i > 0; i /= 2) {
				if (! comp(res, i, j)) res = tree[i].back();
			}
			return res;
		}
		void undo() {
			for (int i = lst.back(); i > 0; i /= 2) {
				tree[i].pop_back();
			}
			lst.pop_back();
		}
};
