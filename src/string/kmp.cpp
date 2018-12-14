#include <iostream>
#include <vector>
using namespace std;

// Subroutine for KMP
void kmpSub(const string& str, const string& pat, vector<int>& aux, vector<int>& res, bool phase) {
	int c = (phase ? -1 : str[0] == pat[0]); // current value
	res[0] = c;
	for (int i = 1; i < res.size(); ++i) {
		char t = str[i - phase]; // extending char
		while((c > -1) && (pat[c] != t)) c = aux[c];
		res[i] = ++c;
		if (c == pat.size()) c = aux[c];
	}
}

// Knuth-morris-pratt pattern matching
vector<int> kmp(const string& str, const string& pat) {
	vector<int> aux(pat.size()+1); // aux[i] := max_j : pat[0..j) = pat[i-j..i), and j < i
	vector<int> res(str.size());   // res[i] := max_j : pat[0..j) = str(i-j..i], and j <= i+1
	kmpSub(pat, pat, aux, aux, 1);
	kmpSub(str, pat, aux, res, 0);
	return res;
}

int main() {
	// Example usage:
	string str, pat;
	cin >> str >> pat;

	vector<int> res = kmp(str, pat);
	for (int i = 0; i < str.size(); ++i) cout << res[i] << ' '; cout << '\n';
	
	// Example input:
	// abaabbabaaba
	// babaa

	// Example output:
	// 0 1 2 0 1 1 2 3 4 5 1 2 
}
