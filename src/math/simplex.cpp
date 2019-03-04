#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
using namespace std;

// Simplex<float>, Simplex<double> or Simplex<long double>
template<class T>
class Simplex {
	const T EPS = 1e-9;

	int n, m;
	vector<vector<T>> ineq;
	vector<int> basic;
	vector<int> nonbasic;

	/*
	void printState() {
		cerr << "\tState: " << n << ' ' << m << endl;
		cerr << "\t\t";
		for (int i = 0; i <= n; ++i) cerr << nonbasic[i] << '\t'; cerr << endl;
		for (int j = 0; j < m; ++j) {
			cerr << "\t" << j << " " << basic[j] << ":\t";
			for (int i = 0; i <= n; ++i) cerr << ineq[j][i] << '\t';
			cerr << endl;
		}
	}
	*/

	void pivot(int pi, int pj) {
		swap(basic[pj], nonbasic[pi]);

		T mult = -1 / ineq[pj][pi];
		for (int i = 0; i <= n; ++i) ineq[pj][i] *= mult;
		ineq[pj][pi] = -mult;

		for (int j = 0; j < m; ++j) {
			if (j == pj) continue;

			mult = ineq[j][pi];
			ineq[j][pi] = 0;
			for (int i = 0; i <= n; ++i) {
				ineq[j][i] += ineq[pj][i] * mult;
			}
		}
	}
	
	int findPivot(int i) {
		int pj = -1;
		for (int j = 0; j+1 < m; ++j) {
			// x  <= -b/a
			if (ineq[j][i] >= 0) continue;
			if (pj == -1 || (ineq[j][0] * ineq[pj][i] > ineq[pj][0] * ineq[j][i])) {
				pj = j;
			}
		}
		return pj;
	}

	bool optimize() {
		while(true) {
			int pi = -1;
			for (int i = 1; i <= n; ++i) {
				if (ineq[m-1][i] <= 0) continue;
				if (pi != -1 && (nonbasic[pi] < nonbasic[i])) continue;
				pi = i;
			}
			if (pi == -1) return true;
			int pj = findPivot(pi);
			if (pj == -1) return false;
			pivot(pi, pj);
		}
		return true;
	}

	public:
	Simplex(int vars) : n(vars), m(0), nonbasic(vars + 1) {
		for (int i = 0; i <= n; ++i) nonbasic[i] = i-1;
	}

	// \sum_{i = 0}^{n-1} x_{i} cl_{i} \leq c
	void addClause(const vector<T>& cl, T c) {
		ineq.emplace_back(n+1);
		ineq.back()[0] = c;
		for (int i = 0; i < n; ++i) ineq.back()[i+1] = -cl[i];

		basic.push_back(n + m);
		++m;
	}

	tuple<bool, T, vector<T>> solve(const vector<T>& obj) {
		// Add the objective function
		ineq.emplace_back(n+1, 0);
		for (int i = 1; i <= n; ++i) {
			int ind = nonbasic[i];
			if (ind < n) {
				ineq.back()[i] += obj[ind];
			}
		}
		for (int j = 0; j < m; ++j) {
			int ind = basic[j];
			if (ind < n) {
				for (int i = 0; i <= n; ++i) {
					ineq.back()[i] += ineq[j][i] * obj[ind];
				}
			}
		}
		++m;
	
		// Solve
		bool bounded = optimize();
		if (! bounded) return {false, 0, {}};
		T res = ineq[m-1][0];
	
		// Find values of variables
		vector<T> vals(n, 0);
		for (int j = 0; j < m; ++j) {
			int ind = basic[j];
			if (ind < n) vals[ind] = ineq[j][0];
		}
	
		// Remove the clause
		ineq.pop_back();
		--m;
	
		return {true, res, vals};
	}

	// Returns true if the LP is feasible
	bool init() {
		// Add objective function
		vector<T> obj(n, 0);
		addClause(obj, 0);

		// Add slack variable
		++n;
		nonbasic.push_back(-2);
		for (int j = 0; j < m; ++j) {
			ineq[j].push_back(-1);
		}
		
		// Solve
		int pj = findPivot(n);
		if (pj != -1 && ineq[pj][0] < 0) {
			pivot(n, findPivot(n));
			optimize();
		}

		// Remove objective function
		if (ineq[m-1][0] < -EPS) return false;
		ineq.pop_back();
		basic.pop_back();
		--m;

		// Pivot slack variable to a nonbasic variable
		for (int j = 0; j < m; ++j) {
			if (basic[j] == -2) {
				pivot(n, j);
				break;
			}
		}

		// Swap slack variable to be the last variable
		for (int i = 0; i < n; ++i) {
			if (nonbasic[i] == -2) {
				swap(nonbasic[i], nonbasic[n]);
				for (int j = 0; j < m; ++j) {
					swap(ineq[j][i], ineq[j][n]);
				}
			}
		}

		// Remove slack variable
		--n;
		nonbasic.pop_back();
		for (int j = 0; j < m; ++j) {
			ineq[j].pop_back();
		}

		return true;
	}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;
	
	Simplex<double> si(n);
	for (int j = 0; j < m; ++j) {
		vector<double> cl(n);
		for (int i = 0; i < n; ++i) cin >> cl[i];

		char tmp;
		cin >> tmp;
		cin >> tmp;

		double c;
		cin >> c;

		si.addClause(cl, c);
	}


	bool feasible = si.init();
	if (! feasible) {
		cout << "infeasible\n";
	} else {
		vector<double> obj(n);
		for (int i = 0; i < n; ++i) cin >> obj[i];

		bool bounded;
		double res;
		vector<double> vars;
		tie(bounded, res, vars) = si.solve(obj);

		if (! bounded) {
			cout << "unbounded\n";
		} else {
			cout << res << '\n';
			for (int i = 0; i < n; ++i) cout << vars[i] << ' ';
			cout << '\n';
		}
	}
}
