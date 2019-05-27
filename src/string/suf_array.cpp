#include <iostream>
#include <algorithm>
using namespace std;

// Finds number of elements smaller than v. If v exists in vec, returns its index.
// Complexity: O(log n), where n = vec.size()
template<class T>
int bins(const vector<T>& vec, T v) {
	int low = 0;
	int high = vec.size();
	while(low != high) {
		int mid = (low + high) >> 1;
		if (vec[mid] < v) low = mid + 1;
		else high = mid;
	}
	return low;
}

// Returns sorted array of unique values in the input vector
// Complexity: O(n log n), where n = vec.size()
template<class T>
vector<T> sortedUnique(vector<T> vec) {
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
	return vec;
}

// Coordinate Compression
// res[i] is the amount of unique elements in vec strictly smaller than vec[i].
// Complexity: O(n log n), where n = vec.size()
template<class T>
vector<int> compress(const vector<T>& vec) {
	vector<T> coll = sortedUnique(vec);
	vector<int> res(vec.size());
	for (int i = 0; i < vec.size(); ++i) res[i] = bins(coll, vec[i]);
	return res;
}

// Builds ind-array of suffix array for the input.
// Cyclic, append '#' or -1 if you want to get non-cyclic result.
// Complexity: O(n log^2 n), where n = str.size()
vector<int> sa(const vector<int>& str) {
	int n = str.size();
	vector<int> ind = str;
	vector<pair<int, int>> vals(n);
	for (int l = 0; l < n; l = (l ? l<<1 : 1)) {
		for (int i = 0; i < n; ++i) vals[i] = {ind[i], ind[(i+l)%n]};
		ind = collapse(vals);
	}
	return ind;
}

int main() {
	// TODO: example usage
}
