#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
using seglist = vector<pair<pair<ll, ll>, int>>;
const ll INF = 1e18;

bool contains(pair<ll, ll> a, pair<ll, ll> b) {
	return (a.first <= b.first && b.second <= a.second);
}
bool overlaps(pair<ll, ll> a, pair<ll, ll> b) {
	return (a.first <= b.second && b.first <= a.second);
}
void recBuildOps(pair<ll, ll> act, const seglist& par_s, const seglist& par_q, vector<pair<int, int>>& ops) {
	vector<int> s, q;
	for (auto que : par_q) {
		if (overlaps(que.first, act)) q.emplace_back(que);
	}
	if (q.empty()) return;

	for (auto seg : par_s) {
		if (contains(seg.first, act)) ops.emplace_back(1, seg.second);
		else if (overlaps(seg.first, act)) s.emplace_back(seg);
	}
	if (s.empty()) {
		for (auto que : q) ops.emplace_back(0, que.second);
	} else {
		ll m = (act.first + act.second) / 2;
		recBuildOps({act.first, m}, s, q, ops);
		recBuildOps({m+1, act.second}, s, q, ops);
	}
	for (auto seg = par_s.rbegin(); seg != par_s.rend(); ++seg) {
		if (contains(seg->first, act)) ops.emplace_back(-1, seg->second);
	}
}

vector<pair<int, int>> buildOps(const vector<pair<ll, ll>>& segs, const vector<ll>& ques) {
	seglist s(segs.size());
	seglist q(ques.size());
	for (int j = 0; j < s.size(); ++j) s[j] = {segs[j], j};
	for (int j = 0; j < q.size(); ++j) q[j] = {{ques[j], ques[j]}, j};

	ll a = INF;
	ll b = -INF;
	for (auto v : ques) {
		a = min(a, v);
		b = max(a, v);
	}

	vector<pair<int, int>> ops;
	recBuildOps({a, b}, s, q, ops);
	return ops;
}

// Example usage. Does offline dynamic connectivity
int main() {
	// TODO
}
