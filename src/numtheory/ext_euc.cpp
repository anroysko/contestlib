#include <iostream>
#include <utility>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

// Finds gcd(a, b), and returns c, d such that ac + bd = gcd(a, b) (MOD)
// Time complexity: log(c + d)
// Returns: {gcd(a, b), {c, d}}
pair<int, pair<int, int>> extEuc(int a, int b) {
	if (b == 0) return {a, {1, 0}};
	int m = a / b;
	auto sub = extEuc(b, a - b * m);

	int ac = sub.second.second;
	int bc = sub.second.first - ((ll)m * sub.second.second) % MOD;
	if (bc < 0) bc += MOD;
	return {sub.first, {ac, bc}};
}

int main() {
	int a = 131;
	int b = 91;
	int c = 147;
	std::pair<int, std::pair<int, int>> sub;
	sub = extEuc(a, b); // Should be 1 999999982 36
	cout << sub.first << ' ' << sub.second.first << ' ' << sub.second.second << '\n';
	sub = extEuc(b, c); // Should be 7 999999999 5
	cout << sub.first << ' ' << sub.second.first << ' ' << sub.second.second << '\n';
}
