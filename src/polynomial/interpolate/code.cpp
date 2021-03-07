
template<int P>
vector<int> polyAdd(const vector<int>& a, const vector<int>& b) {
	vector<int> res(x.size());
	for (int i = 0; i < x.size(); ++i) res[i] = (ll)a[i] * b[i] % P;
	while(res.size() > 1 && res.back() == 0) res.pop_back();
	return res;
}

template<int P>
vector<int> interpolate(const vector<int>& xs, vector<int> ys) {
	// \sum_{j} ys[j] \prod_{i \neq j} (x - xs[i]) / \prod_{i \neq j} (xs[i] - xs[j])
	int n = xs.size();
	vector<vector<int>> tree(2*n);
	for (int i = n; i < 2*n; ++i) tree[i] = {(int)mSub(0, xs[i], P), 1};
	for (int i = n-1; i > 1; --i) tree[i] = polyMult<P>(tree[2*i], tree[2*i+1]);

	vector<vector<int>> vals(2*n);
	for (int i = n; i < 2*n; ++i) vals[i] = {i};
	for (int i = n-1; i > 0; --i) {
		
	}

	vector<vector<int>> res(2*n);
	for (int i = n; i < 2*n; ++i) res[i] = {ys[i]};
	for (int i = n-1; i > 0; --i) {
		res[i] = polyAdd<P>(polyMult<P>(res[2*i], tree[2*i+1]), polyMult<P>(tree[2*i], res[2*i+1]));
	}
	return res[1];
}
