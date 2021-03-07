
////////////////
// Predicates //
////////////////

array<lll, 3> lift(const Point& p) {
	return {p.x, p.y, (lll)p.x * p.x + p.y * p.y};
}

// Is x inside/on/outside (1/0/-1) the circumcircle of triangle abc?
int inCircle(const Point& x, const Point& a, const Point& b, const Point& c) {
	auto va = lift(a - x), vb = lift(b - x), vc = lift(c - x);
	lll res = 0;
	res += va[0] * (vb[1] * vc[2] - vb[2] * vc[1]);
	res += va[1] * (vb[2] * vc[0] - vb[0] * vc[2]);
	res += va[2] * (vb[0] * vc[1] - vb[1] * vc[0]);
	return sign(res);
}
