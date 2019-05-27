#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

// Binary search. Assumes vec is sorted
// res := number of elements strictly smaller than v in vec
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

// Coordinate Compression
// res[i] := number of unique elements in vec strictly smaller than vec[i]
// Complexity: O(n log n), where n = vec.size()
template<class T>
vector<int> compress(const vector<T>& vec) {
	vector<T> coll = vec;
	sort(coll.begin(), coll.end());
	coll.erase(unique(coll.begin(), coll.end()), coll.end());

	vector<int> res(vec.size());
	for (int i = 0; i < vec.size(); ++i) res[i] = bins(coll, vec[i]);
	return res;
}

// Example usage
int main() {
	int n;
	cin >> n;

	vector<ll> vals(n);
	for (int i = 0; i < n; ++i) cin >> vals[i];

	vector<int> comp = compress(vals);
	for (auto v : comp) cout << v << ' '; cout << '\n';
}
