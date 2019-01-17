#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

struct Node {
	Node* p = nullptr; // Parent (or path parent)
	Node* ch[2] = {nullptr, nullptr}; // Children
	bool flip = 0; // Should this node's subtree be flipped
	// Put other wanted data here

	void push() {
		if (flip) {
			swap(ch[0], ch[1]);
			if (ch[0]) ch[0]->flip ^= 1;
			if (ch[1]) ch[1]->flip ^= 1;
			flip = 0;
		}
	}

	void update() {
		// Do nothing by default
		// Note that while it is guaranteed that this node is pushed,
		// its children haven't necessarily been pushed.
	}
};

// Link-Cut tree
// https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-854j-advanced-algorithms-fall-2008/lecture-notes/lec6.pdf
// https://courses.csail.mit.edu/6.851/spring14/scribe/L19.pdf
template<class T>
class LinkCutTree {
	// Set a's child to be b, in direction d
	static void setChild(T* a, T* b, bool d) {
		a->ch[d] = b;
		if (b) b->p = a;
	}
	static bool isRoot(const T* a) {
		return a->p == nullptr || (a->p->ch[0] != a && a->p->ch[1] != a);
	}
	// is right child?
	static bool irc(const T* a) {
		return a->p->ch[1] == a;
	}

	// Zigs or zags na up one step
	// na and its parent should be pushed beforehand
	static void rotate(T* a) {
		bool dir = irc(a);
		Node* b = a->p;
		Node* c = a->ch[dir ^ 1];
		
		if (isRoot(b)) a->p = b->p;
		else setChild(b->p, a, irc(b));
		setChild(b, c, dir);
		setChild(a, b, dir ^ 1);

		b->update();
		a->update();
	}

	// Splays on na. Path from na to it's root should be pushed.
	static void splay(T* a) {
		while(! isRoot(a)) {
			if (! isRoot(a->p)) {
				if (irc(a) == irc(a->p)) rotate(a);
				else rotate(a->p);
			}
			rotate(a);
		}
	}
	// Pushes all nodes on path from na to root of its represented tree
	static void pushPath(T* a) {
		if (a->p) pushPath(a->p);
		a->push();
	}
	// link-cut access operation
	// DOESN'T splay on na in the end!
	static T* access(T* a) {
		T* b = a;
		T* c = nullptr;
		pushPath(b);
		while(b) {
			splay(b);
			b->ch[1] = c;
			b->update();
			c = b;
			b = b->p;
		}
		splay(a);
		return a;
	}
	// Reroots at a, and makes the path from a to b active
	static T* expose(T* a, T* b) {
		access(a);
		a->flip = 1;
		access(b);
		return b;
	}
	// Adds an edge between nodes a and b
	// returns false if a and b are in the same component
	static bool link(T* a, T* b) {
		expose(a, b);
		if (a->p) return false;
		else {
			setChild(b, a, 1);
			b->update();
			return true;
		}
	}

	// Cuts the edge between nodes a and b
	// Returns false if no such edge exists
	static bool cut(T* a, T* b) {
		expose(a, b);
		if (b->ch[0] != a || a->ch[1]) return false;
		else {
			a->p = nullptr;
			b->ch[0] = nullptr;
			b->update();
			return true;
		}
	}

	vector<T> nodes;
	public:
	template<class... Args>
	int emplace(Args&&... args) {
		nodes.emplace_back(forward<Args>(args)...);
		return (int)nodes.size() - 1;
	}
	T& expose(int ai, int bi) {
		return *expose(&nodes[ai], &nodes[bi]);
	}
	bool link(int ai, int bi) {
		if (ai == bi) return false;
		else return link(&nodes[ai], &nodes[bi]);
	}
	bool cut(int ai, int bi) {
		if (ai == bi) return false;
		else return cut(&nodes[ai], &nodes[bi]);
	}
};

// Example usage
// Solves the tree dynamic connectivity problem
int main() {
	int n, q;
	cin >> n >> q;

	LinkCutTree<Node> lctree;
	for (int i = 0; i < n; ++i) lctree.emplace();

	for (int ti = 0; ti < q; ++ti) {
		char op;
		int a, b;
		cin >> op >> a >> b;
		--a; --b;
		if (op == 'l') {
			bool fail = ! lctree.link(a, b);
			if (fail) cout << "already connected\n";
			else cout << "link added\n";
		} else if (op == 'c') {
			bool fail = ! lctree.cut(a, b);
			if (fail) cout << "no such link\n";
			else cout << "link cut\n";
		} else {
			bool conn = ! lctree.link(a, b);
			if (conn) {
				cout << "connected\n";
			} else {
				lctree.cut(a, b);
				cout << "not connected\n";
			}
		}
	}
}
