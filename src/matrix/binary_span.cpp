// Maintains the span of binary vectors
template<int M>
class BinarySpan {
	private:
		vector<pair<bitset<M>, bitset<M>>> vecs;
	public:
		// Returns {\min_{ind}, \argmin_{ind}} of \xor_{i : ind[i] = 1} vec[i] for vectors i added to the basis
		pair<bitset<M>, bitset<M>> reduce(bitset<M> vec) {
			bitset<M> ind;
			for (int i = 0; i < vecs.size(); ++i) {
				bitset<M> nxt = vec ^ vecs[i].first;
				if (nxt < vec) { vec = nxt; ind ^= inds[i].second; }
			}
			return {vec, ind};
		}
		// Tries to add "vec" to the basis. Returns -1 if it already is in the basis, otherwise the vector's index in the basis
		int add(bitset<M> vec) {
			auto pr = reduce(vec);
			int res = (pr.first.any() ? (int)vecs.size() : -1);
			if (res != -1) {
				pr.second[res] = 1;
				vecs.emplace_back(pr);
			}
			return res;
		}
		int rank() const { return vecs.size(); }
};
