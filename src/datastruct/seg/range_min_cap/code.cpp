// Segment tree for range minimum and range capping
class SegTree {
	private:
		const ll NEUT = 4 * (ll)1e18;
		vector<ll> seg, tag;
		int h = 1;

		void apply(int i, ll v) {
			seg[i] = min(seg[i], v);
			if (i < h) tag[i] = min(tag[i], v);
		}
		ll recGet(int a, int b, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return NEUT;
			if (a <= ia && ib <= b) return seg[i];
			int im = (ia + ib) >> 1;
			return min(tag[i], min(
				recGet(a, b, 2*i, ia, im),
				recGet(a, b, 2*i+1, im, ib)));
		}
		void recApply(int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return;
			if (a <= ia && ib <= b) apply(i, v);
			else {
				int im = (ia + ib) >> 1;
				recApply(a, b, v, 2*i, ia, im);
				recApply(a, b, v, 2*i+1, im, ib);
				seg[i] = min(tag[i], min(seg[2*i], seg[2*i+1]));
			}
		}
		int recSearch(int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia || seg[i] > v) return b;
			if (a <= ia && ib <= b && (ia == ib || tag[i] <= v)) return ia;
			int im = (ia + ib) >> 1;
			int tmp = recSearch(a, b, v, 2*i, ia, im);
			if (tmp < b) return tmp;
			else return recSearch(a, b, v, 2*i+1, im, ib);
		}
	public:
		SegTree(int n) {
			while(h < n) h *= 2;
			seg.resize(2*h, NEUT);
			tag.resize(h, NEUT);
		}
		ll rangeMin(int a, int b) { return recGet(a, b+1, 1, 0, h); }
		void rangeCap(int a, int b, ll v) { recApply(a, b+1, v, 1, 0, h); }
		int search(int a, int b, ll v) { return recSearch(a, b+1, v, 1, 0, h); }
};
