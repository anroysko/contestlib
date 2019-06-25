#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 7 + (ll)1e18;

class ZeroTree {
	private:
		vector<pair<ll, ll>> seg;
		vector<ll> tag;
		int h = 1;

		pair<ll, ll> combine(pair<ll, ll> a, pair<ll, ll> b) {
			if (a.first != b.first) return min(a, b);
			else return {a.first, a.second + b.second};
		}
		void apply(int i, ll v) {
			seg[i].first += v;
			tag[i] += v;
		}
		void push(int i) {
			if (tag[i] != 0 && i < h) {
				apply(2*i, tag[i]);
				apply(2*i+1, tag[i]);
				tag[i] = 0;
			}
		}
		void update(int i) {
			seg[i] = combine(seg[2*i], seg[2*i+1]);
		}
		void recAdd(int a, int b, ll v, int i, int ia, int ib) {
			if (b < ia || ib < a) return;
			else if (a <= ia && ib <= b) apply(i, v);
			else {
				push(i);
				int im = (ia + ib) >> 1;
				recAdd(a, b, v, 2*i, ia, im);
				recAdd(a, b, v, 2*i+1, im+1, ib);
				update(i);
			}
		}
		pair<ll, ll> recMin(int a, int b, int i, int ia, int ib) {
			if (b < ia || ib < a) return {INF, 0};
			else if (a <= ia && ib <= b) return seg[i];
			else {
				push(i);
				int im = (ia + ib) >> 1;
				auto le = recMin(a, b, 2*i, ia, im);
				auto ri = recMin(a, b, 2*i+1, im+1, ib);
				return combine(le, ri);
			}
		}
	public:
		ZeroTree(const vector<ll>& val, const vector<ll>& weight) {
			int n = weight.size();
			while(h < n) h <<= 1;
			tag.resize(2*h, 0);
			seg.resize(2*h, {INF, 0});
			for (int i = 0; i < n; ++i) seg[i+h] = {val[i], weight[i]};
			for (int i = h-1; i > 0; --i) update(i);
		}
		void rangeAdd(int a, int b, ll v) {
			recAdd(a, b, v, 1, 0, h-1);
		}
		pair<ll, ll> rangeMin(int a, int b) {
			return recMin(a, b, 1, 0, h-1);
		}
};

// Example main. Calculates total area covered by line segments
// Add 1 to every range covered by a line segment. Only positions with value 0 are not covered.
// Then just calculate the sum of weights over positions with value 0.
int main() {
	int n, q;
	cin >> n >> q;
	ZeroTree tree(vector<ll>(n, 0), vector<ll>(n, 1));
	for (int qi = 0; qi < q; ++qi) {
		int a, b;
		cin >> a >> b;
		tree.rangeAdd(a, b, 1);
	}
	auto res = tree.rangeMin(0, n-1);
	if (res.first > 0) cout << n << '\n';
	else cout << n - res.second << '\n';
}
