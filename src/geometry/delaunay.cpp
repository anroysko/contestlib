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
		Point& operator+=(const Point& rhs) { x += rhs.x; y += rhs.y; return *this; }
		Point& operator-=(const Point& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
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
				HalfEdge(int x, int p, int t) : nxt(x), pre(p), v(t) {}
			};

			vector<Point> points;
			vector<HalfEdge> edges;
			vector<int> inds;

			bool valid(int i, int dx, int dy) const {
				Point p = points[edges[i].v] - points[edges[i^1].v];
				int val = sign(p.x) * (2*dx + dy) + sign(p.y) * (2*dy + dx);
				return val >= 0;
			}
			int advance(int i, int dx, int dy) const {
				while(!valid(i, dx, dy) || valid(edges[i].nxt, dx, dy)) {
					i = edges[i].nxt;
				}
				return i;
			}
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

			int merge(int le, int ri) {
				// Merge
				int cur = connect(le, ri) ^ 1;

				array<bool, 2> lrv = {1, 1};
				while(lrv[0] || lrv[1]) {
					array<Point, 2> pts1 = {points[edges[cur].v], points[edges[cur ^ 1].v]};
					array<Point, 2> pts2, pts3;
					array<int, 2> ed = {edges[cur].nxt, edges[cur].pre};

					for (int j = 0; j <= 1; ++j) {
						pts2[j] = points[edges[ed[j] ^ j].v];
						pts3[j] = points[edges[edges[ed[j] ^ 1].nxt].v];
						lrv[j] = (leftTurn(pts2[j] - pts1[j^1], pts1[j] - pts1[j^1]) == (j ? 1 : -1));
						while(lrv[j] && inCirc(pts1[j^1], pts1[j], pts2[j], pts3[j]) == 1) {
							int tmp = (j ? edges[ed[j] ^ 1].pre : edges[ed[j] ^ 1].nxt);
							delEdge(ed[j]);
							ed[j] = tmp;
							pts2[j] = points[edges[ed[j] ^ j].v];
							pts3[j] = points[edges[edges[ed[j] ^ 1].nxt].v];
						}
					}
					if (lrv[0] && (! lrv[1] || inCirc(pts1[0], pts1[1], pts2[1], pts2[0]) == 1)) {
						cur = connect(ed[0], ed[1]) ^ 1;
					} else if (lrv[1]) {
						cur = connect(cur, edges[ed[1]].pre) ^ 1;
					}
				}
				return cur;
			}

			// Returns pointer to an edge on the outer face, pointing to the vertex with minimum y-coordinate
			int recBuild(int ia, int ib, const vector<pair<T, T>>& xys, const vector<pair<T, T>>& yxs, bool f) {
				int k = ib - ia + 1;
				if (k == 2) {
					return makeEdge(inds[ia], inds[ia + 1]);
				} else if (k == 3) {
					if (xys[inds[ia + 0]] > xys[inds[ia + 1]]) swap(inds[ia + 0], inds[ia + 1]);
					if (xys[inds[ia + 1]] > xys[inds[ia + 2]]) swap(inds[ia + 1], inds[ia + 2]);
					if (xys[inds[ia + 0]] > xys[inds[ia + 1]]) swap(inds[ia + 0], inds[ia + 1]);

					int i = makeEdge(inds[ia], inds[ia + 1]);
					makeEdge(inds[ia + 1], inds[ia + 2]);
					weld(i, i+2);

					int turn = leftTurn(points[inds[ia + 2]] - points[inds[ia]], points[inds[ia + 1]] - points[inds[ia]]);
					if (turn != 0) {
						makeEdge(inds[ia + 2], inds[ia]);
						weld(i+2, i+4);
						weld(i+4, i);
					}
					return (turn == -1 ? (i ^ 1) : i);
				} else {
					int h = k/2;
					quickSelect(ia, ib, inds, (f ? yxs : xys), h);
					int le = advance(recBuild(ia, ia + h-1, xys, yxs, f ^ 1), f^1, f);
					int ri = advance(recBuild(ia + h, ib, xys, yxs, f ^ 1), -(f^1), -f);

					// Back down as far as possible
					while(true) {
						Point a = points[edges[le].v];
						Point b = points[edges[ri].v];
						Point a2 = points[edges[edges[le].pre].v];
						Point b2 = points[edges[edges[ri].nxt].v];
						if (leftTurn(b2 - a, b - a) == -1) ri = edges[ri].nxt;
						else if (leftTurn(a2 - b, a - b) == 1) le = edges[le].pre;
						else break;
					}

					return merge(le, ri);
				}
			}
		public:
			Delaunay(const vector<Point>& pts) : points(pts), inds(pts.size()) {
				int n = points.size();
				vector<pair<T, T>> xys(n), yxs(n);
				for (int i = 0; i < n; ++i) {
					inds[i] = i;
					xys[i] = {points[i].x, points[i].y};
					yxs[i] = {points[i].y, points[i].x};
				}
				recBuild(0, n-1, xys, yxs, 1);
			}
			vector<array<int, 3>> getTriangulation() const {
				vector<array<int, 3>> res;
				for (int i0 = 0; i0 < edges.size(); ++i0) {
					if (edges[i0].v == -1) continue; // Deleted edge
					int i1 = edges[i0].nxt;
					int i2 = edges[i1].nxt;
					Point p0 = points[edges[i0].v];
					Point p1 = points[edges[i1].v];
					Point p2 = points[edges[i2].v];
					if (min(i1, i2) <= i0 || leftTurn(p2 - p0, p1 - p0) >= 0) continue;
					res.push_back({edges[i0].v, edges[i2].v, edges[i1].v});
				}
				return res;
			}
			vector<vector<int>> getConns() const {
				vector<vector<int>> res(points.size());
				for (int i = 0; i < (int)edges.size(); ++i) {
					if (edges[i].v != -1) res[edges[i ^ 1].v].push_back(edges[i].v);
				}
				return res;
			}
	};
};

// Geom::Point, Geom::Delaunay etc
// using Geom = Geometry<GeomDouble<ld>, GeomDouble<ld>>;
using Geom = Geometry<ll, lll>;
istream& operator>>(istream& s, Geom::Point& p) { return s >> p.x >> p.y; }
ostream& operator<<(ostream& s, const Geom::Point& p) { return s << "(" << p.x << ", " << p.y << ")"; }

const int H = 1e9;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n;
	cin >> n;
	vector<Geom::Point> pts(n);
	for (int i = 0; i < n; ++i) cin >> pts[i];
	
	Geom::Delaunay dl(pts);

	vector<vector<int>> g = dl.getConns();

	cout << fixed << setprecision(3);
	cout << "graph G {\n";
	for (int i = 0; i < n; ++i) cout << "\t" << i << " [pos=\"" << (double)pts[i].x << "," << (double) pts[i].y << "!\"];\n";
	for (int i = 0; i < n; ++i) {
		sort(g[i].begin(), g[i].end());
		for (int t : g[i]) {
			if (t > i) cout << "\t" << i << " -- " << t << '\n';
		}
	}
	cout << "}\n";
}
