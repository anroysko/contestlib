// Function for precalculating primes and prime divisors
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

// Struct for precalculation of multiplicative functions
struct Sieve {
	vector<int> primes;  // primes[i]    := i+1'th prime
	vector<int> div_ind; // div_ind[x]   := minimum i s.t. primes[i] | x
	vector<int> mobius;  // mobius[p^k]  := [k == 0] - [k == 1]
	vector<int> div_cou; // div_cou[p^k] := k+1
	vector<int> div_sum; // div_sum[p^k] := (p^{k+1}-1) / (p-1)
	vector<int> totient; // totient[p^k] := p^k - p^{k-1}

	Sieve(int n) : div_ind(n+1, -1), mobius(n+1, 1), div_cou(n+1, 1), div_sum(n+1, 1), totient(n+1, 1) {
		vector<int> base(n+1, 1);
		for (int i = 2; i <= n; ++i) {
			if (div_ind[i] == -1) {
				div_ind[i] = primes.size();
				primes.push_back(i);
			}
			for (int j = 0; j <= div_ind[i]; ++j) {
				int t = i * primes[j];
				if (t > n) break;
				div_ind[t] = j;
				base[t] = (j == div_ind[i] ? base[i] : i);
			}

			// Calculate multiplicative functions
			int bs = base[i];
			if (bs == 1) {
				int p = primes[div_ind[i]];
				int j = i/p;
				mobius[i]  = (i == p ? -1 : 0);
				div_cou[i] = div_cou[j] + 1;
				div_sum[i] = div_sum[j] + i;
				totient[i] = totient[j] * (p - (i == p));
			} else {
				int pk = i/bs;
				mobius[i]  = mobius[bs]  * mobius[pk];
				div_cou[i] = div_cou[bs] * div_cou[pk];
				div_sum[i] = div_sum[bs] * div_sum[pk];
				totient[i] = totient[bs] * totient[pk];
			}
		}
	}
};

