#include <iostream>
#include <random>
#include <chrono>
#include <tuple>
using namespace std;
using ll = long long;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

// Euler tour tree with the operations:
//	1. LINK(p, x): Add x as p's child (x is guaranteed to have no parent)
//	2. CUT(x): Cut the edge from x to its parent (Which is guaranteed to exist)
//	3. DEPTH(x): Depth of node x
//	4. SIZE(x): Size of xs subtree
class EulerTourTree {
	private:
		// Code for treap
		struct Node {
			const int pri, val;
			Node* ch[2] = {nullptr, nullptr};
			Node* p = nullptr;
			Node* nxt = nullptr;

			int depth = 1;
			int dep_add = 0;
			int size;
			
			Node(bool real) : pri(rand<int>()), val(real) { size = val; }

			void update() {
				size = val + (ch[0] ? ch[0]->size : 0) + (ch[1] ? ch[1]->size : 0);
			}
			void apply(int d) {
				depth += d;
				dep_add += d;
			}
			void push() {
				if (ch[0]) ch[0]->apply(dep_add);
				if (ch[1]) ch[1]->apply(dep_add);
				dep_add = 0;
			}
			void setChild(Node* c, bool d) {
				if (ch[d]) ch[d]->p = nullptr;
				if (c) c->p = this;
				ch[d] = c;
				update();
			}
		};

		void pushPath(Node* x) {
			if (x->p) pushPath(x->p);
			x->push();
		}
		void recDel(Node* x) {
			if (x->nxt) recDel(x->nxt);
			delete x;
		}

		// Returns root of the node's treap
		Node* root(Node* x) {
			while(x->p) x = x->p;
			return x;
		}

		// Finds outmost node in the specified direction in the node's subtree in the treap
		Node* out(Node* x, bool d) {
			while(x->ch[d]) x = x->ch[d];
			return x;
		}

		// Finds next node in the specified direction in the treap
		Node* adv(Node* x, bool d) {
			if (x->ch[d]) return out(x->ch[d], d^1);
			for (;; x = x->p) {
				if (! x->p || x->p->ch[d^1] == x) return x->p;
			}
		}

		// Finds last appearance of node, given its first appearance
		Node* getLast(Node* x) {
			Node* pre = adv(x, 0);
			if (! pre) return out(root(x), 1);
			else return adv(pre->nxt, 0);
		}

		// Joins the two treaps, with nodes in a to the left of nodes in b
		Node* join(Node* a, Node* b) {
			if (!a || !b) return a ? a : b;
			Node* res = (a->pri > b->pri) ? a : b;
			res->push();
			if (res == a) a->setChild(join(a->ch[1], b), 1);
			else b->setChild(join(a, b->ch[0]), 0);
			return res;
		}

		// Left subtree will contain x and everything before it
		pair<Node*, Node*> split(Node* x) {
			pushPath(x);
			Node* res[2] = {x, x->ch[1]};
			if (x->ch[1]) x->setChild(nullptr, 1);
			for (; x->p; x = x->p) {
				bool b = (x == x->p->ch[1]);
				x->p->setChild(res[b^1], b);
				res[b^1] = x->p;
			}
			return {res[0], res[1]};
		}

		int cou(Node* x) {
			int res = x->size - (x->ch[1] ? x->ch[1]->size : 0);
			for (; x->p; x = x->p) {
				if (x->p->ch[1] == x) res += (x->p->size) - (x->size);
			}
			return res;
		}

		vector<Node*> fst;

	public:
		// Makes j the parent of i
		void link(int i, int j) {
			auto parts = split(fst[i]);
			Node* mid = root(fst[j]);
			mid->apply(fst[i]->depth); // update depths

			Node* add = new Node(false); // add dummy node
			add->depth = fst[i]->depth;
			add->nxt = fst[i]->nxt;
			fst[i]->nxt = add;

			join(join(parts.first, mid), join(add, parts.second));
		}

		// Cuts the parent of i. THE PARENT MUST EXIST!
		void cut(int i) {
			Node* le = adv(fst[i], 0);
			Node* ri = le->nxt;
			le->nxt = ri->nxt;

			Node* pref = split(le).first;
			Node* mid = split(adv(ri, 0)).first;
			Node* suff = split(ri).second;

			root(mid)->apply(-le->depth); // Update depths
			join(pref, suff);
			delete ri;
		}

		// Gets depth of i
		int depth(int i) {
			pushPath(fst[i]);
			return fst[i]->depth;
		}

		// Gets subtree size of i
		int size(int i) {
			Node* lst = getLast(fst[i]);
			return cou(lst) - cou(fst[i]) + 1;
		}

		EulerTourTree(int n) : fst(n) {
			for (int i = 0; i < n; ++i) fst[i] = new Node(true);
		}
		~EulerTourTree() {
			for (auto x : fst) recDel(x);
		}
};

int main() {
	
}
