#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using lll = __int128;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

template<class T>
T min(T a, T b, T c) { return min(a, min(b, c)); }

template<class T>
T max(T a, T b, T c) { return max(a, max(b, c)); }

template<class T>
int sign(T v) { return (v > 0) - (v < 0); }

template<class T, class TL>
struct Geometry {
	struct Point {
		T x, y;
		
		Point() : x(0), y(0) {}
		Point(T x_, T y_) : x(x_), y(y_) {}

		Point operator+(const Point& rhs) const { return {x + rhs.x, y + rhs.y}; }
		Point operator-(const Point& rhs) const { return {x - rhs.x, y - rhs.y}; }
		Point operator*(const T& v) const { return {x * v, y * v}; }
		Point operator/(const T& v) const { return {x / v, y / v}; }
		TL sqLength() const { return (TL)x*x + (TL)y*y; }

		// Complex operations
		Point conj() const { return {x, -y}; }
		Point inv() const { return conj() / sqLength(); }

		Point operator*(const Point& rhs) const { return {x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x}; }
		Point operator/(const Point& rhs) const { return *this * rhs.inv(); }
	};
	static TL dot(const Point& a, const Point& b) { return (TL)a.x * b.x + (TL)a.y * b.y; }
	static TL cross(const Point& a, const Point& b) { return (TL)a.x * b.y - (TL)a.y * b.x; }

	// Does vector 0 -> a turn LEFT of vector 0 -> b (1 left, 0 parallel, -1 right)
	static int leftTurn(const Point& a, const Point& b) {
		return -sign(cross(a, b));
	}

	// Is point p in the circumcircle of triangle abc? (1 strictly in, 0 in, -1 out)
	static int inCirc(Point a, Point b, Point c, const Point& p) {
		a -= p; b -= p; c -= p;
		TL res = 0;
		res += a.sqLength() * ((TL)b.x * c.y - (TL)b.y * c.x);
		res += b.sqLength() * ((TL)a.y * c.x - (TL)a.x * c.y);
		res += c.sqLength() * ((TL)a.x * b.y - (TL)a.y * b.x);
		return sign(res) * leftTurn(c - a, b - a);
	}

	// Is point p in the smallest circle with point a and b on its boundary? (1 strictly in, 0 in, -1 out)
	static int inCirc(Point a, Point b, Point p) {
		return sign((a - b).sqLength() - (p*2 - (a + b)).sqLength());
	}

	// Returns indices of points on the smallest enclosing circle of given vector of points
	static vector<int> welzl(const vector<Point>& pts) {
		int n = pts.size();
		vector<int> ord(n);
		for (int i = 0; i < n; ++i) ord[i] = i;
		shuffle(ord.begin(), ord.end(), rng);

		vector<int> nxt(n, -1);
		for (int i = 0; i+1 < n; ++i) nxt[ord[i]] = ord[i+1];
		int r = ord[0];

		vector<int> sta, res;
		int i = ord[0];
		int i0 = -1;
		int ind = 0;
		while(i != -1) {
			if (res.empty() || (res.size() == 1 && res[0] != i) || (res.size() == 2 && inCirc(pts[res[0]], pts[res[1]], pts[i]) == -1)
					|| (res.size() == 3 && inCirc(pts[res[0]], pts[res[1]], pts[res[2]], pts[i]) == -1)) {
				while(! sta.empty() && ind > sta.back()) {
					sta.pop_back();
					res.pop_back();
				}
				sta.push_back(ind);
				res.push_back(i);
				
				if (i0 != -1) {
					nxt[i0] = nxt[i];
					nxt[i] = r;
					r = i;

					i = r;
					i0 = -1;
					ind = 0;
				}
			} else {
				i0 = i;
				i = nxt[i];
				++ind;
			}
		}
		return res;
	}
};

// Geom::Point, Geom::Delaunay etc
// using Geom = Geometry<GeomDouble<ld>, GeomDouble<ld>>;
using Geom = Geometry<ll, lll>;
istream& operator>>(istream& s, Geom::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const Geom::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }
