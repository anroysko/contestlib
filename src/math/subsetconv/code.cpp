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
// overfows if (\sum f)(\sum g) overflows
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
