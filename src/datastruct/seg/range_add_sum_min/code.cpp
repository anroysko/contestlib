// Segment tree for range addition, range sum and range minimum.
class SegTree {
	private:
		vector<ll> sum, minv, tag;
		int h = 1;

		// Returns length of interval corresponding to position i
		ll len(int i) { return h >> (31 - __builtin_clz(i)); }

		void apply(int i, ll v) {
			sum[i] += v * len(i);
			minv[i] += v;
			if (i < h) tag[i] += v;
		}
		void push(int i) {
			if (tag[i] == 0) return;
			apply(2*i, tag[i]);
			apply(2*i+1, tag[i]);
			tag[i] = 0;
		}

		ll recGetSum(int a, int b, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return 0;
			if (a <= ia && ib <= b) return sum[i];
			push(i);
			int im = (ia + ib) >> 1;
			return recGetSum(a, b, 2*i, ia, im) + recGetSum(a, b, 2*i+1, im, ib);
		}
		ll recGetMin(int a, int b, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return 4 * (ll)1e18;
			if (a <= ia && ib <= b) return minv[i];
			push(i);
			int im = (ia + ib) >> 1;
			return min(recGetMin(a, b, 2*i, ia, im), recGetMin(a, b, 2*i+1, im, ib));
		}
		void recApply(int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return;
			if (a <= ia && ib <= b) apply(i, v);
			else {
				push(i);
				int im = (ia + ib) >> 1;
				recApply(a, b, v, 2*i, ia, im);
				recApply(a, b, v, 2*i+1, im, ib);
				sum[i] = sum[2*i] + sum[2*i+1];
				minv[i] = min(minv[2*i], minv[2*i+1]);
			}
		}
	public:
		SegTree(int n) {
			while(h < n) h *= 2;
			sum.resize(2*h, 0);
			minv.resize(2*h, 0);
			tag.resize(h, 0);
		}
		ll rangeSum(int a, int b) { return recGetSum(a, b+1, 1, 0, h); }
		ll rangeMin(int a, int b) { return recGetMin(a, b+1, 1, 0, h); }
		void rangeAdd(int a, int b, ll v) { recApply(a, b+1, v, 1, 0, h); }
};
