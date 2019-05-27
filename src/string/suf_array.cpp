#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

// Cyclic suffix index construction. Append -1 or '#' to str to get non-cyclic indexes.
// res[i] := index of suffix starting at i in cyclic suffix array of str
// Complexity: O(n log^2 n), where n = str.size()
vector<int> suffInd(const vector<int>& str) {
	int n = str.size();
	vector<int> ind = str; // ind[i] := index of suffix starting at i in the suffix array
	vector<pair<int, int>> vals(n);
	for (int l = 0; l < n; l = (l ? l<<1 : 1)) {
		for (int i = 0; i < n; ++i) vals[i] = {ind[i], ind[(i+l)%n]};
		ind = compress(vals);
	}
	return ind;
}

int main() {
	string str;
	cin >> str;
	str.push_back('#');
	int n = str.size();

	vector<int> vals(n);
	for (int i = 0; i < n; ++i) vals[i] = str[i] - 'a';
	vector<int> ind = suffInd(vals);

	vector<int> sa(n);
	for (int i = 0; i < n; ++i) sa[ind[i]] = i;

	cout << str << '\n';
	for (auto v : ind) cout << v << ' '; cout << '\n';
	for (auto v : sa) cout << v << ' '; cout << '\n';
}
