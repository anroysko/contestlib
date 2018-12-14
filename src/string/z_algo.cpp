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
	int l = -1;
	int r = -1;

	for (int i = 1; i < n; ++i) {
		if (i <= r) res[i] = min(res[i - l], r-i);
		for (;; ++res[i]) {
			if (r > i+res[i]) break;
			if (i + res[i] >= n) break;
			if (v[res[i]] != v[i + res[i]]) break;
			l = i;
			r = i+res[i];
		}
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
