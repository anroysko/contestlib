#include <iostream>
using namespace std;

class PQTree {
	private:
		struct Node {
			int par = -1;
			char type = 'L';
			vector<int> kids;
		};
		vector<Node> nodes;
	public:
		PQTree(int n) : nodes(n+1) {
			for (int i = 0; i < n; ++i) nodes[i].par = n;
			for (int i = 0; i < n; ++i) nodes[n].kids.push_back(i);
			nodes[n].type = 'P';
		}
		// Adds requirement that indexes in req must be consequent
		bool addReq(const vector<int>& req) {
			// Call a node active if some of the leaves under it are in the requirement,
			// and full if all the children below it appear there.
			// 1. Find the topmost node with multiple active children
			//	a. If that node is a P-node:
			//		If more than two of its children are active but not full, fail.
			//		Otherwise, remove all active children, and add a new Q-type child node,
			//		With a P-node of the full children in the middle, and recursively split
			//		Active but not full nodes at the side
			//		If two of the subtrees with leaves, remove all of its children with leaves.
			//	b. If that node is a Q-node:
			//		If the active nodes are not consequent, fail. If the nodes in the middle are
			//		not full, fail. Otherwise, make a new Q-node for the active nodes similarly as to (a)
			// 2. If necessary, recurse under the corner nodes to split active and inactive ones.
			//	
		}
		// Returns any permutation satisfying all added requirements
		vector<int> getPerm() {

		}
};

int main() {
	
}
