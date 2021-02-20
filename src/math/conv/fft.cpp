#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <assert.h>
using namespace std;

const double PI = atan((double)1) * 4;

// Applies the bit-reverse permutation to the given vector
// Time Complexity: O(n log n)
template<class T>
void bitReverse(vector<T>& vec) {
	int n = vec.size();
	for (int i = 0; i < n; ++i) {
		int t = 0;
		for (int j = 0; (1 << j) < n; ++j) {
			if (i & (1 << j)) t |= n >> (j+1);
		}
		if (i < t) swap(vec[i], vec[t]);
	}
}

// Iterative FFT.
// Assumes that pol.size() = 2^m for some integer m, and that nth_root is the nth root of unity or its inverse.
// Time Complexity: O(n log n)
void fft(vector<complex<double>>& pol, complex<double> nth_root) {
	int n = pol.size();
	bitReverse(pol); // Lets us calculate FFT iteratively

	int m = 0;
	while((1 << m) < n) ++m;
	vector<complex<double>> pows(m); // pows[i] = nth_root^(2^(m-1 - i))
	pows.back() = nth_root;
	for (int i = m - 2; i >= 0; --i) pows[i] = pows[i + 1] * pows[i + 1];
	
	for (int k = 0; k < m; ++k) {
		int h = 1 << k;	// half of step length
		int len = h << 1; // step length
		complex<double> root = pows[k];
		for (int j = 0; j < n; j += len) {
			complex<double> curr = 1;
			for (int i = j; i < j + h; ++i) {
				auto tmp = curr * pol[i + h];
				pol[i + h] = pol[i] -  tmp;
				pol[i] += tmp;
				curr *= root;
			}
		}
	}
}

// Calculates the product of two polynomials a and b.
// Time Complexity: O(n log n) where n = O(max(a.size(), b.size()))
vector<double> polyMult(const vector<double>& a, const vector<double>& b) {
	int as = a.size(), bs = b.size();
	int n = 1;
	while(n < (as + bs)) n <<= 1;
	
	vector<complex<double>> ap (n, 0), bp (n, 0);
	for (int i = 0; i < as; ++i) ap[i] = a[i];
	for (int i = 0; i < bs; ++i) bp[i] = b[i];
	
	complex<double> nth_root = {cos(2*PI/n), sin(2*PI/n)};
	fft(ap, nth_root);
	fft(bp, nth_root);

	vector<complex<double>> cp (n);
	for (int i = 0; i < n; ++i) cp[i] = ap[i] * bp[i];
	
	fft(cp, conj(nth_root)); // conj(nth_root) = 1 / nth_root, since len(nth_root) = 1.
	
	vector<double> res(as + bs - 1);
	for (int i = 0; i < res.size(); ++i) res[i] = cp[i].real() / n;
	return res;
}

int main() {
	int as, bs;
	cin >> as >> bs;
	vector<double> a(as);
	vector<double> b(bs);
	for (int i = 0; i < as; ++i) cin >> a[i];
	for (int i = 0; i < bs; ++i) cin >> b[i];
	
	vector<double> ab = polyMult(a, b);
	for (int i = 0; i < ab.size(); ++i) cout << ab[i] << ' '; cout << '\n';
	
	vector<double> ans (as+bs-1, 0);
	for (int i = 0; i < as; ++i) {
		for (int j = 0; j < bs; ++j) {
			ans[i+j] += a[i] * b[j];
		}
	}
	for (int i = 0; i < ans.size(); ++i) cout << ans[i] << ' '; cout << '\n';
}
