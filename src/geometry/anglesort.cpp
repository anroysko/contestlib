
template<class T, class TL>
struct Point {
	T x, y;
	
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
template<class T, class TL>
istream& operator>>(istream& s, Point<T, TL>& p) { return s >> p.x >> p.y; }

template<class T, class TL>
ostream& operator<<(ostream& os, Point<T, TL>& p) { return s << "(" << p.x << ", " << p.y << ")"; }

template<class T, class TL>
TL dot(const Point<T, TL>& a, const Point<T, TL>& b) { return (TL)a.x * b.x + (TL)a.y * b.y; }

template<class T, class TL>
TL cross(const Point<T, TL>& a, const Point<T, TL>& b) { return (TL)a.x * b.y - (TL)a.y * b.x; }

// Does vector 0 -> a turn LEFT of vector 0 -> b (1 left, 0 parallel, -1 right)
template<class T>
int sign(T v) { return (v > 0) - (v < 0); }

template<class T, class TL>
int leftTurn(const Point<T, TL>& a, const Point& b) {
	return -sign(cross(a, b));
}

// Counter-clockwise ordering around (0, 0), starting form direction (0, 1)
// sort(points.begin(), points.end(), ccw);
template<class T, class TL>
int qr(const Point<T, TL>& a) {
	return 3*(a.x < 0) ^ (a.y < 0);
}

template<class T, class TL>
bool ccw(const Point& a, const Point& b) {
	if (qr(a) != qr(b)) return qr(a) < qr(b);
	return (leftTurn(a, b) == 1);
}

