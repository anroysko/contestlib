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

// Euler tour tree with operations:
//	1. LINK(a, b):	Adds an edge between a and b. The root of as tree remains the root.
//	2. CUT(a, b):	Cut the edge between a and b
//	3. REROOT(x):	Sets x as the root of its subtree
//	4. ROOT(x):	Returns root of xs tree
//	5. ADD(x, v):	Increase value of every node in xs subtree
//	6. SUM(x):	Calculate sum in xs subtree

class EulerTourTree {
	private:
		// Code for treap
		struct Node {
			const int pri, ind;
			Node* ch[2] = {nullptr, nullptr};
			Node* p = nullptr;
			Node* nxt, pre;
			
			Node(int sign) : pri(rand<int>()), sgn(sign) {}

			void update() {}
			void apply() {}
			void push() {}

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

		// Adds y to linked list right after x
		Treap* linkList(Treap* x, Treap* y) {
			y->nxt = x->nxt;
			y->pre = x;
			y->nxt->pre = y;
			y->pre->nxt = y;
			return y;
		}
		void cutList(Treap* x) {
			x->pre->nxt = x->nxt;
			x->nxt->pre = x->pre;
		}

		// Data members
		const int n;
		vector<Node*> apps;

	public:
		// Makes j the parent of i
		void link(int i, int j) {
		}
		void cut(int i) {

		}
		void reroot(int i) {
			auto tmp = split(apps[i]);
			join(tmp.second, tmp.first);
		}
		int root(int i) {
			return 
		}
		vector<int> print(int i) {

		}
};

int main() {
	
}
