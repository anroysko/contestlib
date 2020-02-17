
// Evaluates a degree n polynomial at n points in O(n log^2 n)
// Follows from the identity P(x) mod (x - x0) = P(x0) and O(n log n) polynomial division
// degree 10^5 polynomial and 10^5 points in ~4s
template<int P>
vector<int> multiPointEval(const vector<int>& pol, const vector<int>& xs) {
	int n = xs.size();
	vector<vector<int>> tree(2*n);
	for (int i = n; i < 2*n; ++i) tree[i] = {(int)mSub(0, xs[i-n], P), 1};
	for (int i = n-1; i > 0; --i) tree[i] = polyMult<P>(tree[2*i], tree[2*i+1]);

	vector<vector<int>> rem(2*n);
	rem[1] = polyMod<P>(pol, tree[1]);
	for (int i = 2; i < 2*n; ++i) {
		rem[i] = polyMod<P>(rem[i >> 1], tree[i]);
	}

	vector<int> res(n);
	for (int i = n; i < 2*n; ++i) res[i-n] = rem[i][0];
	return res;
}

// Calculate z^{i^2} for i = 0 ... m-1
template<int P>
vector<int> calcSquares(ll z, int n) {
	vector<int> res(n, 1);
	ll sq = (ll)z * z % P;
	for (int i = 1; i < n; ++i) {
		res[i] = z * res[i-1] % P;
		z = (z * sq) % P; // m^2 = 1 + 3 + ... + (2m + 1)
	}
	return res;
}

// Evaluates P(a), P(ab), ..., P(ab^(k-1)) in O((n+k) log (n+k))
template<int P>
vector<int> chirpzEval(vector<int> pol, int a, int b, int k) {
	int n = pol.size();
	int r = (k-1) / 2;
	auto sq = calcSquares<P>(b, max(r+1, n)); // sq[i] = b^{i^2}
	auto inv = calcSquares<P>(modPow(b, P-2, P), max(r+1, n)); // b^{-i^2}

	vector<int> v(n), u(n+r);
	for (int i = 0; i < n; ++i) v[i] = (ll)pol[i] * sq[i] % P;
	for (int i = 0; i < n+r; ++i) u[i] = inv[abs(i-(n-1))];

	vector<int> res(k);
	for (int p = 0; p <= 1; ++p) {
		ll cur = 1;
		for (int i = 0; i < n; ++i) {
			v[i] = cur * v[i] % P; // Multiply in a^i or b^i
			cur = cur * (p ? b : a) % P;
		}
		// Calculate P(ab^p), P(ab^{2+p}), ..., P(ab^{2r + p})
		// P(ab^{2j + p}) = \sum_i m_i a^i b^{(2j+p)i} = b^{j^2} \sum_i (a_i a^i b^{ip} b^{i^2}) (b^{-(j-i)^{2}})
		// since b^{(2j+p)i} = b^{j^2} b^{ip + i^{2}} b^{-(j-i)^{2}}
		auto prod = polyMult<P>(v, u);
		for (int j = 0; 2*j+p < k; ++j) res[2*j+p] = (ll)sq[j] * prod[n-1+j] % P;
	}
	return res;
}
