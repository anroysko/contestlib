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
const int Q = 1000;
const ll V1 = 1e9;
const ll V2 = 1e9;

void genTest(ofstream& fout) {
	int q = rand(1, Q);
	fout << q << '\n';

	int lc = 0;
	vector<int> ops(q);
	for (int qi = 0; qi < q; ++qi) {
		if (qi == 0 || rand(0, 1)) {
			ops[qi] = lc;
			++lc;
		} else {
			ops[qi] = -1;
		}
	}
	vector<ll> as(lc);
	for (int j = 0; j < lc; ++j) as[j] = rand(-V1, V1);
	sort(as.begin(), as.end());

	vector<string> op_types = {"add_line", "min_line"};
	for (int qi = 0; qi < q; ++qi) {
		string op = op_types[ops[qi] == -1];
		fout << op << ' ';
		if (op == "add_line") {
			ll a = as[ops[qi]];
			ll b = rand(-V2, V2);
			fout << a << ' ' << b << '\n';
		} else if (op == "min_line") {
			ll x = rand(-V1, V1);
			fout << x << '\n';
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
