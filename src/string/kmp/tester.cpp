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

const int N = 100;
const int C = 26;
const int TC = 1000;

void genTest(ofstream& fout) {
	int n = rand(1, N);
	int m = rand(1, N);
	int c = rand(0, C-1);
	string str(n, '_'), pat(m, '_');
	for (int i = 0; i < n; ++i) str[i] = rand('a', (char)('a' + c));
	for (int i = 0; i < m; ++i) pat[i] = rand('a', (char)('a' + c));
	fout << str << ' ' << pat << '\n';
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
