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
	public:
		Hasher(const T& s) : hashes(s.size() + 1) {
			for (int j = 0; j < K; ++j) hashes[0][j] = 1;
			for (int i = 1; i <= s.size(); ++i) {
				for (int j = 0; j < K; ++j) hashes[i][j] = (hashes[i-1][j] * (ull)mults[1][j] + s[i-1]) % P;
			}
		}
		// Hash for substring s[a, b], 0 <= a <= b + 1 <= |s|
		array<uint, K+1> operator()(int a, int b) const {
			array<uint, K+1> res;
			for (int j = 0; j < K; ++j) {
				res[j] = (hashes[b+1][j] - hashes[a][j] * (ull)mults[b-a+1][j]) % P;
				if (res[j] < 0) res[j] += P;
			}
			res[K] = b-a+1;
			return res;
		}
};
template<int K, int H, class T>
const array<array<uint, K>, H> Hasher<K, H, T>::mults = [](){
	array<array<uint, K>, H> res;
	for (int j = 0; j < K; ++j) res[0][j] = 1;
	for (int j = 0; j < K; ++j) res[1][j] = rand(1u, P - 1);
	for (int i = 2; i < H; ++i) {
		for (int j = 0; j < K; ++j) res[i][j] = res[i-1][j] * (ull)res[1][j] % P;
	}
	return res;
}();
using HS = Hasher<3, (int)1e6>; // Probability of collision ~ 1e-11, set length!
