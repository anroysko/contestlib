template<class T> // Float type
class Simplex {
	private:
		// "Variable" 0 is the constant 1
		// "Variable" n+1 is the initialisation slack variable
		// "Clause" 0 is the objective function

		int n, m;
		bool inited = 0;
		vector<vector<T>> mat;
		vector<int> basic, nonbasic, rev;

		void pivot(int px, int py) {
			T div = -mat[py][px];
			mat[py][px] = -1;
			for (int x = 0; x < n; ++x) mat[py][x] /= div;
			
			for (int y = 0; y < m; ++y) {
				if (y == py) continue;

				T mult = mat[y][px];
				mat[y][px] = 0;
				for (int x = 0; x < n; ++x) mat[y][x] += mat[py][x] * mult;
			}

			swap(basic[px], nonbasic[py]);
			if (basic[px] < n) rev[basic[px]] = px;
			if (nonbasic[py] < n) rev[nonbasic[py]] = n + py;
		}
		
		int findPivot(int px) {
			pair<T, int> lim = {INF, -1};
			for (int y = 1; y < m; ++y) {
				if (mat[y][px] < -EPS) {
					T rat = mat[y][0] / -mat[y][px];
					lim = min(lim, {rat, y});
				}
			}
			return lim.second;
		}

		T optimize() {
			while(true) {
				/*
				cerr << "optimizing...\n";
				printState();
				*/

				bool found = 0;
				for (int x = 1; x < n && !found; ++x) {
					if (mat[0][x] > EPS) {
						int y = findPivot(x);
						if (y == -1) return INF; // Unbounded

						// cerr << "pivot " << x << ' ' << y << '\n';
						pivot(x, y);
						found = true;
					}
				}
				if (! found) return mat[0][0];
			}
		}
	public:
		const T EPS = 1e-9;
		const T INF = numeric_limits<T>::infinity();

		Simplex(int var_count) : n(var_count + 2), m(1) {
			nonbasic = {-1};
			basic.resize(n);
			for (int i = 0; i < n; ++i) basic[i] = i;
			rev = basic;

			// Objective for feasibility check
			mat.emplace_back(vector<T>(n, 0));
			mat[0][n-1] = -1;
		}

		// Add clause \sum_{i = 0}^{n-1} a[i] x[i] <= k
		void addClause(const vector<T>& a, T k) {
			mat.emplace_back(vector<T>(n));
			for (int i = 0; i < a.size(); ++i) mat[m][i+1] = -a[i];
			mat[m][0] = k;
			mat[m][n-1] = 1;

			nonbasic.emplace_back(n + m);
			++m;
		}

		/*
		void printState() {
			cerr << "state: \n";
			for (int x = 0; x < n; ++x) cerr << "\t" << basic[x]; cerr << '\n';
			for (int y = 0; y < m; ++y) {
				cerr << nonbasic[y];
				for (int x = 0; x < n; ++x) cerr << "\t" << mat[y][x]; cerr << '\n';
			}
		}
		*/

		// Returns TRUE iff the LP is feasible
		bool init() {
			inited = true;

			pair<T, int> lim = {0, -1};
			for (int y = 1; y < m; ++y) lim = min(lim, {mat[y][0], y});
			if (lim.first < -EPS) {
				pivot(n-1, lim.second);
				T res = optimize();
				if (res < -EPS) return false;
			}

			// Delete the variable
			if (rev[n-1] < n) {
				for (int y = 0; y < m; ++y) mat[y][rev[n-1]] = 0;
			} else {
				for (int x = 0; x < n; ++x) mat[rev[n-1] - n][x] = 0;
			}
			return true;
		}

		// MAXIMIZE \sum_{i = 0}^{n-1} x[i] obj[i]
		// Returns {max value, optimal x}
		pair<T, vector<T>> solve(const vector<T>& obj) {
			if (! inited) {
				bool fail = init();
				if (! fail) return {-INF, {}}; // Infeasible
			}

			// Set objective function
			for (int x = 0; x < n; ++x) mat[0][x] = 0;
			for (int i = 0; i < obj.size(); ++i) {
				if (rev[i+1] < n) mat[0][rev[i+1]] += obj[i];
				else {
					int y = rev[i+1] - n;
					for (int x = 0; x < n; ++x) mat[0][x] += mat[y][x] * obj[i];
				}
			}
		
			
			// Solve
			T res = optimize();
			if (res >= INF) return {INF, {}}; // Unbounded

			// Construct
			vector<T> ans(n-2);
			for (int i = 0; i < obj.size(); ++i) {
				if (rev[i+1] < n) ans[i] = 0;
				else ans[i] = mat[rev[i+1] - n][0];
			}
			return {res, ans};
		}
};
