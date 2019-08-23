#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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
template<class T>
vector<int> compress(const vector<T>& vec) {
	vector<T> coll = vec;
	sort(coll.begin(), coll.end());
	coll.erase(unique(coll.begin(), coll.end()), coll.end());

	vector<int> res(vec.size());
	for (int i = 0; i < vec.size(); ++i) res[i] = bins(coll, vec[i]);
	return res;
}
vector<int> suffInd(const string& str) {
	int n = str.size();
	vector<int> ind(n);
	for (int i = 0; i < n; ++i) ind[i] = str[i];
	vector<pair<int, int>> vals(n);
	for (int l = 0; l < n; l = (l ? l<<1 : 1)) {
		for (int i = 0; i < n; ++i) vals[i] = {ind[i], ind[(i+l)%n]};
		ind = compress(vals);
	}
	return ind;
}

void solve() {
	string str;
	cin >> str;
	str.push_back('$');
	int n = str.size();

	vector<int> rev = suffInd(str);
	vector<int> sa(n);
	for (int i = 0; i < n; ++i) sa[rev[i]] = i;

	for (auto v : sa) cout << v << ' '; cout << '\n';
	for (auto v : rev) cout << v << ' '; cout << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}
