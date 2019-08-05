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
const ll V1 = 1e9;
const ll V2 = 1e18;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int q = rand(1, Q);
	fout << n << ' ' << q << '\n';

	vector<ll> xs(n);
	for (int i = 0; i < n; ++i) xs[i] = rand(-V1, V1);
	sort(xs.begin(), xs.end());
	for (auto v : xs) fout << v << ' '; fout << '\n';

	int cou = 0;
	vector<string> op_types = {"add_line", "min_line", "undo"};
	for (int qi = 0; qi < q; ++qi) {
		int oi = rand(0, (int)op_types.size() - 1);
		string op = op_types[oi];
		if (cou == 0) op = "add_line";

		fout << op << ' ';
		if (op == "add_line") {
			++cou;
			ll a = rand(-V1, V1);
			ll b = rand(-V2, V2);
			fout << a << ' ' << b << '\n';
		} else if (op == "min_line") {
			int j = rand(0, n-1);
			fout << j << '\n';
		} else if (op == "undo") {
			--cou;
			fout << '\n';
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
