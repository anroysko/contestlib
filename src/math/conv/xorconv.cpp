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
