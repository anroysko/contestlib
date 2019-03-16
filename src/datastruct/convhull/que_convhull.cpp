#include <iostream>
#include <deque>
using namespace std;

// Lines of type ax + b
template<class T1, class T2 = T1>
struct Line {
	T1 a, b;
	T2 eval(T2 x) { return (T2)a*x + b; }

	// Returns true if for all x min(le.eval(x), ri.eval(x)) < eval(x)
	// Special case: returns true if le==this==ri. This is needed to
	// not have duplicates in the convex hull. Assumes le.a <= a <= ri.a.
	bool redundant(Line le, Line ri) {
		if (le.a == ri.a) return b >= min(le.b, ri.b);
		return (T2)(le.b - ri.b) * (le.a - a) < (T2)(le.b - b) * (le.a - ri.a);
	}
};

// Convex hull with O(1) insertion under some conditions, and log(n) queries
// Lines li in "lines" are sorted by li.a
template<class T1, class T2 = T1>
struct ConvHull {
	deque<Line<T1, T2>> lines;

	// Add a line. Must have minimum a over all added lines
	void addMin(Line<T1, T2> li) {
		int n = lines.size();
		for (; n && lines[0].redundant(li, lines[min(1, n-1)]); --n) {
			lines.pop_front();
		}
		if (n == 0 || (! li.redundant(li, lines[0]))) lines.push_front(li);
	}
	// Add a line. Must have maximum a over all added lines
	void addMax(Line<T1, T2> li) {
		int n = lines.size();
		for (; n && lines[n-1].redundant(lines[max(0, n-2)], li); --n) {
			lines.pop_back();
		}
		if (n == 0 || (! li.redundant(lines[n-1], li))) lines.push_back(li);
	}
	// Returns line li minimizing li.eval(x). Must have at least one line.
	Line<T1, T2> getLine(T2 x) {
		int low = 0;
		int high = (int)lines.size() - 1;
		while(low < high) {
			int mid = (low + high) >> 1;
			if (lines[mid].eval(x) < lines[mid+1].eval(x)) {
				high = mid;
			} else {
				low = mid + 1;
			}
		}
		return lines[low];
	}
};

// Example usage, should output
//	3 1 3
//	3 11 13
//	1000000000000000000
//	-10 -9.9 0
int main() {
	ConvHull<int> hull1;
	ConvHull<int, long long> hull2;
	ConvHull<double> hull3;

	hull1.addMin({5, 1});
	hull1.addMin({2, 8});
	hull1.addMin({2, 7});
	hull1.addMax({8, -5});
	hull1.addMin({2, 9});

	hull2.addMin({(int)1e9, 0});

	hull3.addMin({1e-17, -9.9});
	hull3.addMin({0, 0});
	hull3.addMax({0.5, -5.0});
	hull3.addMax({1.0, -10.0});

	cout << hull1.lines.size() << ' ' << hull2.lines.size() << ' ' << hull3.lines.size() << '\n';
	cout << hull1.getLine(1).eval(1) << ' ' << hull1.getLine(2).eval(2) << ' ' << hull1.getLine(3).eval(3) << endl;
	cout << hull2.getLine(0).eval((int)1e9) << endl;
	cout << hull3.getLine(1e-18).eval(1e-18) << ' ' << hull3.getLine(1.0).eval(1.0) << ' ' << hull3.getLine(1e18).eval(1e18) << endl;
	
}
