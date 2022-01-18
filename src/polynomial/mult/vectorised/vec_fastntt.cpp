#include <immintrin.h>

// Class for both scalar and vectorised modular operations by non-constant prime modulo P < 2^30.
struct Montgomery {
	private:
		const ull R = 1ull << 32;
		const uint MC, RR;
		__m256i p2_vec, p_vec, mc_vec, one_vec, rr_vec;
		
		static ull modPow(ull a, ull b, ull c) {
			return ((b & 1) ? (modPow(a, b ^ 1, c) * a % c) : (b ? modPow(a*a % c, b >> 1, c) : 1));
		}
		__m256i reduce(__m256i t) const {
			__m256i k = _mm256_mul_epu32(t, mc_vec);
			__m256i kp = _mm256_mul_epu32(k, p_vec);
			__m256i res = _mm256_add_epi64(t, kp);
			return _mm256_srli_epi64(res, 32); 
		}
		__m256i reduceHigh(__m256i t) const {
			__m256i k = _mm256_mul_epu32(t, mc_vec);
			__m256i kp = _mm256_mul_epu32(k, p_vec);
			return _mm256_add_epi64(t, kp);
		}
	public:
		const uint P;

		Montgomery(uint P) : P(P), RR((R % P) * R % P), MC((R * modPow(R % P, P - 2, P) - 1) / P) {
			p2_vec = _mm256_set1_epi32(2*P);
			p_vec = _mm256_set1_epi64x(P);
			mc_vec = _mm256_set1_epi64x(MC);
			one_vec = _mm256_set1_epi64x(1);
			rr_vec = _mm256_set1_epi64x(RR);
		}

		///////////////////////
		// scalar operations //
		///////////////////////

		uint transform(uint v) const { return mul(v, RR); } // standard uint -> mont representation
		uint reverse(uint v) const { return mul(v, 1) % P; } // mont representation -> standard uint
		uint add(uint a, uint b) const { return (a+b < 2*P ? a+b : a+b - 2*P); }
		uint sub(uint a, uint b) const { return (a < b) ? (2*P - b + a) : (a - b); }
		uint inv(uint v) const { return pow(v, P - 2); }
		uint mul(uint a, uint b) const {
			ull t = (ull)a * b; // < 4P^2
			ull k = (uint)t * MC; // < 2^32
			return (t + k * P) >> 32; // < 2P, Exact division!
		}
		uint pow(uint v, ull b) const {
			uint r = transform(1);
			for (; b; b >>= 1) {
				if (b & 1) r = mul(r, v);
				v = mul(v, v);
			}
			return r;
		}

		///////////////////////
		// Vector operations //
		///////////////////////
		
		__m256i transform(__m256i a) const { return mul(a, rr_vec); }
		__m256i reverse(__m256i a) const { return mul(a, one_vec); } // WARNING: result is in [0, 2P) not [0, P)
		__m256i add(__m256i a, __m256i b) const {
			__m256i res = _mm256_add_epi32(a, b);
			__m256i sub = _mm256_sub_epi32(res, p2_vec);
			__m256i mask = _mm256_srai_epi32(sub, 32);
			return _mm256_blendv_epi8(sub, res, mask);
		}
		__m256i sub(__m256i a, __m256i b) const {
			__m256i res = _mm256_sub_epi32(a, b);
			__m256i mask = _mm256_cmpgt_epi32(b, a);
			__m256i add = _mm256_and_si256(p2_vec, mask);
			return _mm256_add_epi32(res, add);
		}
		__m256i mul(__m256i a, __m256i b) const { return reduce(_mm256_mul_epu32(a, b)); }
		__m256i mulHigh(__m256i a, __m256i b) const { return reduceHigh(_mm256_mul_epu32(a, b)); }
};

struct NTT {
	private:
		const int TR = 64; // Point where change to iterative happens, MUST BE A POWER OF TWO >= 64!!
		Montgomery mt;
		vector<uint> mults[2], ol_mults[2], aux;

		void fwdButterfly(uint* pos0, uint* pos1, uint* mult) {
			__m256i v0 = _mm256_loadu_si256((__m256i*)pos0);
			__m256i v1 = _mm256_loadu_si256((__m256i*)pos1);
			__m256i mu_lo = _mm256_loadu_si256((__m256i *)mult);
			
			__m256i sub_lo = mt.sub(v0, v1);
			__m256i sub_hi = _mm256_shuffle_epi32(sub_lo, 0b10110001);
			__m256i mu_hi = _mm256_shuffle_epi32(mu_lo, 0b10110001);
			__m256i tf_lo = mt.mul(sub_lo, mu_lo);
			__m256i tf_hi = mt.mulHigh(sub_hi, mu_hi);
			
			__m256i res_add = mt.add(v0, v1);
			__m256i res_sub = _mm256_blend_epi32(tf_lo, tf_hi, 0b10101010);
			_mm256_storeu_si256((__m256i*)pos0, res_add);
			_mm256_storeu_si256((__m256i*)pos1, res_sub);
		}
		void invButterfly(uint* pos0, uint* pos1, uint* mult) {
			__m256i v0 = _mm256_loadu_si256((__m256i *) pos0);
			__m256i v1_lo = _mm256_loadu_si256((__m256i*) pos1);
			__m256i mu_lo = _mm256_loadu_si256((__m256i*) mult);
			
			__m256i v1_hi = _mm256_shuffle_epi32(v1_lo, 0b10110001);
			__m256i mu_hi = _mm256_shuffle_epi32(mu_lo, 0b10110001);
			__m256i rhs_lo = mt.mul(v1_lo, mu_lo);
			__m256i rhs_hi = mt.mulHigh(v1_hi, mu_hi);
			__m256i rhs = _mm256_blend_epi32(rhs_lo, rhs_hi, 0b10101010);

			__m256i res_add = mt.add(v0, rhs);
			__m256i res_sub = mt.sub(v0, rhs);

			_mm256_storeu_si256((__m256i*)pos0, res_add);
			_mm256_storeu_si256((__m256i*)pos1, res_sub);
		}

		// Forward direction
		void fntt(uint* arr, int h) {
			if (h <= TR) {
				int n = h/4;
				for (; h >= n; h >>= 1) {
					for (int s = 0; s < 8*n; s += 2*h) {
						for (int i = s; i < s + h; i += 8) fwdButterfly(&arr[i], &arr[i + h], &mults[0][i + h - s]);
					}
				}

				for (int j = 0; j < 8; ++j) {
					for (int i = 0; i < n; ++i) aux[8*i + j] = arr[i + n*j];
				}
				for (int i = 0; i < 8*n; ++i) arr[i] = aux[i];

				for (; h > 0; h >>= 1) {
					for (int s = 0; s < n; s += 2*h) {
						for (int i = s; i < s + h; ++i) fwdButterfly(&arr[8*i], &arr[8*(i + h)], &ol_mults[0][8*(i + h - s)]);
					}
				}
			} else {
				for (int i = 0; i < h; i += 8) fwdButterfly(&arr[i], &arr[i + h], &mults[0][i + h]);
				fntt(arr, h >> 1);
				fntt(arr + h, h >> 1);
			}
		}

		// Inverse direction
		void intt(uint* arr, int h) {
			if (h <= TR) {
				int n = h / 4;
				for (h = 1; h < n; h <<= 1) {
					for (int s = 0; s < n; s += 2*h) {
						for (int i = s; i < s + h; ++i) invButterfly(&arr[8*i], &arr[8*(i + h)], &ol_mults[1][8*(i + h - s)]);
					}
				}

				for (int j = 0; j < 8; ++j) {
					for (int i = 0; i < n; ++i) aux[i + n*j] = arr[8*i + j];
				}
				for (int i = 0; i < 8*n; ++i) arr[i] = aux[i];

				for (; h < 8*n; h <<= 1) {
					for (int s = 0; s < 8*n; s += 2*h) {
						for (int i = s; i < s + h; i += 8) invButterfly(&arr[i], &arr[i + h], &mults[1][i + h - s]);
					}
				}
			} else {
				intt(arr, h >> 1);
				intt(arr + h, h >> 1);
				for (int i = 0; i < h; i += 8) invButterfly(&arr[i], &arr[i + h], &mults[1][i + h]);
			}
		}
	public:
		// n: value s.t. res.size() <= n in every polyMult call
		// P: must be a prime s.t. 3 is a generator and max_n | P - 1
		NTT(int n, uint P = 998244353) : mt(P), aux(2*TR) {
			n = max(n, TR);
			while(n & (n-1)) n += n ^ (n & (n - 1));
			mults[0].resize(n); mults[1].resize(n);

			array<uint, 2> roots;
			roots[1] = mt.pow(mt.transform(3), (P - 1) / n);
			roots[0] = mt.pow(roots[1], n - 1);
			
			for (int t = (n >> 1); t > 0; t >>= 1) {
				for (int j = 0; j <= 1; ++j) {
					uint cur = mt.transform(1);
					for (int i = 0; i < t; ++i, cur = mt.mul(cur, roots[j])) mults[j][t + i] = cur;
					roots[j] = mt.mul(roots[j], roots[j]);
				}
			}
			ol_mults[0].resize(2*TR); ol_mults[1].resize(2*TR);
			for (int i = 0; i < 2*TR; ++i) {
				for (int j = 0; j <= 1; ++j) ol_mults[j][i] = mults[j][i >> 3];
			}
		}
		vector<int> polyMul(const vector<int>& a, const vector<int>& b) {
			int n = 1, m = (int)a.size() + (int)b.size() - 1;
			vector<int> res(m, 0);
			while(n < m) n <<= 1;
			if (n < TR) {
				for (int x = 0; x < a.size(); ++x) {
					for (int y = 0; y < b.size(); ++y) res[x + y] = (res[x + y] + (ull)a[x] * b[y]) % mt.P;
				}
			} else {
				vector<uint> as(n, 0), bs(n, 0);
				for (int i = 0; i < a.size(); ++i) as[i] = mt.transform(a[i]);
				for (int i = 0; i < b.size(); ++i) bs[i] = mt.transform(b[i]);
				fntt(as.data(), n / 2); fntt(bs.data(), n / 2);
				uint mult = mt.inv(mt.transform(n));
				for (int i = 0; i < n; ++i) as[i] = mt.mul(mt.mul(as[i], bs[i]), mult);
				intt(as.data(), n / 2);
				for (int i = 0; i < m; ++i) res[i] = mt.reverse(as[i]);
			}
			return res;
		}
};
