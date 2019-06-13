#include <iostream>
#include <sstream>
using namespace std;

int main() {
	// graph_dot_gen < graph.txt > graph.dot
	// dot graph.dot -Tpng > graph.png
	// xdg-open graph.png

	string first_row;
	getline(cin, first_row);
	stringstream ss;
	ss << first_row;
	
	int n, m;
	ss >> n;
	if (ss.eof()) m = n-1;
	else ss >> m;
	
	cerr << "Graph with n, m = " << n << ' ' << m << "\n";

	cout << "Graph G {\n";
	for (int i = 0; i < n; ++i) {
		cout << "\t" << i+1 << ";\n";
	}
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		cout << "\t" << a << " -- " << b << ";\n";
	}
	cout << "}\n";

	cerr << "To compile: dot graph.dot -Tpng > graph.png\n";
}
