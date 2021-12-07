#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

const int TC = 100;
const int N = 10;
const int M = 100;
const int P = (int)1e9 + 7;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int m = rand(n, M);
	fout << n << ' ' << m << '\n';
	for (int i = 0; i < n; ++i) fout << rand(0, 1) * rand(0, P - 1) << ' '; fout << '\n';
	fout << 1 << ' ';
	for (int i = 0; i < n; ++i) fout << rand(0, 1) * rand(0, P - 1) << ' '; fout << '\n';
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout;
		fout.open("test.in");
		
		/*
		int t = TC;
		fout << t << '\n';
		for (int ti = 0; ti < t; ++ti) genTest(fout);
		*/

		genTest(fout);
		fout.flush();
		fout.close();

		bool fail = false;
		fail |= system("./main < test.in > code.out");
		if (fail) {
			cout << "BUG FOUND!\n";
			break;
		}
	}
}
