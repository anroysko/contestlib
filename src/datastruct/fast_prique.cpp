#include <iostream>
#include <vector>
#include <utility>
using namespace std;
typedef long long ll;
const ll INF = 1e18;

// Struct for priority queue operations on
// index set [1, n] with really good constants
// decKey pushes index if it doesn't exist yet
struct FastPrique {
	vector<pair<ll, int>> data;
	FastPrique(int n, ll ini = INF) : data(2*n, {ini, -1}) {
		data[0] = {-INF, -1};
	}
	void decKey(int i, ll v) {
		int j = i + data.size() / 2;
		while(data[j].first > v) {
			data[j] = {v, i};
			j >>= 1;
		}
	}
	pair<ll, int> pop() {
		auto res = data[1];
		int j = res.second + data.size() / 2;
		data[j] = {INF, -1};
		for (; j > 1; j >>= 1) {
			data[j >> 1] = min(data[j], data[j^1]);
		}
		return res;
	}
	bool empty() {
		return data[1].first == INF;
	}
};

// See djikstra.cpp for example usage
