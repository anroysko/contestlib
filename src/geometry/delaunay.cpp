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
template<class T, class TL>
istream& operator>>(istream& s, Point<T, TL>& p) { return s >> p.x >> p.y; }

template<class T, class TL>
ostream& operator<<(ostream& s, const Point<T, TL>& p) { return s << "(" << p.x << ", " << p.y << ")"; }

template<class T, class TL>
TL dot(const Point<T, TL>& a, const Point<T, TL>& b) { return (TL)a.x * b.x + (TL)a.y * b.y; }

template<class T, class TL>
TL cross(const Point<T, TL>& a, const Point<T, TL>& b) { return (TL)a.x * b.y - (TL)a.y * b.x; }


// Does vector 0 -> a turn LEFT of vector 0 -> b (1 left, 0 parallel, -1 right)
template<class T, class TL>
int leftTurn(const Point<T, TL>& a, const Point<T, TL>& b) {
	return -sign(cross(a, b));
}

// Is point p in triangle t? (1 strictly in, 0 in, -1 out)
template<class T, class TL>
int contains(const array<Point<T, TL>, 3>& t, const Point<T, TL>& p) {
	int a = leftTurn(p - t[0], t[1] - t[0]);
	int b = leftTurn(p - t[1], t[2] - t[1]);
	int c = leftTurn(p - t[2], t[0] - t[2]);

	int mi = min(a, b, c);
	int ma = max(a, b, c);
	if (mi == -ma) return -1;
	else return (mi != 0);
}

// Are points t in convex position? (1 strictly convex, 0 convex, -1 not convex)
template<class T, class TL>
int convPos(const array<Point<T, TL>, 4>& t) {
	return leftTurn(t[2] - t[0], t[1] - t[0]) * leftTurn(t[3] - t[0], t[1] - t[0])
		* leftTurn(t[0] - t[2], t[3] - t[2]) * leftTurn(t[1] - t[2], t[3] - t[2]);
}

// Is point p in the circumcircle of triangle t? (1 strictly in, 0 in, -1 out)
// Overflows if (|p| + |a| + |b| + |c|)^4 overflows TL
template<class T, class TL>
array<TL, 3> lift(const Point<T, TL>& p) {
	return {p.x, p.y, p.sqLength()};
}

template<class T, class TL>
int inCirc(const array<Point<T, TL>, 3>& t, const Point<T, TL>& p) {
	auto la = lift(t[0] - p), lb = lift(t[1] - p), lc = lift(t[2] - p);
	if (leftTurn(t[2] - t[0], t[1] - t[0]) == 1) swap(lb, lc);

	TL res = 0;
	res += la[0] * (lb[1] * lc[2] - lb[2] * lc[1]);
	res += la[1] * (lb[2] * lc[0] - lb[0] * lc[2]);
	res += la[2] * (lb[0] * lc[1] - lb[1] * lc[0]);
	return -sign(res);
}

template<class T, class TL> // long long x __int128 or double_type x double_type
class Delaunay {
	private:
		struct HalfEdge {
			int nxt;
			int vertex;
			int hist_ind;

			HalfEdge() : nxt(-1), vertex(-1), hist_ind(-1) {}
			HalfEdge(int n, int v, int h) : nxt(n), vertex(v), hist_ind(h) {}
		};
		struct HistoryNode {
			int edge_ind;
			int new_vertex = -1;
			array<int, 3> child = {-1, -1, -1};

			HistoryNode(int ei) : edge_ind(ei) {}
		};

		vector<Point<T, TL>> points;
		vector<HalfEdge> edges; // Half-edge data structure
		vector<HistoryNode> hg; // History graph

		// Find next triangle in history graph
		pair<int, array<int, 3>> advance(Point<T, TL> p, array<int, 3> cur, const HistoryNode& node) const {
			array<Point<T, TL>, 3> t = {points[cur[0]], points[cur[1]], points[cur[2]]};
			Point<T, TL> new_point = points[node.new_vertex];

			for (int j = 0; j < 3; ++j) {
				if (node.child[j] == -1) continue;
				swap(new_point, t[j]);

				if (contains(t, p) >= 0) {
					cur[j] = node.new_vertex;
					sort(cur.begin(), cur.end());
					return {node.child[j], cur};
				}
				swap(new_point, t[j]);
			}
			return {-1, cur};
		}

		// Find terminal history graph triangle containing point
		int locate(Point<T, TL> p) const {
			int hi = 0;
			array<int, 3> cur = {0, 1, 2};
			while(true) {
				auto nxt = advance(p, cur, hg[hi]);
				if (nxt.first == -1) break;
				else tie(hi, cur) = nxt;
			}
			return hi;
		}

		// Makes the history node nonterminal
		void splitNode(int ind, int new_v, int v1, int c1, int v2, int c2, int v3, int c3) {
			hg[ind].edge_ind = -1;
			hg[ind].new_vertex = new_v;

			array<pair<int, int>, 3> arr;
			arr[0] = {v1, c1};
			arr[1] = {v2, c2};
			arr[2] = {v3, c3};
			sort(arr.begin(), arr.end());
			for (int j = 0; j < 3; ++j) hg[ind].child[j] = arr[j].second;
		}

		// Split history graph triangle hi in three, by adding edges to point pi
		void splitTriangle(int hi, int pi) {
			array<int, 3> te;
			te[0] = hg[hi].edge_ind;
			for (int ind = edges[te[0]].nxt; ind != te[0]; ind = edges[ind].nxt) {
				if (edges[ind].vertex < edges[te[0]].vertex) te[0] = ind;
			}
			te[1] = edges[te[0]].nxt;
			te[2] = edges[te[1]].nxt;

			int es = edges.size();
			int hs = hg.size();
			edges.resize(es + 6);
			for (int j = 0; j < 3; ++j) {
				// Create new face without vertex j
				int ei = te[(j+2) % 3];
				edges[ei].hist_ind = hs + j;
				edges[ei].nxt = es + 2*j;
				edges[es + 2*j] = {es + (2*j + 5) % 6, pi, hs + j};
				edges[es + (2*j + 5) % 6] = {ei, edges[te[(j+1) % 3]].vertex, hs + j};

				hg.emplace_back(ei);
			}
			splitNode(hi, pi, edges[te[0]].vertex, hs, edges[te[1]].vertex, hs + 1, edges[te[2]].vertex, hs + 2);
		}

		// Try to perform a Lawson flip on edge e0
		bool lawsonFlip(int e0) {
			if (e0 < 3) return false; // Edge on outer face

			// Check that the points are in convex position
			array<int, 3> e = {e0, edges[e0].nxt, edges[edges[e0].nxt].nxt};
			array<int, 3> f = {e0 ^ 1, edges[e0 ^ 1].nxt, edges[edges[e0 ^ 1].nxt].nxt};

			int va1 = edges[e[0]].vertex, va2 = edges[f[0]].vertex;
			int vb1 = edges[e[1]].vertex, vb2 = edges[f[1]].vertex;
			auto pa1 = points[va1], pa2 = points[va2];
			auto pb1 = points[vb1], pb2 = points[vb2];

			int cp = convPos<T, TL>({pa1, pa2, pb1, pb2});

			if (cp == -1) {
				return false;
			} else if (cp == 0) {
				if (leftTurn(pb1 - pa1, pa2 - pa1) != 0) return false;
			} else {
				if (va1 >= 3 && va2 >= 3 && (vb2 < 3
					|| inCirc({pa1, pa2, pb1}, pb2) < 1)) return false; // Flipping to a corner / edge not bad
			}

			int hs = hg.size();
			hg.emplace_back(e[0]);
			hg.emplace_back(f[0]);
			splitNode(edges[e[0]].hist_ind, vb2, va2, hs + 1, va1, hs, vb1, -1);
			splitNode(edges[f[0]].hist_ind, vb1, va2, hs + 1, va1, hs, vb2, -1);

			edges[e[0]].vertex = vb1;
			edges[f[0]].vertex = vb2;
			for (int j = 0; j < 3; ++j) {
				edges[e[j]].nxt = (j == 0 ? e : f)[(j + 2) % 3];
				edges[f[j]].nxt = (j == 0 ? f : e)[(j + 2) % 3];
				edges[e[j]].hist_ind = hs + (j == 1);
				edges[f[j]].hist_ind = hs + (1 - (j == 1));
			}
			return true;
		}

		// Adds a point p to the triangulation
		void addPoint(int pi) {
			int hi = locate(points[pi]);
			int ei = hg[hi].edge_ind;

			int e0 = hg[hi].edge_ind;
			int e1 = edges[e0].nxt;
			int e2 = edges[e1].nxt;
			vector<int> que = {e0, e1, e2};

			splitTriangle(hi, pi);

			for (int j = 0; j < que.size(); ++j) {
				if (edges[edges[que[j]].nxt].vertex != pi) continue;
				int i1 = edges[que[j] ^ 1].nxt;
				int i2 = edges[i1].nxt;

				if (lawsonFlip(que[j])) {
					que.push_back(i1);
					que.push_back(i2);
				}
			}
		}
	public:
		vector<vector<int>> triangulation() const {
			vector<vector<int>> g((int)points.size() - 3);
			for (int e = 0; e < edges.size(); ++e) {
				int a = edges[e].vertex;
				if (a < 3) continue;

				int b = edges[edges[e].nxt].vertex;
				if (b < 3) continue;

				g[a-3].push_back(b-3);
			}
			return g;
		}

		Delaunay(vector<Point<T, TL>> pts) {
			edges.emplace_back(1, 0, 0);
			edges.emplace_back(2, 1, 0);
			edges.emplace_back(0, 2, 0);
			edges.emplace_back();
			hg.emplace_back(0);

			T mx = 2;
			for (auto p : pts) mx = max(mx, abs(p.x), abs(p.y));
			points.emplace_back(-3*mx, -3*mx);
			points.emplace_back(-mx/2, 3*mx);
			points.emplace_back(3*mx, -mx/2);

			for (auto p : pts) points.push_back(p);

			vector<int> ord(pts.size());
			for (int i = 0; i < pts.size(); ++i) ord[i] = i;
			shuffle(ord.begin(), ord.end(), rng);

			for (auto i : ord) addPoint(i + 3);
		}

};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n;
	cin >> n;
	vector<Point<ll, lll>> pts(n);
	for (int i = 0; i < n; ++i) cin >> pts[i];
	
	Delaunay<ll, lll> dl(pts);
	vector<vector<int>> g = dl.triangulation();

	cout << g.size() << '\n';

	cout << "graph G {\n";
	for (int i = 0; i < n; ++i) cout << "\t" << i << " [pos=\"" << pts[i].x << "," << pts[i].y << "!\"];\n";
	for (int i = 0; i < n; ++i) {
		sort(g[i].begin(), g[i].end());
		for (int t : g[i]) {
			if (t > i) cout << "\t" << i << " -- " << t << '\n';
		}
	}
	cout << "}\n";
}
