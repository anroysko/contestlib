
class DSU {
	private:
		vector<int> comp, siz;
	public:
		DSU(int n) : comp(n), siz(n, 1) {
			for (int i = 0; i < n; ++i) comp[i] = i;
		}
		int getc(int i) {
			if (comp[i] != i) comp[i] = getc(comp[i]);
			return comp[i];
		}
		bool join(int a, int b) {
			a = getc(a);
			b = getc(b);
			if (a == b) return false;

			if (siz[a] < siz[b]) swap(a, b);
			siz[a] += siz[b];
			comp[b] = a;
			return true;
		}
		int compSize(int i) { return siz[getc(i)]; }
};
