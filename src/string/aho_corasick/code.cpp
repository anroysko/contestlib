// Aho-corasick automaton for counting occurrences of pattern strings
class AhoCora {
	private:
		vector<map<char, int>> conns; // Edges to children from this node
		vector<int> suf_link; // Longest string in automata that is this one's true suffix
		vector<ll> weight; // Total weight of appearances of string represented by this node
		vector<int> pats; // Index of node corresponding to pattern
		int act = 0; // Node representing active string

		int makeNode() {
			conns.emplace_back();
			suf_link.push_back(-1);
			weight.push_back(0);
			return (int)weight.size() - 1;
		}

		// Get target of edge from node i, labeled with char c. -1 if none.
		int get(int i, char c) {
			auto it = conns[i].find(c);
			if (it == conns[i].end()) return -1;
			else return (*it).second;
		}

		// Set target of edge from node i, labeled with char c
		void set(int i, char c, int t) {
			conns[i][c] = t;
		}

		// Extends the suffix by the given character
		int extend(int i, char c) {
			int res = -1;
			while((i != -1) && (res == -1)) {
				res = get(i, c);
				i = suf_link[i];
			}
			return max(res, 0); // root always works
		}

	public:
		AhoCora() {
			makeNode(); // make root
		}

		// Adds the string to the automata, returns its identifier
		// Time complexity: O(|pat| log C)
		int addPattern(const string& pat) {
			int i = 0; // current node
			for (char c : pat) {
				if (get(i, c) == -1) set(i, c, makeNode());
				i = get(i, c);
			}
			pats.push_back(i);
			return (int)pats.size() - 1;
		}

		// Build suf_link and pat_link. Call after inserting all patterns, before matching
		// Time complexity: O(\sum |pat| log C)
		void buildLinks() {
			vector<int> que = {0};
			for (int j = 0; j < que.size(); ++j) {
				for (auto pr : conns[que[j]]) {
					suf_link[pr.second] = extend(suf_link[que[j]], pr.first);
					que.push_back(pr.second);
				}
			}
		}

		// Extend the current string with character c, weight matches of patterns ending here with w
		// Time complexity: O(log C)
		void push(char c, ll w) {
			act = extend(act, c);
			weight[act] += w;
		}

		// Clear current string
		void clear() { act = 0; }

		// Get weighted appearances of each pattern.
		// Time complexity: O(\sum |pat|)
		vector<ll> getCounts() {
			vector<int> que = {0};
			for (int j = 0; j < que.size(); ++j) {
				for (auto pr : conns[que[j]]) que.push_back(pr.second);
			}
			for (int j = (int)que.size() - 1; j > 0; --j) {
				weight[suf_link[que[j]]] += weight[que[j]];
			}
			vector<ll> res(pats.size());
			for (int i = 0; i < pats.size(); ++i) res[i] = weight[pats[i]];
			return res;
		}
};

// Aho-corasick automaton for counting occurrences of pattern strings
class AhoCora {
	private:
		const int C = 26; // Alphabet size
		const int A = 'a'; // First character
		vector<vector<int>> conns; // Edges to children from this node
		vector<int> suf_link; // Longest string in automata that is this one's true suffix
		vector<ll> weight; // Total weight of appearances of string represented by this node
		vector<int> pats; // Index of node corresponding to pattern
		int act = 0; // Node representing active string

		int makeNode() {
			conns.emplace_back(C, -1);
			suf_link.push_back(-1);
			weight.push_back(0);
			return (int)weight.size() - 1;
		}

		// Extends the suffix by the given character
		int extend(int i, int c) {
			for (; i != -1; i = suf_link[i]) {
				if (conns[i][c] != -1) return conns[i][c];
			}
			return 0; // root always works
		}

	public:
		AhoCora() {
			makeNode(); // make root
		}

		// Adds the string to the automata, returns its identifier
		// Time complexity: O(|pat| C)
		int addPattern(const string& pat) {
			int i = 0; // current node
			for (char cc : pat) {
				int c = cc - A;
				if (conns[i][c] == -1) conns[i][c] = makeNode();
				i = conns[i][c];
			}
			pats.push_back(i);
			return (int)pats.size() - 1;
		}

		// Build suf_link and pat_link. Call after inserting all patterns, before matching
		// Time complexity: O(\sum |pat| C)
		void buildLinks() {
			vector<int> que = {0};
			for (int j = 0; j < que.size(); ++j) {
				int i = que[j];
				for (int c = 0; c < C; ++c) {
					int t = conns[i][c];
					if (t != -1) {
						suf_link[t] = extend(suf_link[i], c);
						que.push_back(t);
					}
				}
			}
		}

		// Extend the current string with character c, weight matches of patterns ending here with w
		// Time complexity: amortised O(1)
		void push(char c, ll w) {
			act = extend(act, c - A);
			weight[act] += w;
		}

		// Clear current string
		void clear() { act = 0; }

		// Get weighted appearances of each pattern.
		// Time complexity: O(\sum |pat| C)
		vector<ll> getCounts() {
			vector<int> que = {0};
			for (int j = 0; j < que.size(); ++j) {
				for (auto t : conns[que[j]]) {
					if (t != -1) que.push_back(t);
				}
			}
			for (int j = (int)que.size() - 1; j > 0; --j) {
				int i = que[j];
				weight[suf_link[i]] += weight[i];
			}
			vector<ll> res(pats.size());
			for (int i = 0; i < pats.size(); ++i) res[i] = weight[pats[i]];
			return res;
		}
};

// Aho-corasick automaton for counting occurrences of patterns in O(|res| + |pat| log C)
class AhoCora {
	private:
		vector<int> pat_ind; // Index of pattern at this node (-1 if none)
		vector<int> suf_link; // Longest string in automata that is this one's true suffix
		vector<int> pat_link; // Index of next pattern when following suffix links
		vector<map<char, int>> conns; // Edges to children from this node
		vector<ll> weight; // Total weight of appearances of string represented by this node

		// Alloc a new node
		int makeNode() {
			pat_ind.push_back(-1);
			suf_link.push_back(-1);
			pat_link.push_back(-1);
			conns.emplace_back();
			val.push_back(0);
			return (int)pat_ind.size() - 1;
		}

		// Get target of edge from node i, labeled with char c. -1 if none.
		int get(int i, char c) {
			auto it = conns[i].find(c);
			if (it == conns[i].end()) return -1;
			else return (*it).second;
		}

		// Set target of edge from node i, labeled with char c
		void set(int i, char c, int t) {
			conns[i][c] = t;
		}

		// Extends the suffix by the given character
		int extend(int i, char c) {
			int res = -1;
			while((i != -1) && (res == -1)) {
				res = get(i, c);
				i = suf_link[i];
			}
			return max(res, 0); // root always works
		}

	public:
		AhoCora() {
			makeNode(); // make root
		}

		// Adds the string to the automata
		// Time complexity: O(|pat| log C)
		void addPattern(const string& pat, int ind) {
			int i = 0; // current node
			for (char c : pat) {
				if (get(i, c) == -1) set(i, c, makeNode());
				i = get(i, c);
			}
			pat_ind[i] = ind;
		}

		// Build suf_link and pat_link. Call after inserting all patterns, before matching
		// Time complexity: O(\sum |pat| log C)
		void buildLinks() {
			vector<int> que;
			que.push_back(0);
			for (int j = 0; j < que.size(); ++j) {
				int i = que[j];
				for (auto pr : conns[i]) {
					char c = pr.first;
					int t = pr.second;
					suf_link[t] = extend(suf_link[i], c);
					if (pat_ind[suf_link[t]] != -1) pat_link[t] = suf_link[t];
					else pat_link[t] = pat_link[suf_link[t]];
					que.push_back(t);
				}
			}
		}

		// Returns vector containing all matches {index, pattern},
		// sorted increasingly by index, then decreasingly by length of pattern.
		// Time complexity: O(|res| + |str| log C)
		vector<pair<int, int>> match(const string& str) {
			vector<pair<int, int>> res;
			int i = 0; // current node
			for (int j = 0; j < str.size(); ++j) {
				i = extend(i, str[j]);
				int w = i;
				while(w != -1) {
					if (pat_ind[w] != -1) res.push_back({j, pat_ind[w]});
					w = pat_link[w];
				}
			}
			return res;
		}
};
