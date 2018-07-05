#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <stdlib.h>
using namespace std;

typedef long long ll;
typedef long double ld;
const ld  EPS = (ld)1.0 / 1e5;
const int MOD = 1e9 + 7;

struct Mat {
	int w, h;
	vector<int> data;
	// vector<ld> data;

	Mat(int w, int h) {
		this->w = w;
		this->h = h;
		data.resize(w*h);
	}
	void resize(int w, int h) {
		this->w = w;
		this->h = h;
		data.resize(w * h);
	}
	int& operator()(int x, int y) {
	// ld& operator()(int x, int y) {
		return data[x + y * w];
	}
};

// Finds gcd(a, b), and returns c, d such that ac + bd = gcd(a, b) (MOD)
// Time complexity: log(c + d)
// Returns: {gcd(a, b), {c, d}}
pair<int, pair<int, int>> extEuc(int a, int b) {
	if (b == 0) return {a, {1, 0}};
	int m = a / b;
	auto sub = extEuc(b, a - b * m);

	int ac = sub.second.second;
	int bc = sub.second.first - ((ll)m * sub.second.second) % MOD;
	if (bc < 0) bc += MOD;
	return {sub.first, {ac, bc}};
}

void printMat(Mat& mat) {
	cout << "[[";
	for (int y = 0; y < mat.h; ++y) {
		for (int x = 0; x < mat.w; ++x) {
			cout << mat(x, y);
			if (x + 1 < mat.w) cout << ",\t";
		}
		if (y + 1 < mat.h) cout << "],\n[";
	}
	cout << "]]\n";
}

// Computes the LUP decomposition of the given matrix M
// L is a lower triangular unit matrix, U is a upper triangular matrix, P is a permutation matrix.
// If there is no LUP decomposition (matrix is not singular), U doesn't 
// it holds that LU = PM
// Time Complexity: O(N^3)
// Preconditions: M is a square matrix (mm.w == mm.h)
// Returns: true if the decomposition was successful.
void lupDecompose(Mat& mat_m, Mat& mat_l, Mat& mat_u, Mat& mat_p) {
	// Clear and initialize the matrices
	int n = mat_m.w;

	mat_l.resize(n, n);
	mat_u = mat_m;
	mat_p.resize(n, n);
	vector<int> tmp_p (n);

	for (int y = 0; y < n; ++y) {
		tmp_p[y] = y;
		for (int x = 0; x < n; ++x) {
			mat_l(x,y) = (x == y);
			if (mat_u(x,y) < 0) mat_u(x,y) += MOD;
			mat_p(x,y) = 0;
		}
	}

	// Loop invariant: l p M = u
	for (int i = 0; i < n; ++i) {
		int py = i; // Row we pivot on, pick one with largest abs
		int best = abs(mat_u(i,i));
		// ld best = abs(mat_u(i,i));
		for (int y = i; y < n; ++y) {
			int off = abs(mat_u(i,y));
			if (off > best) {
				py = y;
				best = off;
			}
		}
		if (best < EPS) continue;
		if (py != i) {
			// Operation: Swap rows i and py
			for (int x = 0; x < i; ++x) swap(mat_l(x, i), mat_l(x, py));
			for (int x = i; x < n; ++x) swap(mat_u(x, py), mat_u(x, i));
			swap(tmp_p[i], tmp_p[py]);
		}
		// Subtract row i from other rows
		for (int y = i+1; y < n; ++y) {
			ll mult = ((ll)(MOD-mat_u(i, y)) * extEuc(MOD, mat_u(i, i)).second.second) % MOD;
			// ld mult = -mat_u(i,y) / mat_u(i,i);
			for (int x = 0; x <= i; ++x) {
				mat_l(x,y) = (mat_l(x,y) + mult * mat_l(x,i)) % MOD;
				// mat_l(x,y) += mult * mat_l(x,i);
			}
			for (int x = i; x < n; ++x) {
				mat_u(x,y) = (mat_u(x,y) + mult * mat_u(x,i)) % MOD;
				// mat_u(x,y) += mult * mat_u(x,i);
			}
		}
	}

	// Inverse mat_l
	Mat mat_l_inv (n, n);
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			mat_l_inv(x,y) = (x == y);
		}
	}

	//        [1 0 0]
	//        [? 1 0]
	//        [? ? 1]
	// [1 0 0][1 0 0]
	// [? 1 0][0 1 0]
	// [? ? 1][0 0 1]

	for (int x = 0; x < n; ++x) {
		for (int y = x+1; y < n; ++y) {
			// ld sum_known = 0;
			ll sum_known = 0;
			for (int i = 0; i <= y; ++i) {
				// sum_known += mat_l_inv(i,y) * mat_l(x,i);
				sum_known += ((ll)mat_l_inv(i,y) * mat_l(x,i)) % MOD;
			}

			// mat_l_inv(x,y) = -sum_known;
			mat_l_inv(x,y) = (MOD - sum_known) % MOD;
		}
	}

	// Fix the permutation
	for (int i = 0; i < n; ++i) mat_p(i, tmp_p[i]) = 1;

	// Our actual L is l_inv. Move it over
	mat_l.data = move(mat_l_inv.data);
}

int determinant(Mat& mat) {
	int n = mat.w;
	Mat mat_l(n,n);
	Mat mat_u(n,n);
	Mat mat_p(n,n);
	lupDecompose(mat, mat_l, mat_u, mat_p);
	ll res = 1;
	for (int i = 0; i < n; ++i) {
		res = (res * mat_u(i,i)) % MOD;
	}
	vector<int> tmp_p(n);
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			if (mat_p(x,y)) tmp_p[y] = x;
		}
	}
	int sign = 1;
	for (int i = 0; i < n; ++i) {
		for (int j = i-1; j >= 0; --j) {
			if (tmp_p[j] > tmp_p[j+1]) {
				sign *= -1;
				swap(tmp_p[j], tmp_p[j+1]);
			}
		}
	}
	res *= sign;
	if (res < 0) res += MOD;
	if (res >= MOD / 2) res -= MOD;
	return res;
}

const int M = 3;

int main() {
	int n, s;
	cin >> n >> s;
	srand(s);

	Mat mat_m(n, n);
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			mat_m(x,y) = (rand() % M) - M / 2;
		}
	}
	printMat(mat_m);
	cout << determinant(mat_m) << '\n';

	/*
	Mat mat_l(n, n);
	Mat mat_u(n, n);
	Mat mat_p(n, n);
	lupDecompose(mat_m, mat_l, mat_u, mat_p);

	cout << "L:\n";
	printMat(mat_l);
	cout << "U:\n";
	printMat(mat_u);
	cout << "P:\n";
	printMat(mat_p);
	cout << "M:\n";
	printMat(mat_m);
	*/
}
