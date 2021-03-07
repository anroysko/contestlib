// Segment tree for range addition and range sum. It is also easy to modify:
// just change NEUT, apply, and combine to what you need, and rename rangeSum, rangeAdd
class SegTree {
	private:
		const ll NEUT = 0; 
		vector<ll> seg, tag;
		int h = 1;

		// Returns length of interval corresponding to position i
		ll len(int i) { return h >> (31 - __builtin_clz(i)); }

		void apply(int i, ll v) {
			seg[i] += v * len(i);
			if (i < h) tag[i] += v;
		}
		void push(int i) {
			if (tag[i] == 0) return;
			apply(2*i, tag[i]);
			apply(2*i+1, tag[i]);
			tag[i] = 0;
		}
		ll combine(ll a, ll b) { return a + b; }

		ll recGet(int a, int b, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return NEUT;
			if (a <= ia && ib <= b) return seg[i];
			push(i);
			int im = (ia + ib) >> 1;
			return combine(
				recGet(a, b, 2*i, ia, im),
				recGet(a, b, 2*i+1, im, ib));
		}
		void recApply(int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return;
			if (a <= ia && ib <= b) apply(i, v);
			else {
				push(i);
				int im = (ia + ib) >> 1;
				recApply(a, b, v, 2*i, ia, im);
				recApply(a, b, v, 2*i+1, im, ib);
				seg[i] = combine(seg[2*i], seg[2*i+1]);
			}
		}
	public:
		SegTree(int n) {
			while(h < n) h *= 2;
			seg.resize(2*h, NEUT);
			tag.resize(h, 0);
		}
		ll rangeSum(int a, int b) { return recGet(a, b+1, 1, 0, h); }
		void rangeAdd(int a, int b, ll v) { recApply(a, b+1, v, 1, 0, h); }
};
