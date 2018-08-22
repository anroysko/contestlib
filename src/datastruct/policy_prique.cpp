#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
using namespace std;

// Pairing Heap (https://gcc.gnu.org/onlinedocs/libstdc++/ext/pb_ds/)
// O(1) insert, join, O(log(n)) other operations.
// Faster than regular priority queue for non-primitive types (string, vector), or if join is required.
// Other advantages include being able to iterate through it, and having smallest element first by default.
template<class T>
using pair_heap = __gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::pairing_heap_tag>;

int main() {
	int a, b, c;
	cin >> a >> b >> c;
	
	pair_heap<int> que1;
	pair_heap<int>::point_iterator it = que1.push(a);
	que1.push(b);
	cout << que1.top() << '\n';

	// Can modify elements inside in O(log n) time
	que1.modify(it, c);
	cout << que1.top() << '\n';

	int d, e;
	cin >> d >> e;
	
	pair_heap<int> que2;
	que2.push(d);
	que2.push(e);

	// Supports joining in O(1) time	
	que1.join(que2);

	// Can be iterated (elements aren't in order though)
	for (auto it : que1) cout << it << ' '; cout << '\n';	

	while(! que1.empty()) {
		cout << que1.top() << ' ';
		que1.pop();
	}
	cout << '\n';
}
