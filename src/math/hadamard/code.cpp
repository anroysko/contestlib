// res[k] = \sum_{i,j : i^j == k} a[i] b[j]
// overflows if n res[i] overflows
void xorTF(vector<ll>& v) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			ll ri = v[i^b];
			v[i^b] = v[i] - ri;
			v[i] += ri;
		}
	}
}
vector<ll> xorConv(vector<ll> a, vector<ll> b) {
	xorTF(a); xorTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	xorTF(a);
	for (int i = 0; i < a.size(); ++i) a[i] /= (ll)a.size();
	return a;
}

// res[k] = \sum_{i,j : i&j == k} a[i] b[j]
// overflows if (\sum a)(\sum b) overflows
void andTF(vector<ll>& v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			ll le = v[i];
			v[i] = v[i^b] + (inv ? -le : 0);
			v[i^b] = le + (inv ? 0 : v[i^b]);
		}
	}
}
vector<ll> andConv(vector<ll> a, vector<ll> b) {
	andTF(a); andTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	andTF(a, true);
	return a;
}

// res[k] = \sum_{i,j : i|j == k} a[i] b[j]
// overflows if (\sum a)(\sum b) overflows
void orTF(vector<ll>& v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) v[i] += (inv ? -1 : 1) * v[i^b];
		}
	}
}
vector<ll> orConv(vector<ll> a, vector<ll> b) {
	orTF(a); orTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	orTF(a, true);
	return a;
}
