pair<vector<int>, vector<int>> suffixArray(const string& str, int mx = 256) {
	int n = str.size();
	vector<int> sa(n), rev(n), nsa(n), nrev(n), cou(n+mx+1);
	for (int i = 0; i < n; ++i) sa[i] = i;
	for (int i = 0; i < n; ++i) rev[i] = str[i];
	for (int s = 0; s < n; s = (s?2*s:s+1), rev = nrev) {
		for (int i = 0; i < n; ++i) nsa[i] = (sa[i]-s + n*(sa[i] < s));
		for (auto& c : cou) c = 0;
		for (auto  v : rev) ++cou[v + 1];
		for (int j = 1; j <= n+mx; ++j) cou[j] += cou[j-1];
		for (auto i : nsa) {
			sa[cou[rev[i]]] = i;
			++cou[rev[i]];
		}
		for (int r = 0, j = 0; j < n; ++j) {
			r += (j && (rev[sa[j]] != rev[sa[j-1]]
				|| (rev[(sa[j]+s)%n] != rev[(sa[j-1]+s)%n])));
			nrev[sa[j]] = r;
		}
	}
	return {sa, rev};
}
