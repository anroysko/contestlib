#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<string> readTokens() {
	string row;
	getline(cin, row);
	stringstream ss;
	ss << row;

	vector<string> res;
	while(! ss.eof()) {
		string v;
		ss >> v;
		res.push_back(v);
	}
	return res;
}
string itos(int i) {
	stringstream ss;
	ss << i;
	string res;
	ss >> res;
	return res;
}

int main() {
	vector<string> nm = readTokens();
	int n = stoi(nm[0]);
	int m = (nm.size() == 1 ? n-1 : stoi(nm[1]));
	
	ofstream fout;
	fout.open("graph.dot");
	fout << "Graph G {\n";

	// Print nodes
	vector<vector<string>> labels(n);
	for (int i = 0; i < n; ++i) labels[i].push_back(itos(i+1));

	vector<string> ed = readTokens();
	while(ed.size() == n && n > 3) {
		for (int i = 0; i < n; ++i) labels[i].push_back(ed[i]);
		ed = readTokens();
	}
	for (int i = 0; i < n; ++i) {
		fout << "\t" << i+1 << " [label=\"";
		for (int j = 0; j+1 < labels[i].size(); ++j) fout << labels[i][j] << ", ";
		fout << labels[i].back() << "\"];";
	}

	// Print edges
	for (int i = 0; i < m; ++i) {
		fout << "\t" << ed[0] << " -- " << ed[1];
		if (ed.size() > 2) {
			fout << " [label=\"";
			for (int j = 2; j+1 < ed.size(); ++j) fout << ed[j] << ", ";
			fout << ed.back() << "\"]";
		}
		fout << ";\n";
		if (i+1 < m) ed = readTokens();
	}
	fout << "}\n";
	fout.close();

	// Generate and show
	system("dot graph.dot -Tpng > graph.png");
	system("xdg-open graph.png");
}
