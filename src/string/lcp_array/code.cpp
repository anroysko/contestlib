
// Builds a LCP array from the given suffix array
// lcp[i] := length of longest common prefix between the string starting from sa[i] and sa[i+1]
template<class T> // Either vector or string
vector<int> lcpArray(const T& str, const vector<int>& sa, const vector<int>& rev) {
	int n = str.size();
	vector<int> lcp(n-1, 0);
	for (int i = 0, cur = 0; i < n; ++i, cur = max(0, cur-1)) {
		if (rev[i] == n-1) continue;
		int j = sa[rev[i]+1];
		while (str[i+cur] == str[j+cur]) ++cur;
		lcp[rev[i]] = cur;
	}
	return lcp;
}
