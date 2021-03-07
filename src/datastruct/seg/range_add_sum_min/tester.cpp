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

const int TC = 1000;
const int N = 100;
const int Q = 100;
const ll V = (ll)1e18 / (N*Q);

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	fout << n << ' ' << q << '\n';

	vector<string> op_types = {"add", "sum", "min"};
	for (int qi = 0; qi < q; ++qi) {
		int oi = rand(0, (int)op_types.size() - 1);
		string op = op_types[oi];

		int a = rand(0, n-1);
		int b = rand(0, n-1);
		if (b < a) swap(a, b);
		fout << op << ' ' << a << ' ' << b;

		if (op == "sum" || op == "min") {
			fout << '\n';
		} else if (op == "add") {
			ll v = rand(-V, V);
			fout << ' ' << v << '\n';
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
