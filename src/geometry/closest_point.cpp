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

// Not stable, assumes there are no duplicate values
template<class T>
void quickSelect(int a0, int b0, vector<int>& inds, const vector<T>& vals, int k) {
	while(true) {
		int b = b0;
		int i = rand(a0, b0);
		T x = vals[inds[i]];
		swap(inds[i], inds[b0]);

		for (int t = 0, j = a0; t < b0 - a0; ++t) {
			if (vals[inds[j]] > x) {
				--b;
				swap(inds[b], inds[j]);
			} else {
				++j;
			}
		}
		swap(inds[b], inds[b0]);

		if (k < b - a0) {
			b0 = b - 1;
		} else if (k > b + 1 - a0) {
			k -= b + 1 - a0;
			a0 = b + 1;
		} else break;
	}
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

// Half-edge data structure
class EdgeDS {
	private:
		struct HalfEdge {
			int pre, nxt;
			int v;
			HalfEdge(int x, int p, int t) : nxt(x), pre(p), v(t) {}
		};
		vector<HalfEdge> edges;
	public:
		const HalfEdge& operator[](int i) const { return edges[i]; }
		int size() const { return edges.size(); }

		int makeEdge(int s, int t) {
			int i = edges.size();
			edges.emplace_back(i ^ 1, i ^ 1, t);
			edges.emplace_back(i, i, s);
			return i;
		}
		void setNxt(int i, int j) {
			edges[i].nxt = j;
			edges[j].pre = i;
		}
		void weld(int i, int j) {
			setNxt(i, j);
			setNxt(j ^ 1, i ^ 1);
		}
		int connect(int i, int j) {
			int k = makeEdge(edges[i].v, edges[j].v);
			setNxt(k, edges[j].nxt);
			setNxt(k ^ 1, edges[i].nxt);
			setNxt(i, k);
			setNxt(j, k ^ 1);
			return k;
		}
		void delEdge(int i) {
			setNxt(edges[i].pre, edges[i ^ 1].nxt);
			setNxt(edges[i ^ 1].pre, edges[i].nxt);
			edges[i].v = -1;
			edges[i ^ 1].v = -1;
		}
};

// Functions using constructions do not work with integral T or TL!
template<class T, class TL>
struct Geometry {
	struct Point {
		T x, y;
		
		Point() : x(0), y(0) {}
		Point(T x_, T y_) : x(x_), y(y_) {}

		Point operator+(const Point& rhs) const { return {x + rhs.x, y + rhs.y}; }
		Point operator-(const Point& rhs) const { return {x - rhs.x, y - rhs.y}; }
		Point& operator+=(const Point& rhs) { x += rhs.x; y += rhs.y; return *this; }
		Point& operator-=(const Point& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
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

	class Delaunay {
		private:
			struct HalfEdge {
				int nxt, pre, v; // next edge, previous edge, endpoint (outer face in CCW order)
			};

			vector<pair<T, T>> xys, yxs;
			vector<int> inds;
			vector<Point> p;
			EdgeDS e;
			int outf;

			bool valid(int i, int dx, int dy) const {
				Point pt = p[e[i].v] - p[e[i^1].v];
				int val = sign(pt.x) * (2*dx + dy) + sign(pt.y) * (2*dy + dx);
				return val >= 0;
			}
			int advance(int i, int dx, int dy) const {
				while(!valid(i, dx, dy) || valid(e[i].nxt, dx, dy)) {
					i = e[i].nxt;
				}
				return i;
			}

			int merge(int le, int ri) {
				// Merge
				int cur = e.connect(le, ri) ^ 1;

				array<bool, 2> lrv = {1, 1};
				while(lrv[0] || lrv[1]) {
					array<Point, 2> pts1 = {p[e[cur].v], p[e[cur ^ 1].v]};
					array<Point, 2> pts2, pts3;
					array<int, 2> ed = {e[cur].nxt, e[cur].pre};

					for (int j = 0; j <= 1; ++j) {
						pts2[j] = p[e[ed[j] ^ j].v];
						lrv[j] = (leftTurn(pts2[j] - pts1[j^1], pts1[j] - pts1[j^1]) == (j ? 1 : -1));
						if (! lrv[j]) continue;
						
						pts3[j] = p[e[e[ed[j] ^ 1].nxt].v];
						while(inCirc(pts1[j^1], pts1[j], pts2[j], pts3[j]) == 1) {
							int tmp = (j ? e[ed[j] ^ 1].pre : e[ed[j] ^ 1].nxt);
							e.delEdge(ed[j]);
							ed[j] = tmp;
							pts2[j] = p[e[ed[j] ^ j].v];
							pts3[j] = p[e[e[ed[j] ^ 1].nxt].v];
						}
					}
					if (lrv[0] && (! lrv[1] || inCirc(pts1[0], pts1[1], pts2[1], pts2[0]) == 1)) {
						cur = e.connect(ed[0], ed[1]) ^ 1;
					} else if (lrv[1]) {
						cur = e.connect(cur, e[ed[1]].pre) ^ 1;
					}
				}
				return cur;
			}

			// Returns pointer to an edge on the outer face, pointing to the vertex with minimum y-coordinate
			int recBuild(int ia, int ib, bool f) {
				int k = ib - ia + 1;
				if (k == 2) {
					return e.makeEdge(inds[ia], inds[ia + 1]);
				} else if (k == 3) {
					if (xys[inds[ia + 0]] > xys[inds[ia + 1]]) swap(inds[ia + 0], inds[ia + 1]);
					if (xys[inds[ia + 1]] > xys[inds[ia + 2]]) swap(inds[ia + 1], inds[ia + 2]);
					if (xys[inds[ia + 0]] > xys[inds[ia + 1]]) swap(inds[ia + 0], inds[ia + 1]);

					int i = e.makeEdge(inds[ia], inds[ia + 1]);
					e.makeEdge(inds[ia + 1], inds[ia + 2]);
					e.weld(i, i+2);

					int turn = leftTurn(p[inds[ia + 2]] - p[inds[ia]], p[inds[ia + 1]] - p[inds[ia]]);
					if (turn != 0) {
						e.makeEdge(inds[ia + 2], inds[ia]);
						e.weld(i+2, i+4);
						e.weld(i+4, i);
					}
					return (turn == -1 ? (i ^ 1) : i);
				} else {
					int h = k/2;
					quickSelect(ia, ib, inds, (f ? yxs : xys), h);
					int le = advance(recBuild(ia, ia + h-1, f ^ 1), f^1, f);
					int ri = advance(recBuild(ia + h, ib, f ^ 1), -(f^1), -f);

					// Back down as far as possible
					while(true) {
						Point a = p[e[le].v];
						Point b = p[e[ri].v];
						Point a2 = p[e[e[le].pre].v];
						Point b2 = p[e[e[ri].nxt].v];
						if (leftTurn(b2 - a, b - a) == -1) ri = e[ri].nxt;
						else if (leftTurn(a2 - b, a - b) == 1) le = e[le].pre;
						else break;
					}
					return merge(le, ri);
				}
			}
		public:
			Delaunay(const vector<Point>& pts) : p(pts), inds(pts.size()), xys(pts.size()), yxs(pts.size()) {
				for (int i = 0; i < p.size(); ++i) {
					inds[i] = i;
					xys[i] = {p[i].x, p[i].y};
					yxs[i] = {p[i].y, p[i].x};
				}
				outf = recBuild(0, (int)p.size() - 1, 0);
			}

			// Removes all edges that are not in every Delaunay triangulation
			void trim() {
				vector<bool> outer(e.size(), 0);
				for (int i = outf; !outer[i]; i = e[i].nxt) outer[i] = 1;

				vector<int> rems;
				for (int i = 0; i < e.size(); ++i) {
					if ((i & 1) || outer[i] || outer[i ^ 1] || e[i].v == -1) continue;
					Point a = p[e[i].v];
					Point b = p[e[i^1].v];
					Point c = p[e[e[i].nxt].v];
					Point d = p[e[e[i^1].nxt].v];
					if (inCirc(a, b, c, d) == 0) rems.push_back(i);
				}
				for (auto i : rems) e.delEdge(i);
			}

			vector<vector<int>> getConns() const {
				vector<vector<int>> g(p.size());
				for (int i = 0; i < (int)e.size(); ++i) {
					if (e[i].v != -1) g[e[i ^ 1].v].push_back(e[i].v);
				}
				return g;
			}
			const EdgeDS& getEdges() const { return e; }
			int outerFace() const { return outf; }
	};

	class PointLocator {
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

	// Center of circumcircle of triangle abc
	static Point circumcenter(const Point& a, const Point& b, const Point& c) {
		static_assert(! is_integral<T>::value && ! is_integral<TL>::value); // Constructions need reals!
		TL m = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
		TL bx = -(a.sqLength() * (b.y - c.y) + b.sqLength() * (c.y - a.y) + c.sqLength() * (a.y - b.y));
		TL by = (a.sqLength() * (b.x - c.x) + b.sqLength() * (c.x - a.x) + c.sqLength() * (a.x - b.x));
		return {-bx / m, -by / m};
	}
};

// Geom::Point, Geom::Delaunay etc
// using Geom = Geometry<GeomDouble<ld>, GeomDouble<ld>>;
using Geom = Geometry<ll, lll>;
using GeomR = Geometry<ld, ld>;
istream& operator>>(istream& s, Geom::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const Geom::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }
istream& operator>>(istream& s, GeomR::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const GeomR::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }

// Builds the Voronoi diagram of the point set (strictly contained in [-h, h]^2) bounded by some (non-convex) polygon containing [-h, h]^2.
// res[i] is a vector containing the indices of vertices on face i, in counter-clockwise (CCW) order
static pair<vector<GeomR::Point>, vector<vector<int>>> buildVoronoi(vector<Geom::Point> p, ll h) {
	int n = p.size();
	for (int dx = -1; dx <= 1; dx += 2) {
		for (int dy = -1; dy <= 1; dy += 2) {
			p.emplace_back(3*h*dx, 3*h*dy);
		}
	}

	Geom::Delaunay del(p);
	del.trim();
	EdgeDS e = del.getEdges();

	vector<GeomR::Point> c(p.size());
	for (int i = 0; i < p.size(); ++i) c[i] = {p[i].x, p[i].y};

	int m = e.size();
	vector<int> face(m, -1);
	for (int i = del.outerFace(); face[i] == -1; i = e[i].nxt) face[i] = m;

	vector<GeomR::Point> pts;
	for (int i = 0; i < m; ++i) {
		if (e[i].v == -1 || face[i] != -1) continue;
		for (int t = i; face[t] == -1; t = e[t].nxt) face[t] = pts.size();
		pts.push_back(GeomR::circumcenter(c[e[i^1].v], c[e[i].v], c[e[e[i].nxt].v]));
	}

	vector<vector<int>> res(n);
	for (int i = 0; i < m; ++i) {
		int t = e[i].v;
		if (t == -1 || t >= n || !res[t].empty()) continue;

		int j = i;
		do {
			res[t].push_back(face[j]);
			j = e[j^1].pre;
		} while (j != i);
	}
	return {pts, res};
}

class ClosestPoint {
	private:
		GeomR::PointLocator loc;
	public:
		ClosestPoint(const vector<Geom::Point>& p, ll h) {
			for (auto pt : p) h = max(h, abs(pt.x), abs(pt.y));
			auto voronoi = buildVoronoi(p, h);

			for (auto& vec : voronoi.second) reverse(vec.begin(), vec.end());
			loc.init(voronoi.first, voronoi.second);
		}
		int query(GeomR::Point p) const {
			return loc.locate(p);
		}
};

const int H = 1e7;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	ll h = H;

	int n, q;
	cin >> n >> q;

	vector<Geom::Point> pts(n);
	for (int i = 0; i < n; ++i) {
		cin >> pts[i];
		h = max(h, abs(pts[i].x), abs(pts[i].y));
	}
	
	ClosestPoint cl(pts, h);

	for (int i = 0; i < q; ++i) {
		GeomR::Point p;
		cin >> p;
		cout << cl.query(p) << '\n';
	}
}
