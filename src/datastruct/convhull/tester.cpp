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
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

const int Q = 1e4;
const int N = 1e4;
const ll V = 1e9;

void writeTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	fout << n << ' ' << q << '\n';
	for (int i = 0; i < n; ++i) {
		fout << rand(-V, V) << ' ' << rand(-V*V, V*V) << '\n';
	}
	for (int i = 0; i < q; ++i) fout << rand(-V, V) << '\n';
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout;
		fout.open("test.in");
		
		// CREATE TEST DATA HERE
		writeTest(fout);
		// ...

		fout.close();

		bool fail = false;
		fail |= system("./code < test.in > code.out");
		fail |= system("./brute < test.in > brute.out");
		fail |= system("diff code.out brute.out");
		if (fail) {
			cout << "BUG FOUND!\n";
			break;
		}
	}
}
