// res[k] = \sum_{i,j : i^j == k} a[i] b[j]
// overflows if n res[i] overflows
template<class T>
void xorTF(vector<T>& v) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			T ri = v[i^b];
			v[i^b] = v[i] - ri;
			v[i] += ri;
		}
	}
}
template<class T>
vector<T> xorConv(vector<T> a, vector<T> b) {
	xorTF(a); xorTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	xorTF(a);
	for (int i = 0; i < a.size(); ++i) a[i] /= (T)a.size();
	return a;
}
// res[k] = \sum_{i,j : i&j == k} a[i] b[j]
// overflows if (\sum a)(\sum b) overflows
template<class T>
void andTF(vector<T>& v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) continue;
			T le = v[i];
			v[i] = v[i^b] + (inv ? -le : 0);
			v[i^b] = le + (inv ? 0 : v[i^b]);
		}
	}
}
template<class T>
vector<T> andConv(vector<T> a, vector<T> b) {
	andTF(a); andTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	andTF(a, true);
	return a;
}
// res[k] = \sum_{i,j : i|j == k} a[i] b[j]
// overflows if (\sum a)(\sum b) overflows
template<class T>
void orTF(vector<T>& v, bool inv = false) {
	for (int b = 1; b < v.size(); b *= 2) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & b) v[i] += (inv ? -1 : 1) * v[i^b];
		}
	}
}
template<class T>
vector<T> orConv(vector<T> a, vector<T> b) {
	orTF(a); orTF(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	orTF(a, true);
	return a;
}
