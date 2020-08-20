int firstBit(ull x) { return __builtin_ctzll(x); }
int lastBit(ull x) { return 63 - __builtin_clzll(x); }

// O(n) preprocessing, O(1) RMQ data structure.
template<class T>
class RMQ {
	private:
		const int H = 6; // Block size is 2^H
		const int B = 1 << H;
		vector<T> vec; // Original values
		vector<ull> mins; // Min bits
		vector<int> tbl; // sparse table
		int n, m;

		// Get index with minimum value in range [a, a + len) for 0 <= len <= B
		int getShort(int a, int len) const {
			return a + lastBit(mins[a] & (-1ull >> (64 - len)));
		}
		int minInd(int ia, int ib) const {
			return vec[ia] < vec[ib] ? ia : ib;
		}
	public:
		RMQ(const vector<T>& vec_) : vec(vec_), mins(vec_.size()) {
			n = vec.size();
			m = (n + B-1) >> H;

			// Build sparse table
			int h = lastBit(m) + 1;
			tbl.resize(h*m);
			for (int j = 0; j < m; ++j) tbl[j] = j << H;
			for (int i = 0; i < n; ++i) tbl[i >> H] = minInd(tbl[i >> H], i);
			for (int j = 1; j < h; ++j) {
				for (int i = j*m; i < (j+1)*m; ++i) {
					int i2 = min(i + (1 << (j-1)), (j+1)*m - 1);
					tbl[i] = minInd(tbl[i-m], tbl[i2-m]);
				}
			}
			// Build min bits
			ull cur = 0;
			for (int i = n-1; i >= 0; --i) {
				for (cur <<= 1; cur > 0; cur ^= cur & -cur) {
					if (vec[i + firstBit(cur)] < vec[i]) break;
				}
				cur |= 1;
				mins[i] = cur;
			}
		}
		int argmin(int a, int b) const {
			++b; // to make the range inclusive
			int len = min(b-a, B);
			int ind1 = minInd(getShort(a, len), getShort(b-len, len));

			int ax = (a >> H) + 1;
			int bx = (b >> H);
			if (ax >= bx) return ind1;
			else {
				int h = lastBit(bx-ax);
				int ind2 = minInd(tbl[h*m + ax], tbl[h*m + bx - (1 << h)]);
				return minInd(ind1, ind2);
			}
		}
		int get(int a, int b) const { return vec[argmin(a, b)]; }
};
