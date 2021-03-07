#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}
template<int K, int H, class T = string> // Number of hashes to use, max string length (at least 2). Probability of collision <= (H / 4e9)^K
class Hasher {
	private:
		const static uint P = (uint)4e9 + 7;
		const static array<array<uint, K>, H> mults;
		vector<array<uint, K>> hashes;
		static uint mSub(uint a, uint b) { return (a >= b ? a - b : a + P - b); }
	public:
		Hasher(const T& s) : hashes(s.size() + 1) {
			for (int j = 0; j < K; ++j) hashes[0][j] = 1;
			for (int i = 1; i <= s.size(); ++i) {
				for (int j = 0; j < K; ++j) hashes[i][j] = (hashes[i-1][j] * (ull)mults[1][j] + s[i-1]) % P;
			}
		}
		// Hash for substring s[a, b], 0 <= a <= b + 1 <= |s|
		array<uint, K+1> operator()(int a, int b) const {
			int len = b-a+1;
			array<uint, K+1> res;
			for (int j = 0; j < K; ++j) res[j] = mSub(hashes[b+1][j], hashes[a][j] * (ull)mults[len][j] % P);
			res[K] = len;
			return res;
		}
};
template<int K, int H, class T>
const array<array<uint, K>, H> Hasher<K, H, T>::mults = [](){
	array<array<uint, K>, H> res;
	for (int j = 0; j < K; ++j) res[0][j] = 1;
	for (int j = 0; j < K; ++j) res[1][j] = rand(1u, P - 1);
	for (int i = 2; i < H; ++i) {
		for (int j = 0; j < K; ++j) res[i][j] = res[i-1][j] * (ull)res[1][j] % Hasher<K, H, T>::P;
	}
	return res;
}();
using HS = Hasher<3, (int)2e6>; // Probability of collision ~ 1e-11, set length!

int main() {
	string str;
	cin >> str;
	HS h(str);
	cout << (h(0, 1) == h(2, 3)) << '\n';
}
