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
const int M = 10;
const ll V1 = (ll)1e18 >> (2*M);
const ll V2 = 2;

void genTest(ofstream& fout) {
	int m = rand(1, M);
	int n = 1<<m;
	fout << m << '\n';
	for (int i = 0; i < n; ++i) fout << rand(-V1, V1) << ' '; fout << '\n';
	for (int j = 0; j < m; ++j) {
		for (int i = 0; i < 4; ++i) fout << rand(-V2, V2) << ' '; fout << '\n';
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
