// Segment tree for range minimum, count of minimum and range addition.
class SegTree {
	private:
		const ll INF = 1e18; 
		vector<pair<ll, ll>> seg;
		vector<ll> tag;
		int h = 1;

		static pair<ll, ll> combine(ll x, pair<ll, ll> a, pair<ll, ll> b) {
			if (a.first == b.first) return {a.first + x, a.second + b.second};
			else if (a.first < b.first) return {a.first + x, a.second};
			else return {b.first + x, b.second};
		}
		void apply(int i, ll v) {
			seg[i].first += v;
			if (i < h) tag[i] += v;
		}
		pair<ll, ll> recGet(int a, int b, int i, int ia, int ib) const {
			if (ib <= a || b <= ia) return {INF, 0};
			if (a <= ia && ib <= b) return seg[i];
			int im = (ia + ib) >> 1;
			return combine(tag[i],
				recGet(a, b, 2*i, ia, im),
				recGet(a, b, 2*i+1, im, ib));
		}
		void recApply(int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return;
			if (a <= ia && ib <= b) apply(i, v);
			else {
				int im = (ia + ib) >> 1;
				recApply(a, b, v, 2*i, ia, im);
				recApply(a, b, v, 2*i+1, im, ib);
				seg[i] = combine(tag[i], seg[2*i], seg[2*i+1]);
			}
		}
	public:
		SegTree(const vector<ll>& vals, const vector<ll>& weights) {
			while(h < vals.size()) h *= 2;
			seg.resize(2*h, {0, 0});
			tag.resize(h, 0);
			for (int i = 0; i < vals.size(); ++i) seg[i+h] = {vals[i], weights[i]};
			for (int i = h-1; i > 0; --i) seg[i] = combine(0, seg[2*i], seg[2*i+1]);
		}
		pair<ll, ll> get(int a, int b) const { return recGet(a, b+1, 1, 0, h); }
		void add(int a, int b, ll v) { recApply(a, b+1, v, 1, 0, h); }
};
