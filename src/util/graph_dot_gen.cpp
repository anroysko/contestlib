#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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

int main() {
	vector<string> nm = readTokens();
	int n = stoi(nm[0]);
	int m = (nm.size() == 1 ? n-1 : stoi(nm[1]));
	
	ofstream fout;
	fout.open("graph.dot");
	fout << "Graph G {\n";
	for (int i = 0; i < n; ++i) {
		fout << "\t" << i+1 << ";\n";
	}
	for (int i = 0; i < m; ++i) {
		vector<string> ed = readTokens();
		fout << "\t" << ed[0] << " -- " << ed[1];
		if (ed.size() > 2) {
			fout << " [label=\"";
			for (int j = 2; j+1 < ed.size(); ++j) fout << ed[j] << ", ";
			fout << ed.back() << "\"]";
		}
		fout << ";\n";
	}
	fout << "}\n";
	fout.close();

	system("dot graph.dot -Tpng > graph.png");
	system("xgd-open graph.png");
}
