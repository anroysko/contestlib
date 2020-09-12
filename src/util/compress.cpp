// returns number of elements strictly smaller than v in vec
template<class T>
int bins(const vector<T>& vec, T v) {
	int low = 0;
	int high = vec.size();
	while(low != high) {
		int mid = (low + high) >> 1;
		if (vec[mid] < v) low = mid + 1;
		else high = mid;
	}
	return low;
}

template<class T>
pair<vector<int>, vector<T>> compress(const vector<T>& vec) {
	vector<T> comp = vec;
	sort(comp.begin(), comp.end());
	comp.erase(unique(comp.begin(), comp.end()), comp.end());

	vector<int> res(comp.size());
	for (int i = 0; i < comp.size(); ++i) res[i] = bins(comp, vec[i]);
	return {res, comp};
}
