#include <iostream>
#include <vector>
using namespace std;

// Builds the z-array for the vector v
// for i > 0, z[i] is the maximum value such that v[0, z[i]) = v[i, i+z[i])
// note the half open interval.
// time complexity: O(n)
vector<int> zAlgo(const vector<int>& v) {
	int n = v.size();
	vector<int> z(n, 0);
	int l = 0; // l and r give interval such that r is maximized,
	int r = 0; // l < i, and r = l + z[l].
	for (int i = 1; i < n; ++i) {
		z[i] = max(0, min(z[i-l], r-i));
		while((i+z[i] < n) && (v[z[i]] == v[i+z[i]])) ++z[i];
		if (i+z[i] > r) {
			l = i;
			r = i+z[i];
		}
	}
	return z;
}

int main() {
        // Example usage:
	int n;
	cin >> n;
	
	vector<int> vec(n);
	for (int i = 0; i < n; ++i) cin >> vec[i];

	vector<int> z = zAlgo(vec);
	for (auto v : z) cout << v << ' '; cout << '\n';
        
	// Example input:
	// 10
	// 0 1 0 0 1 0 1 1 0 1

	// Example output:
	// 0 0 1 3 0 2 0 0 2 0
}
