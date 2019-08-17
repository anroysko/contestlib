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
const int N = 10;
const ll V1 = 1e9;
const ll V2 = 1e18;

void genTest(ofstream& fout) {
	vector<string> op_types = {"mobius", "tieredMobius", "subsetConv"};
	string t = op_types[rand(0, (int)op_types.size() - 1)];
	fout << t << '\n';

	int n = rand(1, N);
	fout << n << '\n';

	if (t == "mobius") {
		ll r = rand(0, 1) ? -1 : 1;
		ll v = V2 >> (n-1);
		for (int i = 0; i < (1<<n); ++i) fout << rand(-v, v) << ' '; fout << '\n';
		fout << r << '\n';
	} else if (t == "tieredMobius") {
		ll v = V2 >> (n-1);
		for (int i = 0; i < (1<<n); ++i) fout << rand(-v, v) << ' '; fout << '\n';
	} else if (t == "subsetConv") {
		ll v = V1 >> (n-1);
		for (int i = 0; i < (1<<n); ++i) fout << rand(-v, v) << ' '; fout << '\n';
		for (int i = 0; i < (1<<n); ++i) fout << rand(-v, v) << ' '; fout << '\n';
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
