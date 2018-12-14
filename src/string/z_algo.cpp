#include <iostream>
#include <vector>
using namespace std;

// Builds the z-array for the vector v
// for i > 0, res[i] is the maximum value such that v[0, res[i]) = v[i, i+res[i])
// note the half open interval.
// time complexity: O(n)
vector<int> zAlgo(const vector<int>& v) {
	int n = v.size();
	vector<int> res(n, 0);
	int l = -1; // l and r give interval such that r is maximized,
	int r = -1; // l < i, and r = l + res[l].

	for (int i = 1; i < n; ++i) {
		if (i < r) res[i] = min(res[i - l], r-i);
		if (i + res[i] < r) continue;

		while((i + res[i] < n) && (v[res[i]] == v[i + res[i]])) {
			++res[i];
		}

		l = i;
		r = i+res[i];
	}
	return res;
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
