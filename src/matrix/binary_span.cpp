// Returns set bits in a bitset
template<class T>
vector<int> getBits(const T& bts) {
	vector<int> res;
	for (int j = bts._Find_first(); j < bts.size(); j = bts._Find_next(j)) res.push_back(j);
	return res;
}

// Maintains the span of binary vectors
template<int M>
class BinarySpan {
	private:
		vector<pair<bitset<M>, bitset<M>>> vecs;
		
		pair<int, bitset<M>> tryAdd(bitset<M> bts, bool insert) {
			bitset<M> ind;
			for (int i = 0, j = 0; j < M; ++j) {
				if (i < vecs.size() && vecs[i].first[j]) {
					if (bts[j]) {
						bts ^= vecs[i].first;
						ind ^= vecs[i].second;
					}
					++i;
				} else if (bts[j]) {
					if (insert) {
						ind[(int)vecs.size()] = 1;
						vecs.insert(vecs.begin() + i, {bts, ind});
					}
					return {(int)vecs.size() - insert, ind};
				}
			}
			return {-1, ind};
		}
	public:
		int rank() const { return vecs.size(); }
		int addVec(bitset<M> bts) { return tryAdd(bts, 1).first; }
		pair<bool, vector<int>> inSpan(bitset<M> bts) { auto tmp = tryAdd(bts, 0); return {tmp.first == -1, getBits(tmp.second)}; }
};
