#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <chrono>
using namespace std;
using ll = long long;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

template<class T>
class EulerTourTree {
	private:
		struct Treap {
			const int pri;
			const bool real;
			Treap* ch[2] = {nullptr, nullptr};
			Treap* p = nullptr;
			Treap* pre;

			//////////////////
			// Change these //
			//////////////////

			ll val = 0; // Value
			ll sum = 0; // Subtree sum (in treap)
			ll tag = 0; // Lazy tag

			Treap(int sgn, Treap* pre) : pri(rand<int>()), val(real) {
				if () {
					nxt = pre->nxt;
					pre->nxt = this;
				} else nxt = this;
			}

			ll getVal() { return val; }
			ll getSum() { return sum; }

			void add(ll v) { val += v; }
			void apply(ll v) {
				add(v);
				tag += v;
			}

			void update() {
				sum = (real ? val : 0) + (ch[0] ? ch[0]->val : 0) + (ch[1] ? ch[1]->val : 0);
			}
			void push() {
				if (ch[0]) ch[0]->apply(tag);
				if (ch[1]) ch[1]->apply(tag);
				tag = 0;
			}

			/////////////////////////
			// DO NOT change these //
			/////////////////////////

			void setChild(Treap* c, bool d) {
				if (ch[d]) ch[d]->p = nullptr;
				if (c) c->p = this;
				ch[d] = c;
				update();
			}

			static void pushPath(Treap* x) {
				if (x->p) pushPath(x->p);
				x->push();
			}

			// Returns root of the node's treap
			static Treap* root(Treap* x) {
				while(x->p) x = x->p;
				return x;
			}

			// Finds outmost node in the specified direction in the node's subtree in the treap
			static Treap* outmost(Treap* x, bool d) {
				while(x->ch[d]) x = x->ch[d];
				return x;
			}

			// Finds next node in the specified direction in the treap
			static Treap* advance(Treap* x, bool d) {
				if (x->ch[d]) return outmost(x->ch[d], d^1);
				for (;; x = x->p) {
					if (! x->p || x->p->ch[d^1] == x) return x->p;
				}
			}

			// Joins the two treaps, with nodes in a to the left of nodes in b
			static Treap* join(Treap* a, Treap* b) {
				if (!a || !b) return a ? a : b;
				Treap* res = (a->pri > b->pri) ? a : b;
				res->push();

				if (res == a) a->setChild(join(a->ch[1], b), 1);
				else b->setChild(join(a, b->ch[0]), 0);
				return res;
			}

			// Splits xs tree into nodes left of x and nodes right of x
			static pair<Treap*, Treap*> split(Treap* x, bool d = 1) {
				pushPath(x);
				Treap* res[2] = (d ? {x, x->ch[d]} : {x->ch[d], x});
				x->setChild(nullptr, d);

				for (; x->p; x = x->p) {
					bool b = (x == x->p->ch[1]);
					x->p->setChild(res[b^1], b);
					res[b^1] = x->p;
				}
				return {res[0], res[1]};
			}
		};

		vector<Treap*> nodes;
	public:
		// Sets the parent of i to j. i must not have a parent.
		void link(int i, int j) {
			auto split = Treap::split(nodes[i], 1);
			split.first = Treap::join(split.first, Treap::root(nodes[j]));
			split.first = Treap::join(split.first, new Treap(0, nodes[i]));
			Treap::join(split.first, split.second);
		}
		// Cuts the edge from i to its parent. i must have a parent.
		void cut(int i) {
			Treap* le = Treap::split(nodes[i], 0).first;
			Treap* ri = Treap::split(nodes[i]->pre, 1).second;
			Treap* par = Treap::outmost(le, 1);
			Treap* rem = Treap::outmost(ri, 0);
			if (rem) {
				ri = Treap::split(rem, 1).second;
				par->pre = rem->pre;
				delete rem;
			}
			Treap::join(le, ri);
		}
		// Returns sum of values in is subtree, or is value if st = 0
		ll get(int i, bool st = 1) {
			Treap* le = Treap::split(nodes[i], 0).first;
			auto mr = Treap::split(nodes[i]->pre, 1).second;
			ll res = (st ? mr.first->getSum() : mr.first->getVal());
			Treap::join(le, Treap::join(mr.first, mr.second));
			return res;
		}
		// Adds v to all values in is subtree, or to only i if st = 0
		void apply(int i, ll v, bool st = 1) {
			Treap* le = Treap::split(nodes[i], 0).first;
			auto mr = Treap::split(nodes[i]->pre, 1).second;
			if (st) mr.first->apply(v);
			else mr.first->add(v);
			Treap::join(le, Treap::join(mr.first, mr.second));
			return res;
		}
};

int main() {
	
}
