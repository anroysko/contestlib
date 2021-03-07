#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <stdlib.h>
#include <time.h>
using namespace std;
typedef long long ll;

const int N = 10;
const ll W = 1e17;

ll longRand() {
	const ll BS = 1e9;
	return (random() % BS) + random() * BS;
}

int main() {
	srand(time(0));
	for (int ti = 0;; ++ti) {
		if (ti > 0 && ti % 20 == 0) cout << endl;
		cout << ti << ' ';

		int n = random() % N + 1;
		
		ofstream fout;
		fout.open("test.in");
		fout << n << '\n';
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) fout << longRand() % W << ' '; fout << '\n';
		}
		fout.close();

		bool fail = false;
		fail |= system("./hungarian < test.in > aaa.out");
		fail |= system("./brute < test.in > bbb.out");
		fail |= system("diff aaa.out bbb.out");
		if (fail) {
			cout << "BUG FOUND!\n";
			break;
		}
	}
}
