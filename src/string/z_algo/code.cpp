
// z[i] := max_{k} s.t. str[0, k) = str[i, i+k)}
vector<int> zAlgo(const string& str) {
	int n = str.size();
	vector<int> z(n, 0);
	int l = 0, r = 0; // l and r give interval such that r is maximized, l < i, and r = l + z[l].
	for (int i = 1; i < n; ++i) {
		z[i] = max(0, min(z[i-l], r-i));
		while((i+z[i] < n) && (str[z[i]] == str[i+z[i]])) ++z[i];
		if (i+z[i] > r) {
			l = i;
			r = i+z[i];
		}
	}
	return z;
}
