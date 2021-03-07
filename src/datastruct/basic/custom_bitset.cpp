#include <iostream>
#include <vector>
using namespace std;
using ull = unsigned long long;

// Assumes that all operations are made between two bitsets with equal number of bits
class BitSet {
	private:
	constexpr static ull LM = (1ull<<32) - 1;
	constexpr static ull FM = ~0ull;
	vector<ull> bits;
	int n;

	void clearHighBits() {
		if (n & 63) bits.back() &= ~(FM << (n & 63));
	}

	public:
	BitSet(int bc) : n(bc), bits((bc+63)/64, 0) {}

	bool get(int i) {
		return vec[i>>6] & (1ull << (i & 63));
	}
	void set(int i, ull v) {
		int j = i >> 6;
		int s = i & 63;
		vec[j] = vec[j] & ~(1ull << s) | (v << s);
	}
	int count() const {
		int res = 0;
		for (int i = 0; i < bits.size(); ++i) {
			res += __builtin_popcountll(bits[i]);
		}
		return res;
	}
	bool operator==(const BitSet& rhs) const {
		for (int i = 0; i < bits.size(); ++i) {
			if (bits[i] != rhs.bits[i]) return false;
		}
		return true;
	}
	bool operator<(const BitSet& rhs) const {
		for (int i = 0; i < bits.size(); ++i) {
			if (bits[i] != rhs.bits[i]) return bits[i] < rhs.bits[i];
		}
		return false;
	}

	BitSet operator~() const {
		BitSet res(n);
		for (int i = 0; i < bits.size(); ++i) res.bits[i] = ~bits[i];
		res.clearHighBits();
		return res;
	}
	BitSet operator<<(int d) {
		BitSet res(n);
		int j = d >> 6;
		int s = d & 63;
		// LOW mask of s bits
		for (int i = 0; i+j+1 < bits.size(); ++i) {

		}
			res[i+j+1] |= bits[i] >> (64 - s); // low mask
		for (int i = 0; i+j < bits.size(); ++i) {
			res[i+j] |= bits[i] << s; // high mask
		}
	}
	BitSet operator>>(int d) {}
	BitSet operator|(const BitSet& rhs) const {
		BitSet res(n);
		for (int i = 0; i < bits.size(); ++i) res.bits[i] = bits[i] | rhs.bits[i];
		return res;
	}
	BitSet operator&(const BitSet& rhs) const {
		BitSet res(n);
		for (int i = 0; i < bits.size(); ++i) res.bits[i] = bits[i] & rhs.bits[i];
		return res;
	}
	BitSet operator^(const BitSet& rhs) const {
		BitSet res(n);
		for (int i = 0; i < bits.size(); ++i) res.bits[i] = bits[i] ^ rhs.bits[i];
		return res;
	}
	BitSet operator+(const BitSet& rhs) const {
		
	}
	
};

int main() {
	
}
