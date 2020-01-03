#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const int MOD = (int)1e9 + 7;

#include "code.cpp"

void solve() {
	int x, y;
	char op;
	cin >> x >> op >> y;
	
	auto a = ModNum<MOD>::init(x);
	auto b = ModNum<MOD>::init(y);
	if (op == '+') cout << a + b << ' ';
	else if (op == '-') cout << a - b << ' ';
	else if (op == '*') cout << a * b << ' ';
	else if (op == '/') cout << a / b << ' ';
	else cout << a.pow(y) << ' ';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) {
		solve();
	}
	cout << '\n';
}
