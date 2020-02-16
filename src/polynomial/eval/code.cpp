
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
