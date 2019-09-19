// Segment tree for range addition, setting, sum, minimum and maximum
class SegTree {
	private:
		const array<ll, 3> neut = {0, 4*(ll)1e18, -4*(ll)1e18};
		array<vector<ll>, 3> seg;
		vector<ll> tag, tag_t;
		int h = 1;

		void apply(int t, int i, ll v) {
			if (t == 0) {
				seg[0][i] += v * len(i);
				seg[1][i] += v;
				seg[2][i] += v;
				if (i < h) {
					if (tag_t[i] == -1) tag_t[i] = 0;
					tag[i] += v;
				}
			} else if (t == 1) {
				seg[0][i] = v * len(i);
				seg[1][i] = v;
				seg[2][i] = v;
				if (i < h) {
					tag_t[i] = 1;
					tag[i] = v;
				}
			}
		}
		ll combine(int t, ll a, ll b) {
			if (t == 0) return a + b;
			else if (t == 1) return min(a, b);
			else return max(a, b);
		}

		// #################
		// # DO NOT CHANGE #
		// #################

		// Returns length of interval corresponding to position i
		ll len(int i) { return h >> (31 - __builtin_clz(i)); }

		void push(int i) {
			int t = tag_t[i];
			if (t == -1) return;

			apply(t, 2*i, tag[i]);
			apply(t, 2*i+1, tag[i]);

			tag_t[i] = -1;
			tag[i] = 0;
		}
		void update(int i) {
			for (int t = 0; t < seg.size(); ++t) {
				seg[t][i] = combine(t, seg[t][2*i], seg[t][2*i+1]);
			}
		}

		ll recGet(int t, int a, int b, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return neut[t];
			if (a <= ia && ib <= b) return seg[t][i];
			push(i);
			int im = (ia + ib) >> 1;
			return combine(t,
				recGet(t, a, b, 2*i, ia, im),
				recGet(t, a, b, 2*i+1, im, ib));
		}
		void recApply(int t, int a, int b, ll v, int i, int ia, int ib) {
			if (ib <= a || b <= ia) return;
			if (a <= ia && ib <= b) apply(t, i, v);
			else {
				push(i);
				int im = (ia + ib) >> 1;
				recApply(t, a, b, v, 2*i, ia, im);
				recApply(t, a, b, v, 2*i+1, im, ib);
				update(i);
			}
		}
	public:
		SegTree(int n) {
			while(h < n) h *= 2;
			for (int j = 0; j < seg.size(); ++j) {
				seg[j].resize(2*h, 0);
			}
			tag.resize(h, 0);
			tag_t.resize(h, -1);
		}
		ll rangeSum(int a, int b) { return recGet(0, a, b+1, 1, 0, h); }
		ll rangeMin(int a, int b) { return recGet(1, a, b+1, 1, 0, h); }
		ll rangeMax(int a, int b) { return recGet(2, a, b+1, 1, 0, h); }
		void rangeAdd(int a, int b, ll v) { recApply(0, a, b+1, v, 1, 0, h); }
		void rangeSet(int a, int b, ll v) { recApply(1, a, b+1, v, 1, 0, h); }
};
