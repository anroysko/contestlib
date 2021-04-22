// Duval's algorithm for Lyndon factorization
// https://cp-algorithms.com/string/lyndon_factorization.html
vector<string> duval(const string& str) {
	vector<string> res;
	for (int i = 0; i < str.size()) {
		int k = i, j = i + 1;
		for (; j < str.size() && str[k] <= str[j]; ++j) {
			k = ((str[j] == str[k]) ? k+1 : i);
		}
		for (; i <= k; i += j-k) res.emplace_back(str.substr(i, j-k));
		i = j;
	}
	return res;
}
