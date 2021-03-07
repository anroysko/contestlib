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


// Euler tour tree operations:
// 	1. ROOT(x): Report root of x's tree
//	2. LINK(p, x): Add x as p's child (x is guaranteed to have no parent)
//	3. CUT(x): Cut the edge from x to its parent (Which is guaranteed to exist)
//	4. REROOT(x): Reroot x's tree at x
//	5. SUBTREESUM(x): Find sum of nodes in x's subtree (Important special case: size of subtree)
//	6. PATHSUM(x): Return sum of path from x to root
//	7. LCA(x, y): Return lca of x and y (They are guaranteed to be in the same tree)

// Possible to support all simultaneously in O(log^2 n)
// The issue is maintaining first and last appearances.
// We know their cyclic order, so we can binary search for the first appearance

// Option 1:
// 	O(log n):
//		ROOT(x), LINK(p, x), REROOT(x), LCA(x, y), PATHSUM(x)
//	 O(log n log deg(x)):
//	 	CUT(x), SUBTREESUM(x)

// How to maintain adjacency lists?
// LINK and CUT operations might remove and push to the middle of these lists
// Thankfully the log deg(x) overhead doesn't matter in any operation, as deg(x) < n

// This gives an idea: What if we enforce that every node has degree at most 3,
// and add dummy nodes to facilitate this? If we would connect X and Y, and X already has 3 edges, take X's first child
// and insert a dummy node in its place, with both that child and Y as its children.

// If X has 

template<class Node>
struct Treap {
	static void pushPath(Treap* x) {
		if (x->p) pushPath(x->p);
		x->push();
	}
	static Node* join(Node* a, Node* b) {
		if (! a || ! b) return (a ? a : b);
		Node* res = ((a->pri < b->pri) ? a : b);

		res->push();
		if (res == a) a->setChild(1, join(a->ch[1], b));
		else b->setChild(0, join(a, b->ch[0]));
		return res;
	}


};

class EulerTree {
	private:
		// Code for treap
		struct Node {
			const int pri;
			Treap* ch[2] = {nullptr, nullptr}
			Treap* p = nullptr;
			
			Node() : pri(rand<int>()) {}
			void update() {}
			void push() {}

			void setChild(bool d, Node* c) {
				ch[d] = c;
				if (c) c->p = this;
				update();
			}
		}
		vector<Node> nodes;
	public:
		// Reroot i's component at i
		void reroot(int i) {
			
		}
	
	// Reroot subtree of i at i
	void reroot(int i) {

	}
	Treap* component(int i) {
		Treap* res = &nodes[i];
		while(res->p) res = res->p;
		return res;
	}
	Treap* subtree(int i) {
		
	}
	bool link(int a, int b) {
		reroot(b);
		
	}
	bool cut(int a, int b) {
		Treap *le1, *ri1, *le2, *ri2;
		tie(le1, ri1) = split(fi);
		tie(le2, ri2) = split(se);
		if (le1 == le2 || le1 == ri2) join(le2, ri1);
		else join(le1, ri2);
		delete fi; delete se;
	}
}






// No example usage since unaugmented this doesn't really do anything
int main() {
	
}
