#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
using ll = long long;

template<class T=ll, T zero=0, T one=1>
struct Matrix {
	vector<T> mat;
	int h, w;

	Matrix(int height, int width) : h(height), w(width), mat(height*width, zero) {}

	const T& operator()(int y, int x) const {
		return mat[x+y*w];
	}
	T& operator()(int y, int x) {
		return mat[x+y*w];
	}
	static Matrix ident(int n) {
		Matrix res(n, n);
		for (int i = 0; i < n; ++i) res(i, i) = one;
		return res;
	}

	Matrix& operator+=(const Matrix& rhs) {
		assert(h == rhs.h && w == rhs.w);
		for (int i = 0; i < w*h; ++i) mat[i] += rhs.mat[i];
	}
	Matrix operator*(const Matrix& rhs) const {
		assert(w == rhs.h);
		Matrix res(h, rhs.w);
		for (int i = 0; i < w; ++i) {
			int b1 = i * rhs.w;
			for (int y = 0; y < res.h; ++y) {
				T mult = mat[i + y * w];
				int b2 = y * res.w;
				for (int x = 0; x < res.w; ++x) {
					res.mat[b2 + x] += mult * rhs.mat[b1 + x];
				}
			}
		}
		return res;
	}
	Matrix pow(ll d) const {
		assert(d >= 0 && w == h);
		Matrix res = ident(w);
		for (ll b = 1ll<<62; b > 0; b /= 2) {
			if (d > b) res = res * res;
			if (d & b) res = res * (*this);
		}
		return res;
	}
};

// Example usage. Finds matrix power.
int main() {
	int h, w, d;
	cin >> h >> w >> d;
	Matrix<> m(h, w);
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			cin >> m(y,x);
		}
	}

	auto mp = m.pow(d);
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			cout << mp(y,x) << ' ';
		}
		cout << '\n';
	}
}
