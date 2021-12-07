// For proof, see https://mzhang2021.github.io/cp-blog/berlekamp-massey/#proofs
template<ll P>
class BerlekampMassey {
	private:
		static ll mPow(ll a, ll b) { return ((b & 1) ? (a * mPow(a, b ^ 1) % P) : (b ? mPow(a*a % P, b >> 1) : 1)); }
		static void mDec(ll& a, ll b) { a = (a >= b ? a - b : a + P - b); }
		static void polyMulMod(vector<ll>& a, vector<ll>& b, const vector<ll>& c) {
			int n = (int)c.size() - 1;
			for (int i = 2*n-2; i >= 0; --i) {
				ll v = 0;
				for (int x = max(0, i+1-n); x < min(n, i + 1); ++x) v = (v + a[x] * b[i-x]) % P;
				a[i] = v;
			}
			for (int i = 2*n-2; i >= n; --i) {
				for (int j = n; j >= 0; --j) mDec(a[i-j], c[j] * a[i] % P);
			}
		}
		vector<ll> nxt, rec, old, seq;
		int t = 0, old_t = 0, old_i = -1;
		ll old_d = 1;
	public:
		// Given a sequence seq[0], ..., seq[n-1] \in [0, P), finds the minimum t and associated rec[0], ..., rec[t] \in [0, P) s.t.
		//	1. rec[0] = 1 (mod P)
		// 	2. \sum_{j = 0}^{t} rec[j] seq[i - j] = 0 (mod P) for every i \in [t, n)
		// Time complexity: O(nt)
		BerlekampMassey(const vector<ll>& s) : nxt(s.size() + 1, 0), rec(s.size() + 1, 0), old(s.size() + 1, 0), seq(s) {
			rec[0] = 1, old[0] = 1;
			for (int i = 0; i < seq.size(); ++i) {
				ll d = s[i];
				for (int j = 1; j <= t; ++j) d = (d + rec[j] * seq[i-j]) % P;
				if (d == 0) continue;

				ll mult = d * mPow(old_d, P-2) % P;
				for (int j = 0; j <= t; ++j) nxt[j] = rec[j];
				for (int j = 0; j <= old_t; ++j) mDec(nxt[j + i - old_i], old[j] * mult % P);
				
				if (2*t <= i) {
					old_i = i, old_d = d, old_t = t;
					t = i + 1 - t;
					swap(old, rec);
				}
				swap(rec, nxt);
			}
			rec.resize(t + 1, 0);
		}
		// Returns seq[k], assuming \sum_{j = 0}^{t} rec[j] seq[i - j] = 0 (mod P) holds for i >= n
		// Time complexity: O(t^2 log k)
		ll kthTerm(ll k) const {
			if (t == 1) return seq[0] * mPow((P - rec[1]) % P, k) % P;

			vector<ll> cur(2*(t+1), 0), mult(2*(t+1), 0);
			cur[0] = 1, mult[1] = 1;
			for (; k > 0; k >>= 1) {
				if (k & 1) polyMulMod(cur, mult, rec);
				polyMulMod(mult, mult, rec);
			}
			ll res = 0;
			for (int i = 0; i < t; ++i) res = (res + cur[i] * seq[i]) % P;
			return res;
		}
		vector<ll> getRec() const { return rec; }
};
