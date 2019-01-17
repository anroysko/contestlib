#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

// Link-Cut tree
// https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-854j-advanced-algorithms-fall-2008/lecture-notes/lec6.pdf
// https://courses.csail.mit.edu/6.851/spring14/scribe/L19.pdf
template<class T>
class LinkCutTree {
	struct SplayNode {
		SplayNode* p = nullptr; // Parent
		SplayNode* path_p = nullptr; // Path parent
		SplayNode* ch[2] = {nullptr, nullptr}; // Children
		bool flip = false; // Should this node's subtree be flipped
		T data; // User-defined data. Includes ind.

		SplayNode(const T& d) : data(d) {}
		~SplayNode() {} // Intentionally doesn't delete anything

		void push() {
			if (flip) {
				swap(ch[0], ch[1]);
				if (ch[0]) ch[0]->flip ^= 1;
				if (ch[1]) ch[1]->flip ^= 1;
				flip = false;
				data.flip();
			}
			if (ch[0]) ch[0]->path_p = path_p;
			if (ch[1]) ch[1]->path_p = path_p;
			T* left_data = (ch[0] ? &(ch[0]->data) : nullptr);
			T* right_data = (ch[1] ? &(ch[1]->data) : nullptr);
			data.push(left_data, right_data);
		}
		void update() {
			const T* left_data;
			const T* right_data;
			if (ch[0]) {
				ch[0]->push();
				left_data = &(ch[0]->data);
			} else left_data = nullptr;
			if (ch[1]) {
				ch[1]->push();
				right_data = &(ch[1]->data);
			} else right_data = nullptr;
			data.update(left_data, right_data);
		}
	};
	vector<SplayNode*> nodes;

	// Checks whether na is its parent's right child
	// Assumes that
	// 	1. na is not null
	// 	2. na->p is not null
	// 	3. na is a child of its parent (should be an invariant)
	static bool isRightChild(SplayNode* na) {
		assert(na != nullptr);
		assert(na->p != nullptr);
		assert(na->p->ch[0] == na || na->p->ch[1] == na);

		return (na->p->ch[1] == na);
	}
	// Zigs or Zags on na. na->ch[d] is brought to na's place.
	// Assumes that
	// 	1. na is not null
	// 	2. na->ch[d] is not null
	// 	3. na and na->ch[d] are pushed (unenforceable!)
	static void rotate(SplayNode* na, bool d) {
		assert(na != nullptr);
		assert(na->ch[d] != nullptr);
		// na and na->ch[d] are pushed

		bool invd = d ^ 1;
		auto* nb = na->ch[d];
		auto* nc = nb->ch[invd];

		// Set edges from nb, na and nc to their parents
		if (na->p) na->p->ch[isRightChild(na)] = nb;
		nb->p = na->p;
		nb->ch[invd] = na;
		na->p = nb;
		na->ch[d] = nc;
		if (nc) nc->p = na;

		na->update();
		nb->update();
	}
	// Splays on na
	// Assumes that
	// 	1. na is not null
	static void splay(SplayNode* na) {
		assert(na != nullptr);

		while(na->p) {
			auto* nb = na->p;
			auto* nc = nb->p;
			if (nc) nc->push();
			nb->push();
			na->push();
			bool arc = isRightChild(na);
			if (nc) {
				bool brc = isRightChild(nb);
				if (arc == brc) {
					rotate(nc, brc);
					rotate(nb, arc);
				} else {
					rotate(nb, arc);
					rotate(nc, brc);
				}
			} else {
				rotate(nb, arc);
			}
		}
	}
	// Find first (leftmost) node under or at na.
	// Assumes that
	// 	1. na is not null
	static SplayNode* findFirst(SplayNode* na) {
		assert(na != nullptr);

		na->push();
		while(na->ch[0]) {
			na = na->ch[0];
			na->push();
		}
		splay(na);
		return na;
	}
	// Joins the two splay trees into one.
	// Assumes that
	// 	1. na is not null
	//	2. right child of na is null
	// 	3. na is the root of its splay tree
	static SplayNode* join(SplayNode* na, SplayNode* nb) {
		assert(na != nullptr);
		assert(na->ch[1] == nullptr);
		assert(na->p == nullptr);

		if (nb) {
			na->push();
			na->ch[1] = nb;
			nb->p = na;
			na->update();
		}
		return na;
	}
	// Splits na into two splay trees, the right one being na's right child.
	// Assumes that
	// 	1. na is not null
	// 	2. na is the root of its splay tree
	static SplayNode* split(SplayNode* na) {
		assert(na != nullptr);
		assert(na->p == nullptr);

		auto* nb = na->ch[1];
		if (nb) {
			na->push();
			na->ch[1] = nullptr;
			nb->p = nullptr;
			nb->path_p = na;
			na->update();
		}
		return nb;
	}
	// Standard link-cut access operation
	// Assumes that
	//	1. na is not null
	static void access(SplayNode* na) {
		assert(na != nullptr);

		auto* nc = na;
		SplayNode* nb = nullptr;
		while(nc) {
			splay(nc);
			split(nc);
			nb = join(nc, nb);
			nc = nb->path_p;
		}
		splay(na);
	}
	// Find root of na's represented tree
	// Assumes that
	// 	1. na is not null
	static SplayNode* findRoot(SplayNode* na) {
		assert(na != nullptr);
		
		access(na);
		return findFirst(na);
	}
	// Reroots na's represented tree at na.
	// Assumes that
	// 	1. na is not null
	static void reroot(SplayNode* na) {
		assert(na != nullptr);

		access(na);
		na->flip ^= 1;
	}

	public:
	LinkCutTree(const vector<T>& ini) : nodes(ini.size()) {
		for (int i = 0; i < nodes.size(); ++i) nodes[i] = new SplayNode(ini[i]);
	}
	~LinkCutTree() {
		for (int i = 0; i < nodes.size(); ++i) delete nodes[i];
	}
	// Reroots the represented tree of the given node at the given node
	// Assumes that
	// 	1. 0 <= i < n
	void reroot(int i) {
		assert(0 <= i && i < nodes.size());

		reroot(nodes[i]);
	}
	// Sets active path to the path from the given node to the root.
	// Returns data of root
	// Assumes that
	// 	1. 0 <= i < n
	T& findRoot(int i) {
		assert(0 <= i && i < nodes.size());

		auto* root = findRoot(nodes[i]);
		root->push();
		return root->data;
	}
	// Sets active path to the path from the given node to the root.
	// Returns data of given node
	// Assumes that
	// 	1. 0 <= i < n
	T& access(int i) {
		assert(0 <= i && i < nodes.size());

		access(nodes[i]);
		nodes[i]->push();
		return nodes[i]->data;
	}
	// Adds an edge between nodes a and b
	// If a and b are in the same component, nothing changes and returns false.
	// Otherwise, the edge a-b is added, and the root of the combined component is set to root of b.
	// Assumes that
	// 	1. 0 <= a, b < n
	bool link(int a, int b) {
		assert(0 <= a && a < nodes.size());
		assert(0 <= b && b < nodes.size());

		auto* na = nodes[a];
		auto* nb = nodes[b];
		auto* a_root = findRoot(na);
		auto* b_root = findRoot(nb);
		if (a_root == b_root) return false;

		reroot(na);
		splay(nb);
		na->path_p = nb;
		return true;
	}
	// Cuts the edge between nodes a and b
	// If no such edge exists, returns false.
	// Otherwise, the edge a-b is cut, the side with the original root keeps its root,
	// and the root of the other side is set to a or b, whichever is on that side.
	// Assumes that
	// 	1. 0 <= a, b < n
	bool cut(int a, int b) {
		assert(0 <= a && a < nodes.size());
		assert(0 <= b && b < nodes.size());

		auto* na = nodes[a];
		auto* nb = nodes[b];

		access(na);
		splay(nb);
		splay(na);		
		if (na->ch[0] == nb) {
			access(nb);
			na->path_p = nullptr;
			return true;
		}

		access(nb);
		splay(na);
		splay(nb);
		if (nb->ch[0] == na) {
			access(na);
			nb->path_p = nullptr;
			return true;
		}
		return false;
	}
};

struct IndData {
	const int ind;
	IndData(int i) : ind(i) {}

	void update(const IndData*, const IndData*) const {}
	void push(const IndData*, const IndData*) const {}
	void flip() const {}
};

// Example usage
// Solves the tree dynamic connectivity problem
int main() {
	int n, q;
	cin >> n >> q;

	vector<IndData> ini_data;
	for (int i = 0; i < n; ++i) ini_data.emplace_back(i);
	LinkCutTree<IndData> lctree(ini_data);

	for (int ti = 0; ti < q; ++ti) {
		char op;
		int a, b;
		cin >> op >> a >> b;
		--a; --b;
		if (op == 'l') {

			bool fail = ! lctree.link(a, b);
			if (fail) cout << "Already in same component\n";
		} else if (op == 'c') {

			bool fail = ! lctree.cut(a, b);
			if (fail) cout << "Edge doesn't exist\n";
		} else {

			int a_root = lctree.findRoot(a).ind;
			int b_root = lctree.findRoot(b).ind;
			if (a_root == b_root) {
				cout << "Connected\n";
			} else {
				cout << "Not connected\n";
			}
		}
	}
	cout << '\n';
}
