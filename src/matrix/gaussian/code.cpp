// Gaussian elimination mod 2
template<int M>
class BitGaussian {
	private:
		vector<bitset<M>> vecs, masks; // vectors, what linear combinations they are of input vectors
		vector<int> inds; // index of vector corresponding to bit j in masks
	public:
		// Adds a vector in O(M + nM / 64) time. Returns false if v in SPAN, true otherwise.
		bool addVec(bitset<M> vec, int ind = -1) {
			bitset<M> mask;
			int i = 0;
			int n = vecs.size();
			for (int j = 0; i < n; ++i) {
				while(! vec[j] && !vecs[i][j]) ++j;
				if (vecs[i][j] && vec[j]) {
					vec ^= vecs[i];
					mask ^= masks[i];
				} else if (vec[j]) break;
			}

			if (vec.count() == 0) return false;
			else {
				mask[n] = 1;
				inds.push_back(ind);
				vecs.insert(vecs.begin() + i, vec);
				masks.insert(masks.begin() + i, mask);
				return true;
			}
		}

		// Minimises v ^ S (and writes it to v) where S is some subset of input vectors, returns that set
		// Most significant bit is at index 0. Runs in O(M + nM / 64) time
		vector<int> minXor(bitset<M>& vec) {
			bitset<M> rm;
			for (int i = 0, j = 0; i < vecs.size(); ++i) {
				while(! vecs[i][j]) ++j;
				if (vec[j]) {
					vec ^= vecs[i];
					rm ^= masks[i];
				}
			}
			vector<int> ans;
			for (int j = 0; j < inds.size(); ++j) {
				if (rm[j]) ans.push_back(inds[j]);
			}
			return ans;
		}
};
