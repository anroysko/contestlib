using ull = unsigned long long;
using uint = unsigned int;

// Requires C++14 or newer!
// Class for faster modular multiplication by non-constant prime modulo P < 2^30.
class Montgomery {
	private:
		static constexpr ull R = (ull)1 << 32;
		const uint P, RR, MC; // Modulo, 2^64 mod P, x for which xP = -1 mod 2^32

		static ull modPow(ull a, ull b, ull c) {
			return ((b & 1) ? (a * modPow(a, b ^ 1, c) % c) : (b ? modPow(a*a % c, b >> 1, c) : 1));
		}
	public:
		Montgomery(uint m) : P(m), RR((R%m)*R % m), MC((R * modPow(R % m, m-2, m) - 1) / m) {}
		uint transform(uint v) const { return mul(v, RR); } // standard uint -> mont representation
		uint reverse(uint v) const { return mul(v, 1) % P; } // mont representation -> standard uint
		uint add(uint a, uint b) const { return (a+b < 2*P ? a+b : a+b - 2*P); }
		uint sub(uint a, uint b) const { return (a < b) ? (2*P - b + a) : (a - b); }
		uint inv(uint v) const { return pow(v, P - 2); }
		uint mul(uint a, uint b) const {
			ull t = (ull)a * b; // < 4P
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
};

struct NTT {
	private:
		Montgomery mt;
		vector<uint> mults[2];

		// Forward direction
		void fntt(uint* arr, int h) {
			if (h <= 64) {
				for (int n = 2*h; h > 0; h >>= 1) {
					for (int s = 0; s < n; s += 2*h) {
						for (int i = s; i < s + h; ++i) {
							uint v0 = arr[i], v1 = arr[i + h];
							arr[i] = mt.add(v0, v1);
							arr[i + h] = mt.mul(mt.sub(v0, v1), mults[0][i + h - s]);
						}
					}
				}
			} else {
				for (int i = 0; i < h; ++i) {
					uint v0 = arr[i], v1 = arr[i + h];
					arr[i] = mt.add(v0, v1);
					arr[i + h] = mt.mul(mt.sub(v0, v1), mults[0][i + h]);
				}
				fntt(arr, h >> 1);
				fntt(arr + h, h >> 1);
			}
		}

		// Inverse direction
		void intt(uint* arr, int h) {
			if (h <= 64) {
				int n = 2*h;
				for (h = 1; h < n; h <<= 1) {
					for (int s = 0; s < n; s += 2*h) {
						for (int i = s; i < s + h; ++i) {
							uint v0 = arr[i], v1 = mt.mul(arr[i + h], mults[1][i + h - s]);
							arr[i] = mt.add(v0, v1);
							arr[i + h] = mt.sub(v0, v1);
						}
					}
				}
			} else {
				intt(arr, h >> 1);
				intt(arr + h, h >> 1);
				for (int i = 0; i < h; ++i) {
					uint v0 = arr[i], v1 = mt.mul(arr[i + h], mults[1][i + h]);
					arr[i] = mt.add(v0, v1);
					arr[i + h] = mt.sub(v0, v1);
				}
			}
		}
	public:
		// max_n: a value s.t. res.size() <= max_n in every polyMult call
		// P: must be a prime s.t. 3 is a generator and max_n | P - 1
		NTT(int max_n, uint P = 998244353) : mt(P) {
			int n = 1;
			while(n < max_n) n <<= 1;
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
		}
		vector<int> polyMult(const vector<int>& a, const vector<int>& b) {
			int n = 1, m = (int)a.size() + (int)b.size() - 1;
			while(n < m) n <<= 1;

			vector<uint> as(n, 0), bs(n, 0);
			for (int i = 0; i < a.size(); ++i) as[i] = mt.transform(a[i]);
			for (int i = 0; i < b.size(); ++i) bs[i] = mt.transform(b[i]);
			fntt(as.data(), n / 2); fntt(bs.data(), n / 2);

			uint mult = mt.inv(mt.transform(n));
			for (int i = 0; i < n; ++i) as[i] = mt.mul(mt.mul(as[i], bs[i]), mult);
			intt(as.data(), n / 2);

			vector<int> res(m);
			for (int i = 0; i < m; ++i) res[i] = mt.reverse(as[i]);
			return res;
		}
};
