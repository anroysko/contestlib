
void mobius(vector<ll>& f, bool rev = false) {
	for (int b = 1; b < f.size(); b *= 2) {
		for (int m = 0; m < f.size(); ++m) {
			if (m & b) f[m] += f[m^b] * (rev ? -1 : 1);
		}
	}
}
vector<vector<ll>> tieredMobius(const vector<ll>& f, int n) {
	vector<vector<ll>> res(n+1, vector<ll>(1<<n, 0));
	for (int m = 0; m < (1<<n); ++m) res[__builtin_popcount(m)][m] = f[m];
	for (int k = 0; k <= n; ++k) mobius(res[k]);
	return res;
}
// res[m]: \sum_{s \subset m} f[s] g[m^s]
// overfows if (\sum f)(\sum g) overflows.
// Time complexity: O(n^2 2^n), space: O(n 2^n)
vector<ll> subsetConv(const vector<ll>& f, const vector<ll>& g, int n) {
	auto fc = tieredMobius(f, n);
	auto gc = tieredMobius(g, n);
	vector<ll> res((1<<n));
	for (int s = 0; s <= n; ++s) {
		vector<ll> tmp((1<<n), 0);
		for (int j = 0; j <= s; ++j) {
			for (int m = 0; m < (1<<n); ++m) {
				tmp[m] += fc[j][m] * gc[s-j][m];
			}
		}
		mobius(tmp, true);
		for (int m = 0; m < (1<<n); ++m) {
			if (__builtin_popcount(m) == s) res[m] = tmp[m];
		}
	}
	return res;
}

template<int N>
class BinaryConv {
	private:
		static constexpr int M = 1 << N;
		array<bitset<M>, N> has_bit; // no_bit[i][m] iff m has bit i set
		
		void mob(bitset<M>& f) {
			for (int b = 0; b < N; ++b) {
				f ^= has_bit[b] & (f << (1 << b));
			}
		}
		array<bitset<M>, N+1> tMob(const bitset<M>& f) {
			array<bitset<M>, N+1> res;
			for (int m = 0; m < M; ++m) res[__builtin_popcount(m)][m] = f[m];
			for (int n = 0; n <= N; ++n) mob(res[n]);
			return res;
		}
	public:
		BinaryConv() {
			for (int m = 0; m < M; ++m) {
				for (int j = 0; j < N; ++j) has_bit[j][m] = (m & (1<<j));
			}
		}
		// res[m]: \xor_{s \subset m} f[s] & g[m^s]
		// Time complexity: O(n 2^n), space O(2^n)
		bitset<M> apply(const bitset<M>& f, const bitset<M>& g) {
			auto fc = tMob(f), gc = tMob(g);
			array<bitset<M>, N+1> tmp;
			for (int s = 0; s <= N; ++s) {
				for (int j = 0; j <= s; ++j) tmp[s] ^= fc[j] & gc[s-j];
				mob(tmp[s]);
			}
			bitset<M> res;
			for (int m = 0; m < M; ++m) res[m] = tmp[__builtin_popcount(m)][m];
			return res;
		}
};

