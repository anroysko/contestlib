#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

// Supports range minimum and setting value of element
// https://codeforces.com/blog/entry/18051
template<class T = int>
struct SegTree {
	vector<T> seg;
	int n;
	
	SegTree(const vector<T>& vals) : n(vals.size()), seg(2*vals.size()) {
		for (int i = 0; i < n; ++i) seg[i+n] = vals[i];
		for (int i = n-1; i > 0; --i) seg[i] = min(seg[2*i], seg[2*i+1]);
	}
	T rangeMin(int a, int b) const {
		a += n;
		b += n+1;
		T res = seg[a];
		while(a < b) {
			if (a & 1) {
				res = min(res, seg[a]);
				++a;
			}
			if (b & 1) {
				--b;
				res = min(res, seg[b]);
			}
			a >>= 1;
			b >>= 1;
		}
		return res;
	}
	void setVal(int i, T v) {
		i += n;
		seg[i] = v;
		for (i >>= 1; i > 0; i >>= 1) seg[i] = min(seg[2*i], seg[2*i+1]);
	}
};

// Example usage. Demonstrates using pair<int, int> to get index of minimum
int main() {
	int n, q;
	cin >> n >> q;

	vector<pair<int, int>> vals(n);
	for (int i = 0; i < n; ++i) {
		cin >> vals[i].first;
		vals[i].second = i;
	}
	
	// SegTree seg(...) defaults to SegTree<int>. Here we need explicit declaration
	SegTree<pair<int, int>> seg(vals);

	// Queries use zero-indexing
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "set_val") {
			int i, v;
			cin >> i >> v;
			seg.setVal(i, {v, i});
		} else if (op == "range_min") {
			int a, b;
			cin >> a >> b;
			pair<int, int> ans = seg.rangeMin(a, b);
			cout << ans.second << ' ' << ans.first << '\n'; // index, value
		}
	}
}
