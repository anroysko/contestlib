mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

struct MeldHeap {
	const int ind;
	ll val, add = 0;
	MeldHeap* ch[2] = {nullptr, nullptr}

	MeldHeap(int i, ll v) : ind(i), val(v) {}
	~MeldHeap() { delete ch[0]; delete ch[1]; }

	static void push(MeldHeap* p) {
		add(p->ch[0], p->add);
		add(p->ch[1], p->add);
		p->add = 0;
	}

	// Increment all values in heap p by v
	static void add(MeldHeap* p, ll v) {
		if (! p) return;
		p->val += v;
		p->add += v;
	}
	// Merge heaps a and b
	static MeldHeap* meld(MeldHeap* a, MeldHeap* b) {
		if (! a) return b;
		if (! b) return a;
		if (a->val > b->val) swap(a, b);

		push(a);
		int x = rand(0, 1);
		a->ch[x] = meld(a->ch[x], b);
		return a;
	}
	// Pop minimum element in heap p
	pair<int, ll> pop(MeldHeap*& p) {
		pair<int, ll> res = {p->ind, p->val};
		MeldHeap* tmp = p;
		p = meld(p->ch[0], p->ch[1]);
		delete tmp;
		return res;
	}
};
