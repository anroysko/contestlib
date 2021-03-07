#include <iostream>
#include <array>
using namespace std;

template<int... P>
constexpr int sum() {
	constexpr int n = sizeof...(P);
	array<int, n> p = {P...};
	int res = 0;
	for (auto v : p) res += v;
	return res;
}

int main() {
	int res = sum<1, 2, 3>();
	cout << res << '\n';
}
