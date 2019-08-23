class KMP {
	private:
		const string pat;
		vector<int> aux; // aux[i] := max_{j < i} s.t. pat[0, j) = pat[i-j, i)

		void kmpSub(const string& str, const string& pat, vector<int>& res, bool p) {
			for (int i = 0, c = -2*p; i < res.size(); ++i) {
				while(c > -1 && (c == pat.size() || pat[c] != str[i-p])) c = aux[c];
				res[i] = ++c;
			}
		}
	public:
		KMP(const string& pt) : pat(pt), aux(pt.size() + 1) {
			kmpSub(pat, pat, aux, 1);
		}
		vector<int> match(const string& str) {
			vector<int> res(str.size());
			kmpSub(str, pat, res, 0);
			return res; // res[i] := max_{j} s.t. pat[0, j) = str(i-j, i]
		}
};
