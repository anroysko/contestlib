
void pushb(vector<int>& v1, const vector<int>& v2, int a, int b) {
	for (int i = a; i <= b; ++i) v1.push_back(v2[i]);
}
void popb(vector<int>& v1, int k) {
	while(--k) v1.pop_back();
}

template<class M>
int findFree(int n, vector<int> cur, const vector<int>& tar, M& mat) {
	int m = cur.size();
	int a = 0, b = (int)tar.size() - 1;

	pushb(cur, tar, a, b);
	if (mat.rank(cur) <= m) return -1;
	popb(cur, b-a+1);

	while(a < b) {
		int m = (a+b) / 2;
		pushb(cur, tar, a, m);
		if (mat.rank(cur) > m) b = m;
		else a = m+1;
		popb(cur, m-a+1);
	}
	return tar[a];
}
template<class M>
int findSwap(int n, vector<int> cur, int tar, M& mat) {
	if (mat.indep(
	int m = cur.size();
	int a = 0, b = (int)tar.size() - 1;
}

template<class M>
bool indep(vector<int>& vec, int x, M& mat) {
	vec.push_back(x);
	bool res = mat.independent(vec);
	vec.pop_back();
	return res;
}

template<class M1, class M2>
void updDist(vector<int>& dist, const vector<int>& bip, M1& mat1, M2& mat2) {
	for (int i = 0; i < n; ++i) {
		// If i is in 
	}
}

// Returns a set in the intersection of matroids M1 and M2 of maximum size
template<class M1, class M2>
vector<int> matroidITS(M1& mat1, M2& mat2) {
	int n = mat1.size();
	vector<int> dist(n, 0);
}
