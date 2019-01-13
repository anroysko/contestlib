#include <iostream>
using namespace std;

// Splay tree
// https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-854j-advanced-algorithms-fall-2008/lecture-notes/lec6.pdf
template<class T>
struct SplayTree {
	struct SplayNode {
		T v; // Value
		SplayNode* p = nullptr; // Parent
		SplayNode* ch[2] = {nullptr, nullptr}; // Children

		SplayNode(T val) : v(val) {}
		~SplayNode() {} // Intentionally doesn't delete children

		void update() {
			// write updating here, for example subtree sum
		}
	};

	static void setEdge(SplayNode* na, SplayNode* nb, bool d) {
		na->ch[d] = nb;
		if (nb) nb->p = na;
	}
	static void rotate(SplayNode* na, bool d) {
		bool dr = d ^ 1;
		auto* nb = na->ch[d];
		auto* nc = nb->ch[dr];
		auto* np = na->p;

		if (np) setEdge(np, nb, (np->ch[0] != na));
		setEdge(nb, na, dr);
		setEdge(na, nc, d);
		na->update();
		nb->update();
	}
	// Splay on node na
	// Amortized cost O(1 + log(s(root) / s(x))),
	// where s is sum of weights in nodes subtree.
	static SplayNode* splay(SplayNode* na) {
		while(na->p) {
			auto* nb = na->p;
			auto* nc = nb->p;
			bool arc = (nb->ch[0] != na);
			bool brc = (nc->ch[0] != nb);
			if (nc == nullptr) rotate(nb, arc);
			else if (arc == brc) {
				rotate(nc, brc);
				rotate(nb, arc);
			} else {
				rotate(nb, arc);
				rotate(nc, brc);
			}
		}
		return na;
	}
	static SplayNode* findLast(SplayNode* na) {
		if (na == nullptr) return nullptr;
		auto* nb = na->ch[1];
		while(nb) {
			na = nb;
			nb = na->ch[1];
		}
		return splay(na);
	}
	// Finds rightmost node with node->value <= v
	static SplayNode* lowerBound(T v, SplayNode* nb) {
		auto* na = nullptr;
		auto* res = nullptr;
		while(nb) {
			na = nb;
			bool leq = (na->v <= v);
			if (leq) res = na;
			nb = na->ch[leq];
		}
		if (na) splay(na);
		if (res) splay(res);
		return res;
	}
	static void split(T v, SplayNode* tree, SplayNode*& le, SplayNode*& ri) {
		le = lowerBound(v, tree);
		if (le == nullptr) ri = tree;
		else {
			ri = le->ch[1];
			ri->p = nullptr;
			le->ch[1] = nullptr;
			le->update();
		}
	}
	static SplayNode* join(SplayNode* le, SplayNode* ri) {
		if (le == nullptr) return ri;
		le = findLast(le);
		setEdge(le, ri, 1);
		le->update();
	}
	static SplayNode* add(T v, SplayNode* tree) {
		SplayNode* le, ri;
		split(v, tree, le, ri);
		SplayNode* root = new SplayNode(v);
		setEdge(root, le, 0);
		setEdge(root, ri, 1);
		root->update();
		return root;
	}
	static SplayNode* remove(SplayNode* node) {
		splay(node);
		auto* res = join(node->ch[0], node->ch[1]);
		delete node;
		return res;
	}
	static void recDelete(SplayNode* node) {
		if (node == nullptr) return;
		recDelete(node->ch[0]);
		recDelete(node->ch[1]);
		delete node;
	}
};

int main() {

}
