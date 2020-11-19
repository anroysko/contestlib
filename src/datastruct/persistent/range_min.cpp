template<class T>
class PersistentRangeMin {
	private:
		struct Node {
			int le, ri;
			T minv;
		};
		vector<Node> hist; 
		vector<int> roots;
		int n;

		void makeNode(T minv) {
			hist.emplace_back(-1, -1, minv);
			return (int)hist.size() - 1;
		}
		void makeNode(int le, int ri) {
			hist.emplace_back(le, ri, min(hist[le].minv, hist[ri].minv));
			return (int)hist.size() - 1;
		}
		int recBuild(int ia, int ib, const vector<T>& v) {
			if (ia + 1 == ib) return makeNode(v[ia]);

			int im = (ia + ib) >> 1;
			return makeNode(recBuild(ia, im, v), recBuild(im, ib, v));
		}
		int recSet(int ind, const T& v, int i, int ia, int ib) {
			if (ia + 1 == ib) return makeNode(v);

			int im = (ia + ib) >> 1;
			if (ind < im) {
				return makeNode(recSet(ind, v, hist[i].le, ia, im), hist[i].ri);
			} else {
				return makeNode(hist[i].le, recSet(ind, v, hist[i].ri, im, ia));
			}
		}
		T recGet(int a, int b, int i, int ia, int ib) const {
			if (a <= ia && b <= ib) return hist[i].minv;

			int im = (ia + ib) >> 1;
			if (a >= im) return recGet(a, b, hist[i].ri, im, ib);
			
			T r0 = recGet(a, b, hist[i].le, ia, im);
			if (b > im) {
				T r1 = recGet(a, b, hist[i].ri, im, ib);
				if (r1 < r0) return r1;
			}
			return r0;
		}
		int recSearch(int a, const T& v, int i, int ia, int ib) const {
			if (ia + 1 == ib) return (hist[i].minv <= v ? ia : n);

			int im = (ia + ib) >> 1;
			if (a < im && (hist[hist[i].le].minv <= v)) {
				int r = recSearch(a, v, hist[i].le, ia, im);
				if (r < n) return r;
			}
			return recSearch(a, v, hist[i].ri, im, ib);
		}
	public:	
		RangeMin(const vector<T>& v) : n(v.size()) {
			int r = recBuild(0, n, v);
			roots.push_back(r);
		}
		// Sets val[ind] = v to array after update t, and returns index of the update made
		int set(int ind, const T& v, int t) {
			int r = recSet(ind, v, roots[t], 0, n);
			roots.push_back(r);
			return (int)roots.size() - 1;
		}
		// Returns range minimum after update t. MUST HAVE a <= b
		T get(int a, int b, int t) const {
			return recGet(a, b + 1, roots[t], 0, n);
		}
		// Returns smallest ind >= a s.t. after update t, it held that val[ind] <= v, or n if there is none
		int search(int a, T v, int t) const {
			return recSearch(a, v, roots[t], 0, n);
		}
};
