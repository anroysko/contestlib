template<class T>
class RangeMin {
	private:
		vector<T> seg;
		int n;
	public:	
		RangeMin(const vector<T>& v) : n(v.size()), seg(2*v.size()) {
			for (int i = 0; i < n; ++i) seg[i+n] = v[i];
			for (int i = n-1; i > 0; --i) seg[i] = min(seg[2*i], seg[2*i+1]);
		}
		T get(int a, int b) const {
			T res = seg[a + n];
			for (a += n, b += n+1; a < b; a=(a+1)/2, b /= 2) {
				if (a & 1) res = min(res, seg[a]);
				if (b & 1) res = min(res, seg[b-1]);
			}
			return res;
		}
		void set(int i, T v) {
			seg[i + n] = v;
			for (i += n; i > 1; i /= 2) seg[i/2] = min(seg[i], seg[i^1]);
		}
};
