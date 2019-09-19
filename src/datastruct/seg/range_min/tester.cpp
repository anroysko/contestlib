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
const ll V = (ll)1e18;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	fout << n << ' ' << q << '\n';
	for (int i = 0; i < n; ++i) fout << rand(-V, V) << ' '; fout << '\n';

	vector<string> op_types = {"get", "set", "find"};
	for (int qi = 0; qi < q; ++qi) {
		int oi = rand(0, (int)op_types.size() - 1);
		string op = op_types[oi];

		fout << op << ' ';
		if (op == "get") {
			int a = rand(0, n-1);
			int b = rand(0, n-1);
			if (b < a) swap(a, b);
			fout << a << ' ' << b << '\n';
		} else if (op == "set") {
			int i = rand(0, n-1);
			ll v = rand(-V, V);
			fout << i << ' ' << v << '\n';
		} else if (op == "find") {
			int i = rand(0, n);
			ll v = rand(-V, V);
			fout << i << ' ' << v << '\n';
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
