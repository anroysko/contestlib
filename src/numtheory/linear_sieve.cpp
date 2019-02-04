#include <iostream>
#include <vector>
using namespace std;

void sieve(int n, vector<int>& primes, vector<int>& div_ind) {
	primes.reserve(n+1);
	div_ind.resize(n+1);
	for (int i = 0; i <= n; ++i) div_ind[i] = -1;
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
}

int main() {
	const int N = 1e7;
	vector<int> primes;
	vector<int> div_ind;
	sieve(N, primes, div_ind);
	// Should print
	// 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 
	// -1 -1 0 1 0 2 0 3 0 1 0 4 0 5 0 1 0 6 0 7 
	for (int i = 0; i < 20; ++i) cout << primes[i] << ' '; cout << '\n';
	for (int i = 0; i < 20; ++i) cout << div_ind[i] << ' '; cout << '\n';
}
