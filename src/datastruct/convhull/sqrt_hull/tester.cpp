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
const ll V1 = 5000;
const ll V2 = 5000;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	fout << n << ' ' << q << '\n';

	for (int i = 0; i < n; ++i) fout << rand(-V1, V1) << ' ' << rand(-V2, V2) << '\n';

	vector<string> op_types = {"add", "min"};
	for (int qi = 0; qi < q; ++qi) {
		string op = op_types[rand(0, 1)];
		int a = rand(0, n-1);
		int b = rand(a, n-1);
		ll x = rand(-V1, V1);
		fout << op << ' ' << a << ' ' << b << ' ' << x << '\n';
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
