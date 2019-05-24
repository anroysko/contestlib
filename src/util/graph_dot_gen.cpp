#include <iostream>
using namespace std;

int main() {
	// graph_dot_gen < graph.txt > graph.dot
	// dot graph.dot -Tpng > graph.png
	// xdg-open graph.png

	int n, m;
	cin >> n >> m;
	
	cout << "Graph G {\n";
	for (int i = 0; i < n; ++i) {
		cout << "\t" << i+1 << ";\n";
	}
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		cout << "\t" << a << " -- " << b << ";\n";
	}
	cout << "}";
}
