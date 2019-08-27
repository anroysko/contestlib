
// Calculates vM, M = m[k-1] x ... x m[1] x m[0] where x is the kronecker product
// M_{y,x} = \prod_{s = 0}^{k-1} m[s][2*(bool)(y & 2^s) + (bool)(x & 2^s)]
vector<ll> kronecker(vector<ll> v, const vector<vector<ll>>& m) {
	for (int j = 0; j < m.size(); ++j) {
		for (int i = 0; i < v.size(); ++i) {
			if (i & (1<<j)) continue;
			ll ri = v[i^(1<<j)];
			v[i^(1<<j)] = v[i] * m[j][1] + ri * m[j][3];
			v[i] = v[i] * m[j][0] + ri * m[j][2];
		}
	}
	return v;
}
