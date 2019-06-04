#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

// Supports range addition and range sum
// https://codeforces.com/blog/entry/18051
template<class T = ll>
struct SegTree {
	vector<T> seg;
	vector<T> add;
	vector<int> siz;
	int n;
	
	SegTree(const vector<T>& vals) : n(vals.size()), seg(2*vals.size()), add(vals.size(), 0), siz(vals.size(), 2) {
		for (int i = 0; i < n; ++i) seg[i+n] = vals[i];
		for (int i = n-1; i > 0; --i) seg[i] = seg[2*i] + seg[2*i+1];
		for (int i = (n-1)/2; i > 0; --i) siz[i] = siz[2*i] + siz[2*i+1];
	}

	void apply(int i, T v) {
		if (i < n) {
			seg[i] += v * siz[i];
			add[i] += v;
		} else seg[i] += v;
	}
	void push(int i, int b = 31) {
		for (; b > 0; --b) {
			int j = i >> b;
			if (j && add[j]) {
				apply(2*j, add[j]);
				apply(2*j+1, add[j]);
				add[j] = 0;
			}
		}
	}
	void update(int i) {
		for (i /= 2; i > 0; i /= 2) {
			seg[i] = seg[2*i] + seg[2*i+1] + siz[i]*add[i];
		}
	}
	T rangeSum(int a0, int b0) {
		push(a0 + n);
		push(b0 + n);
		int a = a0 + n;
		int b = b0 + n+1;
		T res = 0;
		while(a < b) {
			if (a & 1) {
				res += seg[a];
				++a;
			}
			if (b & 1) {
				--b;
				res += seg[b];
			}
			a /= 2;
			b /= 2;
		}
		return res;
	}
	void rangeAdd(int a0, int b0, T v) {
		int a = a0 + n;
		int b = b0 + n+1;
		while(a < b) {
			if (a & 1) {
				apply(a, v);
				++a;
			}
			if (b & 1) {
				--b;
				apply(b, v);
			}
			a /= 2;
			b /= 2;
		}
		update(a0 + n);
		update(b0 + n);
	}
};

// Example usage
int main() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n);
	for (int i = 0; i < n; ++i) {
		cin >> vals[i];
	}
	SegTree<> seg(vals);

	// Queries use zero-indexing
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "range_add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			seg.rangeAdd(a, b, v);
		} else if (op == "range_sum") {
			int a, b;
			cin >> a >> b;
			ll ans = seg.rangeSum(a, b);
			cout << ans << '\n';
		}
	}
}
