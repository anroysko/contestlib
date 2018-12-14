#include <iostream>
#include <vector>
using namespace std;

// Subroutine for KMP
void kmpSub(const string& str, const string& pat, vector<int>& aux, vector<int>& res, int p) {
	for (int i = 1; i < res.size(); ++i) {
		res[i] = res[i-1];
		if (res[i] == pat.size()) res[i] = aux[res[i]];
		while((res[i] > -1) && (pat[res[i]] != str[i-p])) {
			res[i] = aux[res[i]];
		}
		++res[i];
	}
}

// Knuth-morris-pratt pattern matching
vector<int> kmp(const string& str, const string& pat) {
	int n = str.size();
	int m = pat.size();
	vector<int> aux(m+1, -1);
	vector<int> res(n, str[0] == pat[0]);
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
	// TODO
	// Example output:
	// TODO
}
