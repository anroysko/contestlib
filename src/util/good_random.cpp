#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

int main() {
	int a, b, n;
	cin >> a >> b >> n;
	vector<int> perm(n);
	for (int i = 0; i < n; ++i) perm[i] = i;

	// Taking random number in range. First is faster and fails to b < a-1.
	cout << rand<int>(a, b) << '\n'; // Segfaults if b < a
	cout << rand<int>() % (b-a+1) + a << '\n'; // Segfaults if b = a-1

	// random_shuffle uses rand(), and therefore doesn't work well on windows
	shuffle(perm.begin(), perm.end(), rng);
	for (auto v : perm) cout << v << ' '; cout << '\n';
}
