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
const int M = 1000;
const int P = 998244353;

void genTest(ofstream& fout) {
	int t = rand(0, 1);
	fout << t << '\n';
	if (t == 0) {
		int n = rand(1, N);
		int m = rand(1, M);
		fout << n << ' ' << m << '\n';

		for (int i = 0; i < n; ++i) fout << rand(1, P-1) << ' '; fout << '\n';
		for (int i = 0; i < m; ++i) fout << rand(1, P-1) << ' '; fout << '\n';
	} else {
		int n = rand(1, N);
		int a = rand(1, P);
		int b = rand(1, P);
		int k = rand(1, M);
		fout << n << ' ' << a << ' ' << b << ' ' << k << '\n';

		for (int i = 0; i < n; ++i) fout << rand(1, P-1) << ' '; fout << '\n';
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
