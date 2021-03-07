#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 1e18;

// Lines of type ax + b
template<class T = ll>
struct Line {
	T a, b;
	T eval(T x) const { return a*x + b; }
};

// Answers queries
//	add_min a b: add line (a, b). It is guaranteed that a is minimum of all lines added so far
//	add_max a b: add line (a, b). It is guaranteed that a is maximum of all lines added so far
//	get_line x: find line (a, b) that minimizes line.eval(x). This won't be the first query.
int main() {
	/*
	int q;
	cin >> q;
	
	vector<Line<ll>> lines;
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add_min") {
			ll a, b;
			cin >> a >> b;
			lines.push_back({a, b});
		} else if (op == "add_max") {
			ll a, b;
			cin >> a >> b;
			lines.push_back({a, b});
		} else if (op == "eval") {
			ll x;
			cin >> x;
			ll v = lines[0].eval(x);
			for (int i = 1; i < lines.size(); ++i) v = min(v, lines[i].eval(x));
			cout << v << '\n';
		}
	}
	*/
	int n, q;
	cin >> n >> q;

	vector<Line<ll>> lines(n);
	for (int i = 0; i < n; ++i) cin >> lines[i].a >> lines[i].b;

	for (int qi = 0; qi < q; ++qi) {
		ll x;
		cin >> x;
		ll v = lines[0].eval(x);
		for (int i = 1; i < lines.size(); ++i) v = min(v, lines[i].eval(x));
		cout << v << '\n';
	}
}








