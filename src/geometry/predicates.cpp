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

	// Is point p in triangle t? (1 strictly in, 0 in, -1 out)
	static int contains(const array<Point, 3>& t, const Point& p) {
		int a = leftTurn(p - t[0], t[1] - t[0]);
		int b = leftTurn(p - t[1], t[2] - t[1]);
		int c = leftTurn(p - t[2], t[0] - t[2]);

		int mi = min(a, b, c);
		int ma = max(a, b, c);
		if (mi == -ma) return -1;
		else return (mi != 0);
	}

	// Are points t in convex position? (1 strictly convex, 0 convex, -1 not convex)
	static int convPos(const array<Point, 4>& t) {
		return leftTurn(t[2] - t[0], t[1] - t[0]) * leftTurn(t[3] - t[0], t[1] - t[0])
			* leftTurn(t[0] - t[2], t[3] - t[2]) * leftTurn(t[1] - t[2], t[3] - t[2]);
	}

	// Is point p in the circumcircle of triangle t? (1 strictly in, 0 in, -1 out)
	// Overflows if (|p| + |a| + |b| + |c|)^4 overflows TL
	static array<TL, 3> lift(const Point& p) {
		return {p.x, p.y, p.sqLength()};
	}

	static int inCirc(const array<Point, 3>& t, const Point& p) {
		auto la = lift(t[0] - p), lb = lift(t[1] - p), lc = lift(t[2] - p);
		if (leftTurn(t[2] - t[0], t[1] - t[0]) == 1) swap(lb, lc);

		TL res = 0;
		res += la[0] * (lb[1] * lc[2] - lb[2] * lc[1]);
		res += la[1] * (lb[2] * lc[0] - lb[0] * lc[2]);
		res += la[2] * (lb[0] * lc[1] - lb[1] * lc[0]);
		return -sign(res);
	}
};

// Geom::Point, Geom::Delaunay etc
// using Geom = Geometry<GeomDouble<ld>, GeomDouble<ld>>;
using Geom = Geometry<ll, __int128>;
istream& operator>>(istream& s, Geom::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const Geom::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }
