pair<ll, ll> extEucMod(ll a, ll b, ll p) {
	if (b == 0) return {1, 0};
	ll m = a / b;
	auto sub = extEucMod(b, a - b * m, p);
	return {sub.second, (sub.first - m*sub.second) % p};
}
ll modInv(ll a, ll p) {
	ll res = extEucMod(p, a, p).second;
	return (res < 0 ? res + p : res);
}
ll gcd(ll a, ll b) { return (b == 0 ? a : gcd(b, a % b)); }
ll mSub(ll a, ll b, ll m) { return (a >= b ? a - b : a - b + m); }
ll posMod(ll a, ll m) { ll res = a % m; return (res < 0 ? res + m : res); }
ll getSteps(ll t, ll ia, ll b, ll m) { return mSub(t, b, m) * ia % m; }

// Returns minimum value of ax + b (mod m) for x \in [0, k]. O(log m) time
ll minRem(ll a, ll b, ll m, ll k) {
	for (ll g = gcd(a, m); g > 1;) return g * minRem(a/g, b/g, m/g, k) + (b % g);
	for (ll b0 = b, m0 = m, ia0 = modInv(a, m), na, nb, nm; a; a = na, b = nb, m = nm) {
		if (a > m - a) {
			na = a % (m - a);
			nb = b % (m - a);
			nm = m - a;
			for (ll steps = getSteps(nb, ia0, b0, m0); steps > k;) {
				ll add = steps - getSteps(nb + nm, ia0, b0, m0);
				return nb + nm * ((steps - k + (add - 1)) / add);
			}
		} else {
			na = posMod(-m, a);
			nb = (b < a ? b : posMod(b - m, a));
			nm = a;
			if (getSteps(nb, ia0, b0, m0) > k) break;
		}
	}
	return b;
}

// Returns minimum x such that ax + b (mod m) \in [le, ri] or -1 if there is no such x. O(log m) time
ll firstInRange(ll a, ll b, ll m, ll le, ll ri) {
	for (ll g = gcd(a, m); g > 1;) return firstInRange(a/g, b/g, m/g, le/g + (le % g > b % g), ri/g - (ri % g < b % g));
	if (le > ri) return -1; // impossible
	ll ia = modInv(a, m);
	return minRem(ia, mSub(le, b, m) * ia % m, m, ri - le);
}
