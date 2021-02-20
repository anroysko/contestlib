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
