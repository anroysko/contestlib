#include <iostream>
#include <vector>
using namespace std;

// Lexicographical ordering for pairs and triplets.
inline bool lexLeq(int a1, int a2, int b1, int b2) {
	return (a1 < b1 || (a1 == b1 && a2 < b2));
}
inline bool lexLeq(int a1, int a2, int a3, int b1, int b2, int b3) {
	return (a1 < b1 || (a1 == b1 && lexLeq(a2, a3, b2, b3)));
}

void radixPass(const int* src, int* res, const int* vals, int n, int c) {
	int* cou = new int[c];
	memset(cou, 0, c * sizeof(int));
	for (int i = 0; i < n; ++i) ++cou[vals[src[i]]];
	for (int i = 1; i < c; ++i) cou[i] += cou[i-1];
	for (int i = n-1; i >= 0; --i) {
		int j = src[i];
		int v = vals[j];
		tar[cou[v]] = j;
		--cou[v];
	}
	delete[] cou;
}

// n > 2
// str[n] = str[n+1] = str[n+2] = 0
void recSuffixArray(const int* str, int* res, int n, int c) {
	int high = (n % 3 == 1 ? n : n-1);
	int n0 = (high+3)/3;
	int n1 = (high+2)/3;
	int n2 = (high+1)/3;
	int n12 = n1 + n2;

	int* vals = new int[n12+3];
	int* inds = new int[n12+3];
	for (int i = n12; i < n12 + 3; ++i) {
		vals[i] = 0;
		inds[i] = 0;
	}

	for (int i = 0, j = 0; i <= high; ++i) {
		if (i % 3 != 0) {
			vals[j] = i;
			++j;
		}
	}
	radixPass(vals, inds, str+2, n12, c);
	radixPass(inds, vals, str+1, n12, c);
	radixPass(vals, inds, str,   n12, c);

	int nc = 0;
	int cv0 = -1;
	int cv1 = -1;
	int cv2 = -1;
	for (int i = 0; i < n12; ++i) {
		int j = inds[i];
		if (i > 0 && (str[j] != cv0 || str[j+1] != cv1 || str[j+2] != cv2)) {
			++nc;
			cv0 = str[j];
			cv1 = str[j+1];
			cv2 = str[j+2];
		}
		if (j % 3 == 1) vals[j/3] = nc;
		else vals[j/3 + n1] = nc;
	}

	if (nc < n12) {
		// Some value appears multiple times.
		// Otherwise, All values in the suffix array are unique, no need to recurse further
		// Base case is that no value appears multiple times.
		recSuffixArray(vals, inds, n12, nc+1);
		for (int i = 0; i < n12; ++i) {
			inds[i] = (inds[i] < n1 ? 3*inds[i]+1 : 3*(inds[i]-n1)+2);
		}
	}
	
	// inds now contains suffixes with i != 0 (3) sorted in alphabetical order. Calculate rank of indices with i != 0 (3)
	int* rank = new int[n+3];
	for (int i = 0; i < n12; ++i) rank[inds[i]] = i;
	for (int i = n; i < n+3; ++i) rank[i] = -1;
	
	// Sort suffixes starting from indexes with i = 0 (3). Store the indexes in vals. Use res as a temporary array.
	for (int i = 0; i <= high; i += 3) vals[i/3] = i;
	radixPass(vals, res, rank+1, n0, n12);
	radixPass(res, vals, str, n0, c);

	// Combine vals and inds into res.
	for (int a = (inds[0] >= n), b = 0, j = 0; j < n; ++j) {
		bool pick_a;
		if (a >= n12) {
			pick_a = false;
		} else if (b >= n0) {
			pick_a = true;
		} else {
			int ia = inds[a];
			int ib = vals[b];
			if (ia % 3 == 1) pick_a = lexReq(str[ia], rank[ia+1], str[ib], rank[ib+1]);
			else pick_a = lexReq(str[ia], str[ia+1], rank[ia+2], str[ib], str[ib+1], rank[ib+2]);
		}
		if (pick_a) {
			res[j] = inds[a];
			++a;
		} else {
			res[j] = vals[b];
			++b;
		}
	}

	// Free memory
	delete[] vals;
	delete[] inds;
	delete[] rank;
}

// DC3 linear time suffix array construction algorithm (https://www.cs.helsinki.fi/u/tpkarkka/publications/jacm05-revised.pdf )
// NOT a cyclic suffix array; No need to append #. If you need a cyclic array of S, call on SS.
// vector<int> can be replaced with string.
// c is the size of the alphabet. 0 < str[i] < c must hold for all i.
// The I'th entry of the suffix array is the starting point of the I'th suffix when all of the cyclic suffixes are sorted in alphabetical order.
// Running Time: O(n + c)
vector<int> suffixArray(vector<int> str, int c) {
	int n = str.size();
	if (n == 1) return {0};
	if (n == 2) return (str[0] < str[1] ? {0, 1} : {1, 0});
	vector<int> res(n);
	str.push_back(0); str.push_back(0); str.push_back(0);
	recSuffixArray(str.data(), res.data(), n, c);
	str.pop_back(); str.pop_back(); str.pop_back();
	return res;
}

int main() {
	string str;
	cin >> str;
	int n = str.size();
	vector<int> vals(n);
	for (int i = 0; i < n; ++i) vals[i] = (str[i]-'a'+1);
	vector<int> sa = suffixArray(str, 27);
	for (int i = 0; i < n; ++i) cout << sa[i] << ' '; cout << '\n';
}
