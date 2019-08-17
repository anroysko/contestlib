vector<ll> mobius(int n, const vector<ll>& f, ll r = 1) {
	vector<ll> res = f;
	for (int b = 1; b < (1<<n); b *= 2) {
		for (int m = 0; m < (1<<n); ++m) {
			if (m & b) res[m] += res[m ^ b] * r;
		}
	}
	return res; // res[m] = \sum_{s \subset m} f[s] r^|m^s|
}
vector<vector<ll>> tieredMobius(int n, const vector<ll>& f) {
	vector<vector<ll>> res(n+1, vector<ll>(1<<n, 0));
	for (int m = 0; m < (1<<n); ++m) res[__builtin_popcount(m)][m] = f[m];
	for (int k = 0; k <= n; ++k) res[k] = mobius(n, res[k]);
	return res; // res[k][m]: \sum_{s \subset m, |s| = k} f[s]
}
vector<ll> subsetConv(int n, const vector<ll>& f, const vector<ll>& g) {
	auto fc = tieredMobius(n, f);
	auto gc = tieredMobius(n, g);
	vector<ll> res((1<<n));
	for (int s = 0; s <= n; ++s) {
		vector<ll> tmp((1<<n), 0);
		for (int j = 0; j <= s; ++j) {
			for (int m = 0; m < (1<<n); ++m) {
				tmp[m] += fc[j][m] * gc[s-j][m];
			}
		}
		tmp = mobius(n, tmp, -1);
		for (int m = 0; m < (1<<n); ++m) {
			if (__builtin_popcount(m) == s) res[m] = tmp[m];
		}
	}
	return res; // res[m]: \sum_{s \subset m} f[s] g[m^s]
}
