#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

tuple<vector<int>, vector<int>> sieve(int n) {
	vector<int> primes;
	vector<int> div_ind(n+1, -1);
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

int main() {
	int n;
	cin >> n;

	vector<int> primes, div_ind;
	tie(primes, div_ind) = sieve(n);

	for (auto p : primes) cout << p << ' '; cout << '\n';
	for (int i = 0; i <= n; ++i) cout << div_ind[i] << ' '; cout << '\n';
}
