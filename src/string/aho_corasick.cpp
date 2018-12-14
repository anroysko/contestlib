#include <iostream>
#include <vector>
#include <map>
#include <utility>
using namespace std;

// Aho-corasick automaton for matching a string against multiple patterns
struct AhoCora {
	vector<int> pat_ind; // Index of pattern at this node (-1 if none)
	vector<int> suf_link; // Suffix link to next one in automata
	vector<int> pat_link; // Index of next pattern when following suffix links
	vector<map<char, int>> conns; // Edges to children from this node

	// Alloc a new node
	int makeNode() {
		pat_ind.push_back(-1);
		suf_link.push_back(-1);
		pat_link.push_back(-1);
		conns.emplace_back();
		return (int)pat_ind.size() - 1;
	}

	// Get target of edge from node i, labeled with char c. -1 if none.
	int get(int i, char c) {
		auto it = conns[i].find(c);
		if (it == conns[i].end()) return -1;
		else return (*it).second;
	}
	// Set target of edge from node i, labeled with char c, and return it.
	int set(int i, char c, int t) {
		conns[i][c] = t;
		return t;
	}

        // #################### USE ONLY THE FUNCTIONS BELOW ####################
	
	AhoCora() {
		makeNode(); // make root
	}

	// Adds the string to the automata, and returns its index.
	// If the string already exists, returns the existing's index.
	// Time complexity: O(|pat| log C)
	int addPattern(const string& pat, int ind) {
		int i = 0; // current node
		for (char c : pat) {
			int ni = get(i, c);
			if (ni == -1) ni = set(i, c, makeNode());
			i = ni;
		}
		if (pat_ind[i] != -1) return pat_ind[i];
		pat_ind[i] = ind;
		return ind;
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

				// Find suf_link
				int w = suf_link[i]; // node of current suffix of i
				while((w != -1) && (suf_link[t] == -1)) {
					suf_link[t] = get(w, c);
					w = suf_link[w];
				}
				if (suf_link[t] == -1) suf_link[t] = 0;

				// Find pat_link
				if (pat_ind[suf_link[t]] != -1) pat_link[t] = suf_link[t];
				else pat_link[t] = pat_link[suf_link[t]];

				// Push to que
				que.push_back(t);
			}
		}
	}

	// Returns vector containing all matches {index, pattern},
	// sorted increasingly by index, then decreasingly by length of pattern.
	// Time complexity: O(|res| + |str| log C)
	vector<pair<int, int>> getMatches(const string& str) {
		vector<pair<int, int>> res;
		int i = 0; // current node
		for (int j = 0; j < str.size(); ++j) {
			// Move the state
			char c = str[j];
			int ni = -1;
			while((i != -1) && (ni == -1)) {
				ni = get(i, c);
				i = suf_link[i];
			}
			if (ni == -1) ni = 0;
			i = ni;

			// Find matches
			int w = i;
			while(w != -1) {
				if (pat_ind[w] != -1) res.push_back({j, pat_ind[w]});
				w = pat_link[w];
			}
		}
		return res;
	}
};

int main() {
	// Example usage:
	string str;
	cin >> str;

	AhoCora atm;

	int m;
	cin >> m;
	for (int i = 0; i < m; ++i) {
		string pat;
		cin >> pat;
		atm.addPattern(pat, i);
	}
	atm.buildLinks();

	auto matches = atm.getMatches(str);
	for (auto pr : matches) cout << pr.first << ' ' << pr.second << ", "; cout << '\n';

	// Example input:
	// aababbabaaba
	// 3 aba bab ab

	// Example output:
	// 2 2, 3 0, 4 1, 4 2, 7 1, 7 2, 8 0, 10 2, 11 0,
}
