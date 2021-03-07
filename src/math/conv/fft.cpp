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


void fft(vector<complex<ld>>& pol, complex<ld> r) {
	int n = pol.size();
	bitReverse(pol);

	// roots[h + x] = r^(nx / 2h) when h = 2^k and 0 <= x < h
	// Calculated this way to limit precision error
	vector<complex<ld>> roots(n, {1, 0});
	for (int h = n/2; h > 1; h >>= 1, r *= r) roots[h+1] = r;
	for (int h = 4; h < n; h <<= 1) {
		for (int x = h+2; x < 2*h; x += 2) {
			roots[x] = roots[x >> 1];
			roots[x ^ 1] = roots[x] * roots[h+1];
		}
	}

	for (int h = 1; h < n; h <<= 1) {
		for (int j = 0; j < n; j += 2*h) {
			for (int i = j; i < j + h; ++i) {
				auto tmp = roots[h + (i-j)] * pol[i + h];
				pol[i + h] = pol[i] - tmp;
				pol[i] += tmp;
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
	complex<ld> nth_root = {cos(2*PI/n), sin(2*PI/n)};
	
	vector<complex<ld>> vec(n, 0);
	for (int i = 0; i < n; ++i) vec[i] = {(i < as ? a[i] : 0), (i < bs ? b[i] : 0)};
	fft(vec, nth_root);

	// https://en.wikipedia.org/wiki/Discrete_Fourier_transform#DFT_of_real_and_purely_imaginary_signals
	// ans1[i] = conj(ans1[n-i])
	// ans2[i] = -conj(ans2[n-i])
	for (int i = 0; i < n-i; ++i) {
		ld va = (vec[i] + conj(vec[n-1-i])) / 2;
		ld vb = ((vec[i] - conj(vec[n-1-i])) / 2);
		vb = {vb.imag(), vb.real()};

		vec[i] = va * vb;
		vec[n-1-i] = conj(va) * -conj(vb);
	}
	fft(vec, conj(nth_root));
	
	vector<T> res(as + bs - 1);
	for (int i = 0; i < as + bs - 1; ++i) {
		res[i] = vec[i].real() / n; // round(cp[i].real() / n); // UNCOMMENT FOR INTEGRAL T
	}
	return res;
}
