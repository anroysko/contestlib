#include <iostream>
using namespace std;
const int INF = (int)1e9 + 7;

struct PSeg {
	struct Node {
		int minv = 0; // Minimum value below
		int maxv = 0; // Maximum value below
		int setv = -1; // All values below this should be set to zero
		int le = -1; // Left child. -1 for none
		int ri = -1; // Right child. -1 for none
	};

	vector<Node> nodes;
	vector<int> roots = {0};
	int h = 1;

	int apply(int i, int v) {
		if (v == -1) nodes.emplace_back(nodes[i]);
		else nodes.push_back({v, v, v, nodes[i]->le, nodes[i]->ri});
		return (int)nodes.size() - 1;
	}

	PSeg(int n) {
		while(h < vals.size()) h <<= 1;
		nodes.resize(2*h-1);
		for (int i = 0; i < h-1; ++i) {
			nodes[i].le = 2*i + 1;
			nodes[i].ri = 2*i + 2;
		}
	}

	// Given value r and range [a, b], find [c, d] s.t. a <= c <= d <= b,
	// val[x] <= r for c <= x <= d, c is minimized, and d is maximized for the c.
	pair<int, int> recGetMin(int a, int b, int i, int ia, int ib) {
		if (
		if (b < ia || ib < a) return {INF, -1};
		else if (nodes[i].minv == nodes[i].maxv) return nodes[i].
		if (nodes[i].setv != -1) return 
	}
	pair<int, int> getMin(int t, int a, int b) {
		return recGetMin(a, b, roots[t], 0, h-1);
	}
};

int main() {
	
}
