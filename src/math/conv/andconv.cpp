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
