// res[k] = \sum_{i,j : i^j == k} a[i] b[j]
// overflows if n res[i] overflows for some i
valarray<ll> xorTF(valarray<ll> v) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			ll ri = v[i^b];
			v[i^b] = v[i] - ri;
			v[i] += ri;
		}
	}
	return v;
}
vector<ll> xorConv(const vector<ll>& a, const vector<ll>& b, int h) {
	valarray<ll> res = xorTF(xorTF({a.data(), 1ull<<h}) * xorTF({b.data(), 1ull<<h})) >> h;
	return vector<ll>(begin(res), end(res));
}

// res[k] = \sum_{i,j : i&j == k} a[i] b[j]
// overflows if res[i] overflows for some i
valarray<ll> andTF(valarray<ll> v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			ll le = v[i];
			v[i] = v[i^b] + (inv ? -le : 0);
			v[i^b] = le + (inv ? 0 : v[i^b]);
		}
	}
	return v;
}
vector<ll> andConv(const vector<ll>& a, const vector<ll>& b, int h) {
	valarray<ll> res = andTF(andTF({a.data(), 1ull<<h}) * andTF({b.data(), 1ull<<h}), true);
	return vector<ll>(begin(res), end(res));
}

// res[k] = \sum_{i,j : i|j == k} a[i] b[j]
// overflows if (\sum a)(\sum b) overflows
valarray<ll> orTF(valarray<ll> v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) v[i] += (inv ? -1 : 1) * v[i^b];
		}
	}
	return v;
}
vector<ll> orConv(const vector<ll>& a, const vector<ll>& b, int h) {
	valarray<ll> res = orTF(orTF({a.data(), 1ull<<h}) * orTF({b.data(), 1ull<<h}), true);
	return vector<ll>(begin(res), end(res));
}
