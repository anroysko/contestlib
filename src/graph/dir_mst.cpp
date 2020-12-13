#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

// Meldable Heap
struct MH {
	const int ind;
	ll val, inc = 0;
	MH* ch[2] = {nullptr, nullptr};

	MH(int i, ll v) : ind(i), val(v) {}
	~MH() { delete ch[0]; delete ch[1]; }

	static void push(MH* p) {
		add(p->ch[0], p->inc);
		add(p->ch[1], p->inc);
		p->inc = 0;
	}

	// Increment all values in heap p by v
	static void add(MH* p, ll v) {
		if (! p) return;
		p->val += v;
		p->inc += v;
	}
	// Merge heaps a and b
	static MH* meld(MH* a, MH* b) {
		if (! a) return b;
		if (! b) return a;
		if (a->val > b->val) swap(a, b);

		push(a);
		int x = rand(0, 1);
		a->ch[x] = meld(a->ch[x], b);
		return a;
	}
	// Pop minimum element in heap p
	static pair<int, ll> pop(MH*& p) {
		pair<int, ll> res = {p->ind, p->val};
		MH* tmp = p;
		p = meld(p->ch[0], p->ch[1]);
		tmp->ch[0] = tmp->ch[1] = nullptr;
		delete tmp;
		return res;
	}
};

struct Edge {
	int s, t;
	ll w;
	Edge(int src, int tar, ll weight) : s(src), t(tar), w(weight) {}
};

class DirMst {
	private:
		const vector<Edge> ed;
		vector<int> par, cmp, pick;
		vector<MH*> ins;
		int n, x = 0; // x: remaining vertex after contractions
		
		void setp(int i, int p) { par[i] = p; cmp[i] = p; }
		int getc(int i) {
			if (cmp[i] != i) cmp[i] = getc(cmp[i]);
			return cmp[i];
		}
		int adv(int i) {
			while(ins[i]) {
				int r = getc(ed[ins[i]->ind].s);
				if (r != i) return r;
				else MH::pop(ins[i]);
			}
			return -1;
		}
		void setDI(int i, int t) {
			for (; cmp[i] < t; i = par[i]) cmp[i] = t;
		}
	public:
		// INPUT GRAPH MUST BE FULLY CONNECTED!
		DirMst(int n_, const vector<Edge>& e) : n(n_), ed(e), par(2*n_), cmp(2*n_), pick(2*n_), ins(2*n_, nullptr) {
			for (int i = 0; i < 2*n; ++i) setp(i, i);
			for (int j = 0; j < ed.size(); ++j) ins[ed[j].t] = MH::meld(ins[ed[j].t], new MH(j, ed[j].w));
			for (int j = n; adv(x) != -1; ++j) {
				for (; pick[x] < j; x = adv(x)) pick[x] = j;
				for (; x != j; x = getc(ed[pick[x]].s)) {
					ll w;
					tie(pick[x], w) = MH::pop(ins[x]);
					MH::add(ins[x], -w);
					ins[j] = MH::meld(ins[j], ins[x]);
					setp(x, j);
				}
			}
			for (int& v : cmp) v = -1; // reuse for unpack
			if (ins[x]) delete ins[x];
		}

		// Returns the total cost of and the edges in the minimum directed spanning tree rooted at r
		pair<ll, vector<int>> unpack(int r) {
			setDI(r, cmp[x] + 1);
			pair<ll, vector<int>> res;
			for (int i = x; i >= 0; --i) {
				if (cmp[i] < cmp[x]) {
					res.second.push_back(pick[i]);
					res.first += ed[pick[i]].w;
					setDI(ed[pick[i]].t, cmp[x]);
				}
			}
			return res;
		}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, r;
	cin >> n >> m >> r;
	vector<Edge> edges;
	for (int i = 0; i < m; ++i) {
		int a, b;
		ll w;
		cin >> a >> b >> w;
		edges.emplace_back(a, b, w);
	}
	for (int i = 0; i < n; ++i) edges.emplace_back(i, r, 0); // make sure the graph is fully connected

	DirMst mst(n, edges);
	auto res = mst.unpack(r);

	cout << res.first << '\n';

	vector<int> ans(n);
	ans[r] = r;
	for (auto i : res.second) ans[edges[i].t] = edges[i].s;
	for (auto i : ans) cout << i << ' '; cout << '\n';
}
