// https://arxiv.org/pdf/1902.01961.pdf
// Fast calculation of x % d for fixed d \in [1, 2^32) and any x \in [0, 2^32)
struct FastMod {
	const ull c, d;
	FastMod(uint d) : c((ull)-1 / d + 1), d(d) {}
	
	// Returns x % d
	uint operator()(uint x) { return ((__int128)(c * x) * d) >> 64; }

	// Returns (x % d) == 0
	bool zeroRem(uint x) { return c*x <= c - 1; }
};

// Lemma 1:
//	Fix N, and pick d \in [1, 2^N) and c, L s.t.
//		2^{N + L} <= cd <= 2^{N + L} + 2^{L}
//	then for every x \in [0, 2^N)
//		|c * (x mod d) - (cx mod 2^{N + L})| <= 2^L floor(n / d)
//	This is useful since x mod d is what we want to calculate
// Proof:
//	...

// Theorem 1:
//	Fix N, and pick d \in [1, 2^N) and c, L s.t.
//		2^{N + L} <= cd <= 2^{N + L} + 2^{L}
//	then for every x \in [0, 2^N)
//		x mod d = ((cx mod 2^{N + L}) * d) / 2^{N + L}
// Proof:
//	...


// Note:
//	For L = N = 32, d \in [1, 2^32) and c = (2^64 - 1) / d + 1, we have
//		2^64 <= cd <= 2^64 + 2^32

// Corollary:
//	For L = N = 32 and c = (2^64 - 1) / d + 1,
//	then for every x \in [0, 2^N)
//		1. x mod d		= ((cx mod 2^64) * d) / 2^64
//		2. ((x mod d) == 0)	= ((cx mod 2^64) <= (c - 1 mod 2^64))
// Proof:
//	1. is a direct application of theorem 1
//	For 2., note that (x mod d) == 0 IFF (cx mod 2^64) * d < 2^64
//	If (cx mod 2^64) <= (c - 1 mod 2^64), then
//		(cx mod 2^64) * d <= (c - 1 mod 2^64) * d = ((2^64 - 1) / d) * d <= 2^64 - 1 < 2^64
//	If (cx mod 2^64) >= (c - 1 mod 2^64) + 1, then
//		(cx mod 2^64) * d >= ((c - 1 mod 2^64) + 1) * d = ((2^64 - 1) / d + 1) * d = ((2^64 + (d-1)) / d) * d >= 2^64
