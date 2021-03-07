
// returns number of elements strictly smaller than v in vec
template<class T>
int bins(const vector<T>& vec, T v) {
	int low = 0;
	int high = vec.size();
	while(low != high) {
		int mid = (low + high) >> 1;
		if (vec[mid] < v) low = mid + 1;
		else high = mid;
	}
	return low;
}

template<class T>
pair<vector<int>, vector<T>> compress(const vector<T>& vec) {
	vector<T> comp = vec;
	sort(comp.begin(), comp.end());
	comp.erase(unique(comp.begin(), comp.end()), comp.end());

	vector<int> res(comp.size());
	for (int i = 0; i < comp.size(); ++i) res[i] = bins(comp, vec[i]);
	return {res, comp};
}

int mAdd(int a, int b, int m) { return (a+b >= m ? a+b-m : a+b); }
int mSub(int a, int b, int m) { return (a-b < 0 ? a-b+m : a-b); }

// Cyclic suffix array construction in O(n log n)
// sa[i]  := Position where i'th smallest suffix starts in the string
// rev[i] := Index of suffix starting at position i in suffix array
template<class T> // Either vector or string
pair<vector<int>, vector<int>> suffixArray(const T& str_) {
	vector<int> str = compress(str_).first;
	int n = str.size();

	vector<int> sa(n), rev(n), nsa(n), nrev(n), cou(n+1, 0);
	for (int i = 0; i < n; ++i) sa[i] = i;
	for (int i = 0; i < n; ++i) rev[i] = str[i];

	for (int s = 0; s < n; s = (s ? 2*s : s+1)) {
		for (auto& c : cou) c = 0;
		for (auto v : rev) ++cou[v + 1];
		for (int j = 1; j <= n; ++j) cou[j] += cou[j-1];

		for (int i = 0; i < n; ++i) nsa[i] = mSub(sa[i], s, n);
		for (auto i : nsa) {
			sa[cou[rev[i]]] = i;
			++cou[rev[i]];
		}

		for (int r = 0, j = 0; j < n; ++j) {
			if (j) r += (rev[sa[j]] != rev[sa[j-1]]) || (rev[mAdd(sa[j], s, n)] != rev[mAdd(sa[j-1], s, n)]);
			nrev[sa[j]] = r;
		}
		swap(rev, nrev);
	}
	return {sa, rev};
}
