#include <iostream>
#include <random>
#include <chrono>
#include <tuple>
using namespace std;
typedef long long ll;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

struct Treap {
	const int pri;
	Treap* le = nullptr;
	Treap* ri = nullptr;
	int siz = 1;

	Treap() : pri(rand<int>()) {}
	~Treap() {
		delete le;
		delete ri;
	}

	void update() {
		siz = 1 + (le ? le->siz : 0) + (ri ? ri->siz : 0);
	}
	void push() {} // Lazy update

	static Treap* join(Treap* a, Treap* b) {
		if (! a || ! b) return (a ? a : b);
		Treap* res = ((a->pri < b->pri) ? a : b);

		res->push();
		if (res == a) a->ri = join(a->ri, b);
		else          b->le = join(a, b->le);
		res->update();
		return res;
	}

	static pair<Treap*, Treap*> split(int as, Treap* x) {
		if (! x) return {nullptr, nullptr};
		int ls = 1 + (x->le ? x->le->siz : 0);
		Treap* oth;
		
		x->push();
		if (ls > as) {
			tie(oth, x->le) = split(as, x->le);
			x->update();
			return {oth, x};
		} else {
			tie(x->ri, oth) = split(as-ls, x->ri);
			x->update();
			return {x, oth};
		}
	}
};

// No example usage since unaugmented this doesn't really do anything
int main() {
	
}
