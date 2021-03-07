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

const int N = 1e6;
const int Q = 1e6;
const ll V = 1e6;

void buildTestCase(ofstream& fout) {
	int n = N; // rand(1, N);
	int q = Q; // rand(1, Q);
	fout << n << ' ' << q << '\n';
	for (int i = 0; i < n; ++i) fout << rand((ll)1, V) << ' '; fout << '\n';

	vector<string> ops = {"range_sum", "range_add"};
	for (int qi = 0; qi < q; ++qi) {
		const string& op = ops[rand(0, (int)ops.size()-1)];
		if (op == "range_sum") {
			int a = rand(0, n-1);
			int b = rand(a, n-1);
			fout << op << ' ' << a << ' ' << b << '\n';
		} else if (op == "range_add") {
			int a = rand(0, n-1);
			int b = rand(a, n-1);
			ll v = rand((ll)0, V);
			fout << op << ' ' << a << ' ' << b << ' ' << v << '\n';
		}
	}
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout;
		fout.open("test.in");
		
		// CREATE TEST DATA HERE
		buildTestCase(fout);
		// ...

		fout.close();

		break;

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
