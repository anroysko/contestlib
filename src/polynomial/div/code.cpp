
// Finds the polynomial inverse pol * inv = 1 (mod x^n) in O(n log n) assuming pol[0] = 1
// Uses Hensel lifting: given pol * inv = 1 (mod x^h), find pol * inv' = 1 (mod x^2h)
// http://people.csail.mit.edu/madhu/ST12/scribe/lect06.pdf
template<int P>
vector<int> inverse(const vector<int>& pol, int n) {
	vector<int> inv = {1}, cur = {1};
	for (int h = 1; h < n; h <<= 1) {
		cur.resize(2*h, 0);
		for (int i = h; i < min(2*h, pol.size()); ++i) cur[i] = pol[i];
		vector<int> prod = polyMult<P>(cur, inv);

		vector<int> high(h);
		for (int i = 0; i < h; ++i) high[i] = prod[i+h];
		high = polyMult<P>(high, inv);

		inv.resize(2*h);
		for (int i = 0; i < h; ++i) inv[i+h] = high[i];
	}
	inv.resize(n);
	return inv;
}

// Finds a such that p = aq + b for some b, deg(b) < deg(q). q must be monic (highest degree term has multiplier 1)
template<int P>
vector<int> polyDiv(vector<int> p, vector<int> q) {
	reverse(p.begin(), p.end());
	reverse(q.begin(), q.end());
	vector<int> rqi = inverse(q, (int)p.size() - (int)q.size());
	vector<int> ra = polyMult<P>(rqi, p);
	reverse(ra.begin(), ra.end());
	return ra;
}
