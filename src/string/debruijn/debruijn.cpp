// Generates the LEXICOGRAPHICALLY SMALLEST string of length k^n,
// that contains every string in \Sigma_k^n as a (cyclic) substring
// see pg 219 of http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.93.5967&rep=rep1&type=pdf
vector<int> debruijn(int n, int k) {
	if (k == 1) return vector<int>(n, 0);

	vector<int> a(n+1, 0), p(n+2, 1), res;
	for (int t = 1;; ++t) {
		int cp = p[t];
		for (; t <= n; ++t) {
			p[t] = cp;
			a[t] = a[t - cp];
		}
		if (! (n % cp)) {
			for (int j = 1; j <= cp; ++j) res.push_back(a[j]);
		}
		for (--t; a[t] == k-1;) --t;
		if (t == 0) return res;

		++a[t];
		p[t+1] = t;
	}
}
