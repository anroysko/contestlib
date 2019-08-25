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
const int N = 10000;
const int Q = 10000;
const ll V = 1e18;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	ll v = rand(1ll, V/q);
	fout << n << ' ' << q << '\n';

	vector<string> op_types = {"add", "get", "search"};
	int oc = op_types.size();
	for (int qi = 0; qi < q; ++qi) {
		string op = op_types[rand(0, oc-1)];

		fout << op << ' ';
		if (op == "add") {
			int i = rand(0, n-1);
			ll val = rand(0ll, v);
			fout << i << ' ' << val << '\n';
		} else if (op == "get") {
			int a = rand(0, n-1);
			int b = rand(0, n-1);
			if (b < a) swap(a, b);
			fout << a << ' ' << b << '\n';
		} else if (op == "search") {
			ll val = rand(0ll, v*q);
			fout << val << '\n';
		}
	}
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout("test.in");
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
