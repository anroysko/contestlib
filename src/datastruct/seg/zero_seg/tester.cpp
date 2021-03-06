#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}

const int TC = 100;
const int N = 1000;
const int Q = 1000;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	ll v = (ll)1e18 / max(n, q);
	if (rand(0, 1)) v = rand(1, 100);

	fout << n << ' ' << q << '\n';
	for (int i = 0; i < n; ++i) fout << rand(-v, v) << ' '; fout << '\n';
	for (int i = 0; i < n; ++i) fout << rand(-v, v) << ' '; fout << '\n';

	vector<string> op_types = {"add", "get"};
	for (int qi = 0; qi < q; ++qi) {
		int oi = rand(0, (int)op_types.size() - 1);
		string op = op_types[oi];

		int a = rand(0, n-1);
		int b = rand(0, n-1);
		if (b < a) swap(a, b);
		fout << op << ' ' << a << ' ' << b;

		if (op == "get") {
			fout << '\n';
		} else if (op == "add") {
			ll val = rand(-v, v);
			fout << ' ' << val << '\n';
		}
	}
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout;
		fout.open("test.in");
		
		int t = TC;
		fout << t << '\n';
		for (int ti = 0; ti < t; ++ti) genTest(fout);

		fout.close();

		bool fail = false;
		fail |= system("./main < test.in > main.out");
		fail |= system("./brute < test.in > brute.out");
		fail |= system("diff main.out brute.out");
		if (fail) {
			cout << "BUG FOUND!\n";
			break;
		}
	}
}
