#include <iostream>
#include <vector>
using namespace std;

struct HLD {
	vector<int> par; // Parent
	vector<int> pp; // Path parent
	vector<int> ind; // HLD index

	// p: parent of node i. Must have p[i] < i (p[0] = -1).
	HLD(const vector<int>& p) : par(p), pp(p.size()), ind(p.size(), -1) {
		int n = p.size();
		vector<int> siz(n, 1); // subtree size
		for (int i = n-1; i > 0; --i) siz[par[i]] += siz[i];

		vector<int> pc(n, -1); // Preferred child
		for (int i = n-1; i > 0; --i) {
			if (2*siz[i] >= siz[par[i]]) pc[par[i]] = i;
		}

		int cur = 0; // Current position in array
		for (int i = 0; i < n; ++i) {
			if (ind[i] != -1) continue;
			for (int j = i; j != -1; j = pc[j], ++cur) {
				ind[i] = cur;
				pp[j] = i;
			}
		}
	}
	// Get intervals corresponding to path between a and b
	vector<pair<int, int>> get(int a, int b) {
		vector<pair<int, int>> res;
		while(true) {
			if (ind[b] < ind[a]) swap(a, b);
			if (ind[pp[b]] <= ind[a]) {
				res.push_back({ind[a], ind[b]});
				return res;
			} else {
				res.push_back({ind[pp[b]], ind[b]});
				b = par[pp[b]];
			}
		}
	}
};

// Example usage. Answers LCA-queries
int main() {
	int n, q;
	cin >> n >> q;

	vector<int> p(n);
	for (int i = 0; i < n; ++i) cin >> p[i];

	HLD hld(p);

	vector<int> rev(n);
	for (int i = 0; i < n; ++i) rev[hld.ind[i]] = i;

	for (int j = 0; j < q; ++j) {
		int a, b;
		cin >> a >> b;

		int res = rev[hld.get(a, b).back().first];
		cout << res << '\n';
	}
}
