

struct FlowGraph {
	struct Edge {
		ll cap; // Capacity
		ll base; // Edge base cost
		ll flow; // Flow on edge
		int a, b; // directed from a to b

		// Cost to push f flow through this edge. Must be convex.
		ll cost(ll f) { return f * base; }
	};
	int source, sink;
	vector<Edge> edges;
	vector<vector<int>> adj;
};

// Given a weighted directed graph G, finds potentials p_i s.t. c_ab + p_a - p_b >= 0 for all edges (a, b),
// Sets the weights of edges in the flowgraph so that they are not negative, and returns a vector containing the potentials.
// If a negative weight cycle was found, returns an empty vector and no changes to the flowgraph are made.
vector<ll> labelCorr(FlowGraph& g) {
	int n = g.adj.size();
	vector<ll> p(n, 0);
	priority_queue<
}

struct NegPot {
	vector<ll> pot;

	NegPot(
};
