// van Emde Boas tree. Maintains a set of integers in range [0, 2^B) and supports operations
//	findNext(i): returns minimum j >= i in set, or 2^B if none exist
// 	findPrev(i): returns maximum j <= i in set, or -1 if none exist
//	insert(i), erase(i): insert/erase i into the set
//	empty(): returns TRUE if the set is empty
//	clear(): empties the set
//	init(bts): inits the set, after the call i will be in the set if bts[i] = 1. bts should be a bitset, but can be a vector of 0/1
// All operations except empty, clear and init are O(log B) = O(log log 2^B) with good constants
template<int B, typename ENABLE = void>
class VEBTree {
	private:
		const static int K = B / 2, R = (B + 1) / 2, M = (1 << B);
		const static int S = 1 << K, MASK = (1 << R) - 1;
		array<VEBTree<R>, S> ch;
		VEBTree<K> act;
		int mi, ma;
	public:
		bool empty() const { return ma < mi; }
		
		int findNext(int i) const {
			if (i <= mi) return mi;
			if (i > ma) return M;
			
			int j = i >> R, x = i & MASK;
			int res = ch[j].findNext(x);
			if (res <= MASK) return (j << R) + res;
			
			j = act.findNext(j + 1);
			return (j >= S) ? ma : ((j << R) + ch[j].findNext(0));
		}
		int findPrev(int i) const {
			if (i >= ma) return ma;
			if (i < mi) return -1;
			
			int j = i >> R, x = i & MASK;
			int res = ch[j].findPrev(x);
			if (res >= 0) return (j << R) + res;
			
			j = act.findPrev(j - 1);
			return (j < 0) ? mi : ((j << R) + ch[j].findPrev(MASK));
		}
		void insert(int i) {
			if (i <= mi) {
				if (i == mi) return;
				swap(mi, i);
				if (i == M) ma = mi; // we were empty
				if (i >= ma) return; // we had mi == ma
			} else if (i >= ma) {
				if (i == ma) return;
				swap(ma, i);
				if (i <= mi) return; // we had mi == ma
			}
			int j = i >> R;
			if (ch[j].empty()) act.insert(j);
			ch[j].insert(i & MASK);
		}
		void erase(int i) {
			if (i <= mi) {
				if (i < mi) return;
				i = mi = findNext(mi + 1);
				if (i >= ma) {
					if (i > ma) ma = -1; // we had mi == ma
					return; // after erase we have mi == ma
				}
			} else if (i >= ma) {
				if (i > ma) return;
				i = ma = findPrev(ma - 1);
				if (i <= mi) return; // after erase we have mi == ma
			}
			int j = i >> R;
			ch[j].erase(i & MASK);
			if (ch[j].empty()) act.erase(j);
		}

		void clear() {
			mi = M, ma = -1;
			act.clear();
			for (int i = 0; i < S; ++i) ch[i].clear();
		}
		template<class T>
		void init(const T& bts, int shift = 0, int s0 = 0, int s1 = 0) {
			while(s0 + s1 < M && !bts[shift + s0]) ++s0;
			while(s0 + s1 < M && !bts[shift + M-1 - s1]) ++s1;
			if (s0 + s1 >= M) clear();
			else {
				act.clear();
				mi = s0, ma = M-1 - s1;
				++s0; ++s1;
				for (int j = 0; j < S; ++j) {
					ch[j].init(bts, shift + (j << R), max(0, s0 - (j << R)), max(0, s1 - ((S-1-j) << R)));
					if (! ch[j].empty()) act.insert(j);
				}
			}
		}
};
template<int B>
class VEBTree<B, enable_if_t<(B <= 6)>> {
	private:
		const static int M = (1 << B);
		ull act;
	public:
		bool empty() const { return !act; }
		void clear() { act = 0; }

		int findNext(int i) const {
			ull tmp = act >> i;
			return (tmp ? i + __builtin_ctzll(tmp) : M);
		}
		int findPrev(int i) const {
			ull tmp = act << (63 - i);
			return (tmp ? i - __builtin_clzll(tmp) : -1);
		}
		void insert(int i) { act |= 1ull << i; }
		void erase(int i) { act &= ~(1ull << i); }
		
		template<class T>
		void init(const T& bts, int shift = 0, int s0 = 0, int s1 = 0) {
			act = 0;
			for (int i = s0; i + s1 < M; ++i) act |= ((ull)bts[shift + i]) << i;
		}
};
