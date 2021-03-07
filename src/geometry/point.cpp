
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
