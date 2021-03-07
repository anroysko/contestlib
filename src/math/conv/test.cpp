#include <bits/stdc++.h>
using namespace std;
using ld = long double;
using ll = long long;
using co = complex<ld>;

void fft(vector<co>& pol, co r) {
	int n = pol.size();
	for (int i = 0; i < n; ++i) {
		int j = 0;
		for (int b = 1; b < n; b *= 2) j = 2*j + bool(i & b);
		if (i < j) swap(pol[i], pol[j]);
	}

	// roots[h + x] = r^(nx / 2h) when h = 2^k and 0 <= x < h
	// Calculated this way to limit precision error
	vector<co> roots(n, {1, 0});
	for (int h = n/2; h > 1; h >>= 1, r *= r) roots[h+1] = r;
	for (int h = 4; h < n; h <<= 1) {
		for (int x = h+2; x < 2*h; x += 2) {
			roots[x] = roots[x >> 1];
			roots[x ^ 1] = roots[x] * roots[h+1];
		}
	}

	for (int h = 1; h < n; h *= 2) {
		for (int i = 0; i < n; i += h) {
			for (int j = i + h, w = h; w < 2*h; ++i, ++j, ++w) {
				co x = roots[w] * pol[j];
				pol[j] = pol[i] - x;
				pol[i] = pol[i] + x;
			}
		}
	}
}

// Polynomial multiplication: res[k] = \sum_{t = 0}^{k} a[t] b[k-t]
template<class T>
vector<T> polyMult(const vector<T>& a, const vector<T>& b) {
	int as = a.size(), bs = b.size(), n = 1;
	while(n < (as + bs)) n <<= 1;

	const ld PI = atan((ld)1) * 4;
	co nth_root = {cos(2*PI/n), sin(2*PI/n)};
	
	vector<co> tmpa(n);
	for (int i = 0; i < n; ++i) tmpa[i] = {a[i], 0};
	fft(tmpa, nth_root);
	
	vector<co> tmpb(n);
	for (int i = 0; i < n; ++i) tmpb[i] = {0, b[i]};
	fft(tmpb, nth_root);
	
	for (int i = 0; i < n; ++i) cerr << tmpa[i] - tmpa[(n-i)%n] << ' '; cerr << '\n';
	for (int i = 0; i < n; ++i) cerr << tmpb[i] + tmpb[(n-i)%n] << ' '; cerr << '\n';

	vector<co> vec(n, 0);
	for (int i = 0; i < n; ++i) vec[i] = {ld(i < as ? a[i] : 0), ld(i < bs ? b[i] : 0)};
	fft(vec, nth_root);

	// https://en.wikipedia.org/wiki/Discrete_Fourier_transform#DFT_of_real_and_purely_imaginary_signals
	// ans1[i] = conj(ans1[n-i])
	// ans2[i] = -conj(ans2[n-i])
	for (int i = 0; i <= n-i; ++i) {
		int r = (n-i)%n;
		co va = (vec[i] + conj(vec[r])) * ld(0.5);
		co vb = (vec[i] - conj(vec[r])) * ld(0.5);
		vb = {vb.imag(), vb.real()};

		vec[i] = va * vb;
		vec[r] = -conj(vec[i]);
	}
	fft(vec, conj(nth_root));
	
	vector<T> res(as + bs - 1);
	for (int i = 0; i < as + bs - 1; ++i) res[i] = round(vec[i].real() / n);
	return res;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cerr << fixed << setprecision(1);

	int n, m;
	cin >> n >> m;
	vector<ll> as(n), bs(m);
	for (ll& a : as) cin >> a;
	for (ll& b : bs) cin >> b;
	vector<ll> res = polyMult(as, bs);

	const ll MOD = 998244353;
	for (ll& v : res) cout << (v % MOD) << ' '; cout << '\n';
}
