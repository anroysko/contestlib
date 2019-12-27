
class BaseMatroid {
	private:
	public:
		BaseMatroid() { }

		// Sets the active set U. It must be independent!
		void setActive(const vector<int>& u) {

		}
		// Checks if U - x + y is independent, where U is the active set
		bool isInd(int x, int y) const {

		}
		// Returns size of the ground set E
		int size() const { return n; }
};

/*
// Linear matroid
class LinearMatroid {};
*/

/*
// Linear matroid in Z2
template<int N>
class BinaryMatroid {};
*/

/*
// Graphic Matroid
class GraphicMatroid {};
*/

/*
// TODO: other matroid types
*/

// Finds a maximal set in the intersection of the independent sets of the two input matroids
template<class M1, class M2>
pair<vector<int>, vector<int>> matroidIntersection(M1 m1, M2 m2) {
	int n = m1.size();
	vector<int> u;
	vector<bool> act(n+1, 0);
	act[0] = 1;
	while(true) {
		m1.setActive(u);
		m2.setActive(u);
		vector<int> pre(n+1, -1), que = {0};
		for (int j = 0; j < que.size() && pre[0] == -1; ++j) {
			int i = que[j];
			for (int t = 0; t <= n && pre[0] == -1; ++t) {
				if (act[i] == act[t] || pre[t] != -1) continue;
				if (act[i] && !m1.isInd(i-1, t-1)) continue;
				if (!act[i] && !m2.isInd(t-1, i-1)) continue;
				pre[t] = i;
				que.push_back(t);
			}
		}
		if (pre[0] == -1) return u;
		for (int i = pre[0]; i; i = pre[i]) act[i] ^= 1;

		u.clear();
		for (int i = 0; i < n; ++i) {
			if (act[i+1]) u.push_back(i);
		}
	}
}
