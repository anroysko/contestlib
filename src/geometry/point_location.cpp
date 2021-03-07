#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
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

template<class T>
struct Ratio {
	T a, b; // a / b

	Ratio() : a(0), b(1) {}
	Ratio(T va, T vb) : a(va), b(vb) {
		if (b < 0) {
			a *= -1;
			b *= -1;
		}
	}
	bool operator<(const Ratio& rhs) {
		return a * rhs.b < b * rhs.a;
	}
};

// returns number of elements strictly smaller than v in vec
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
class PersistentRangeMin {
	private:
		struct Node {
			int le, ri;
			T minv;
			Node(int le_, int ri_, T minv_) : le(le_), ri(ri_), minv(minv_) {}
		};
		vector<Node> hist; 
		vector<int> roots;
		int n;

		int makeNode(T minv) {
			hist.emplace_back(-1, -1, minv);
			return (int)hist.size() - 1;
		}
		int makeNode(int le, int ri) {
			hist.emplace_back(le, ri, min(hist[le].minv, hist[ri].minv));
			return (int)hist.size() - 1;
		}
		int recBuild(int ia, int ib, const vector<T>& v) {
			if (ia + 1 == ib) return makeNode(v[ia]);

			int im = (ia + ib) >> 1;
			return makeNode(recBuild(ia, im, v), recBuild(im, ib, v));
		}
		int recSet(int ind, const T& v, int i, int ia, int ib) {
			if (ia + 1 == ib) return makeNode(v);

			int im = (ia + ib) >> 1;
			if (ind < im) {
				return makeNode(recSet(ind, v, hist[i].le, ia, im), hist[i].ri);
			} else {
				return makeNode(hist[i].le, recSet(ind, v, hist[i].ri, im, ib));
			}
		}
		T recGet(int a, int b, int i, int ia, int ib) const {
			if (a <= ia && b <= ib) return hist[i].minv;

			int im = (ia + ib) >> 1;
			if (a >= im) return recGet(a, b, hist[i].ri, im, ib);
			
			T r0 = recGet(a, b, hist[i].le, ia, im);
			if (b > im) {
				T r1 = recGet(a, b, hist[i].ri, im, ib);
				if (r1 < r0) return r1;
			}
			return r0;
		}
		int recSearch(int a, const T& v, int i, int ia, int ib) const {
			if (ia + 1 == ib) return (hist[i].minv <= v ? ia : n);

			int im = (ia + ib) >> 1;
			if (a < im && (hist[hist[i].le].minv <= v)) {
				int r = recSearch(a, v, hist[i].le, ia, im);
				if (r < n) return r;
			}
			return recSearch(a, v, hist[i].ri, im, ib);
		}
	public:	
		void init(const vector<T>& v) {
			n = v.size();
			int r = recBuild(0, n, v);
			roots.push_back(r);
		}
		// Sets val[ind] = v to array after update t, and returns index of the update made
		int set(int ind, const T& v, int t) {
			int r = recSet(ind, v, roots[t], 0, n);
			roots.push_back(r);
			return (int)roots.size() - 1;
		}
		// Returns range minimum after update t. MUST HAVE a <= b
		T get(int a, int b, int t) const {
			return recGet(a, b + 1, roots[t], 0, n);
		}
		// Returns smallest ind >= a s.t. after update t, it held that val[ind] <= v, or n if there is none
		int search(int a, T v, int t) const {
			return recSearch(a, v, roots[t], 0, n);
		}
};

template<class T>
struct GeomDouble {
	static constexpr T eps = 1e-9; // Change this!
	T v;

	GeomDouble() : v(0) {}
	GeomDouble(T val) : v(val) {}
	operator T() const { return v; }

	template<class K> GeomDouble& operator+=(const K& rhs) { v += (T)rhs; return *this; }
	template<class K> GeomDouble& operator-=(const K& rhs) { v -= (T)rhs; return *this; }
	template<class K> GeomDouble& operator*=(const K& rhs) { v *= (T)rhs; return *this; }
	template<class K> GeomDouble& operator/=(const K& rhs) { v /= (T)rhs; return *this; }
	template<class K> GeomDouble operator+(const K& rhs) const { return {v + (T)rhs}; }
	template<class K> GeomDouble operator-(const K& rhs) const { return {v - (T)rhs}; }
	template<class K> GeomDouble operator*(const K& rhs) const { return {v * (T)rhs}; }
	template<class K> GeomDouble operator/(const K& rhs) const { return {v / (T)rhs}; }
	template<class K> bool operator<(const GeomDouble& rhs) const { return v <= (T)rhs - eps; }
	template<class K> bool operator>(const GeomDouble& rhs) const { return v >= (T)rhs + eps; }
	template<class K> bool operator<=(const GeomDouble& rhs) const { return v < (T)rhs + eps; }
	template<class K> bool operator>=(const GeomDouble& rhs) const { return v > (T)rhs - eps; }
	template<class K> bool operator==(const GeomDouble& rhs) const { return abs(v - (T)rhs) < eps; }
	template<class K> bool operator!=(const GeomDouble& rhs) const { return abs(v - (T)rhs) >= eps; }
};
template<class T>
GeomDouble<T> abs(const GeomDouble<T>& val) { return {abs(val.v)}; }

template<class T>
istream& operator>>(istream& s, GeomDouble<T>& val) { return s >> val.v; }

template<class T>
ostream& operator<<(ostream& s, GeomDouble<T>& val) { return s << val.v; }

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

	class PointLocation {
		private:
			struct Segment {
				Point a, b;

				Segment(const Point& ap, const Point& bp) : a(ap), b(bp) {
					if (a.y < b.y || (a.y == b.y && a.x < b.x)) swap(a, b);
				}

				Ratio<TL> xAt(T y) const {
					if (a.y == b.y) return Ratio<TL>(max(a.x, b.x), 1);
					else return Ratio<TL>((TL)a.x * (a.y - b.y) + (TL)(b.x - a.x) * (a.y - y), a.y - b.y);
				}

				// Assumes the segments do not intersect at more than one point, and share a y-coordinate
				bool operator<(const Segment& rhs) const {
					T y0 = min(a.y, rhs.a.y);
					T y1 = max(b.y, rhs.b.y);
					return y0 >= y1 && ((xAt(y0) < rhs.xAt(y0)) || (xAt(y1) < rhs.xAt(y1)));
				}
				bool operator>(const Segment& rhs) const { return rhs < *this; }
				bool operator<=(const Segment& rhs) const { return !(*this > rhs); }
				bool operator>=(const Segment& rhs) const { return !(*this < rhs); }

			};

			vector<Point> points;
			vector<pair<Segment, int>> segs;
			PersistentRangeMin<Segment> range_min;
			vector<T> ev_ys;

			vector<pair<T, int>> makeEvs() {
				int m = segs.size();
				vector<pair<T, int>> evs(2*m);
				for (int j = 0; j < m; ++j) {
					evs[2*j] = {segs[j].first.a.y, -(j+1)}; // Start segment j
					evs[2*j + 1] = {segs[j].first.b.y, j+1}; // End segment j
				}
				sort(evs.begin(), evs.end());
				reverse(evs.begin(), evs.end());
				return evs;
			}

			void order() {
				int m = segs.size();
				auto evs = makeEvs();

				int first = -1;
				vector<int> nxt(m, -1);
				set<pair<Segment, int>> cur;
				for (auto pr : evs) {
					int j = pr.second;
					if (j < 0) {
						j = -j - 1;
						auto it = cur.insert({segs[j].first, j}).first;
						if (it == cur.begin()) {
							++it;
							nxt[j] = (first != -1 ? first : (it == cur.end() ? -1 : (*it).second));
							first = j;
						} else {
							--it;
							int i = (*it).second;
							nxt[j] = nxt[i];
							nxt[i] = j;
						}
					} else {
						j = j - 1;
						cur.erase({segs[j].first, j});
					}
				}

				vector<int> ord;
				for (int i = first; i != -1; i = nxt[i]) ord.push_back(i);
				reverse(ord.begin(), ord.end());

				auto tmp = segs;
				for (int i = 0; i < m; ++i) segs[i] = tmp[ord[i]];			
			}
			void build(T inf) {
				int m = segs.size();

				auto evs = makeEvs();
				while(evs.back().second > 0) evs.pop_back();

				Segment inf_seg({inf, inf}, {inf, -inf});
				range_min.init(vector<Segment>(m, inf_seg));
				ev_ys.push_back(inf);
				
				int t = 0;
				for (auto pr : evs) {
					int j = pr.second;
					ev_ys.push_back(pr.first);
					if (j < 0) {
						j = -j - 1;
						range_min.set(j, segs[j].first, t);
					} else {
						j = j - 1;
						range_min.set(j, inf_seg, t);
					}
					++t;
				}
			}
		public:
			// Points in a polygon must be listed in COUNTERCLOCKWISE order -- Interior to the LEFT of edges
			void init(const vector<Point>& pts, const vector<vector<int>>& polygons) {
				points = pts;
				for (int j = 0; j < polygons.size(); ++j) {
					const vector<int>& p = polygons[j];
					for (int i = 0; i < p.size(); ++i) {
						int i2 = (i+1) % p.size();
						if (points[p[i]].y > points[p[i2]].y) {
							segs.emplace_back(Segment(points[p[i]], points[p[i2]]), j);
						}
					}
				}

				order();

				T inf = 0;
				for (auto p : pts) inf = max(inf, abs(p.x), abs(p.y));
				inf += 1;

				build(inf);
			}
			// The point must be in one of the input polygons!
			int locate(const Point& p) const {
				int low = -1;
				int high = (int)ev_ys.size() - 1;
				while(low != high) {
					int mid = (low + high + 1) >> 1;
					if (p.y <= ev_ys[mid]) low = mid;
					else high = mid - 1;
				}

				Segment seg(p, p);
				int i = range_min.search(0, seg, low);
				if (i >= segs.size()) return -1;
				return segs[i].second;
			}
	};
};

// Geom::Point, Geom::Delaunay etc
// using Geom = Geometry<ll, __int128>;
using Geom = Geometry<GeomDouble<ld>, GeomDouble<ld>>;
istream& operator>>(istream& s, Geom::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const Geom::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, q;
	cin >> n >> m >> q;

	vector<Geom::Point> points(n);
	for (auto& p : points) cin >> p;

	vector<vector<int>> polygons(m);
	for (int i = 0; i < m; ++i) {
		int k;
		cin >> k;
		polygons[i].resize(k);
		for (int& j : polygons[i]) cin >> j;
	}

	Geom::PointLocation loc;
	loc.init(points, polygons);

	for (int i = 0; i < q; ++i) {
		Geom::Point p;
		cin >> p;

		cout << loc.locate(p) << '\n';
	}
}
