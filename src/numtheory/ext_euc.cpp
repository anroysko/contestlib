#include <iostream>
#include <utility>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

// Finds gcd(a, b), and returns c, d such that ac + bd = gcd(a, b) (MOD)
// Time complexity: log(a + b)
// Returns: {gcd(a, b), {c, d}}
pair<int, pair<int, int>> extEucMod(int a, int b) {
	if (b == 0) return {a, {1, 0}};
	int m = a / b;
	auto sub = extEucMod(b, a - b * m);

	int ac = sub.second.second;
	int bc = sub.second.first - ((ll)m * sub.second.second) % MOD;
	if (bc < 0) bc += MOD;
	return {sub.first, {ac, bc}};
}

// Finds gcd(a, b), and returns c, d such that ac + bd = gcd(a, b)
// Time complexity: log(a + b)
// Returns: {gcd(a, b), {c, d}}
pair<int, pair<int, int>> extEuc(int a, int b) {
	if (b == 0) return {a, {1, 0}};
	int m = a / b;
	auto sub = extEuc(b, a - b * m);

	int ac = sub.second.second;
	int bc = sub.second.first - (m * sub.second.second);
	return {sub.first, {ac, bc}};
}

int main() {
	int a = 131;
	int b = 91;
	int c = 147;
	int d = 111;
	auto sub1 = extEucMod(a, b);
	auto sub2 = extEucMod(b, c);
	auto sub3 = extEuc(c, d);
	auto sub4 = extEuc(d, a);
	// Should print:
	// 1 999999982 36
	// 7 999999999 5
	// 3 -3 4
	// 1 -59 50
	for (auto v : {sub1, sub2, sub3, sub4}) {
		cout << v.first << ' ' << v.second.first << ' ' << v.second.second << '\n';
	}
}
