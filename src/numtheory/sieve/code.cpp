pair<vector<int>, vector<int>> sieve(int n) {
	vector<int> div_ind(n+1, -1), primes;
	for (int i = 2; i <= n; ++i) {
		if (div_ind[i] == -1) {
			div_ind[i] = primes.size();
			primes.push_back(i);
		}
		for (int j = 0; j <= div_ind[i]; ++j) {
			int t = primes[j] * i;
			if (t > n) break;
			div_ind[t] = j;
		}
	}
	return {primes, div_ind};
}
