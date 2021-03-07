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

const int N = 2 * (int)1e5;
const int M = 1e4;

void genTest(ofstream& fout) {
	int n = N; // rand(1, N);

	fout << n << '\n';
	for (int i = 0; i < n; ++i) {
		int x = rand(-M, M);
		int y = rand(-M, M);
		fout << x << ' ' << y << '\n';
	}
}

int main() {
	for (int ti = 0;; ++ti) {
		ofstream fout;
		fout.open("test.in");
		
		/*
		int t = TC;
		fout << t << '\n';
		for (int ti = 0; ti < t; ++ti) genTest(fout);
		*/

		genTest(fout);
		fout << 0 << '\n';

		fout.flush();
		fout.close();

		break;
	}
}
