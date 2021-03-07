
class UniformMatroid {
	private:
		int n, k;
	public:
		UniformMatroid(int n_, int k_) : n(n_), k(k_) {}

		int rank(const vector<int>& vec) const {
			return min(vec.size(), k);
		}
		bool independent(const vector<int>& vec) const {
			return vec.size() <= k;
		}
};
class PartitionMatroid {
	private:
		vector<int> caps, prt;
	public:
		PartitionMatroid(const vector<int>& caps_, const vector<int>& prt_) : caps(caps_), prt(prt_) {}

		int rank(const vector<int>& vec) {
			int res = 0;
			for (auto v : vec) {
				res += (caps[prt[v]] > 0);
				--caps[prt[v]];
			}
			for (auto v : vec) ++caps[prt[v]];
			return res;
		}
		bool independent(const vector<int>& vec) {
			return rank(vec) == vec.size();
		}
};
