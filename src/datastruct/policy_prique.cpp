#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
using namespace std;

// Pairing Heap
// O(1) insert, join, O(log(n)) other operations.
// Faster than regular priority queue for non-primitive types (string, vector), or if join is required.
template<class T>
using pairing_heap = __gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::pairing_heap_tag>;

// thin heap (better fib heap)
// O(1) insert, join, reduce_key, O(log(n)) pop.
// Speeds up djikstra if m = O(n^2) (maybe?)
template<class T>
using fib_heap = __gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::thin_heap_tag>;

int main() {
	int a, b, c;
	cin >> a >> b >> c;
	
	fib_heap<int> que1;
	fib_heap<int>::point_iterator it = que1.push(a);
	que1.push(b);
	cout << que1.top() << '\n';
	que1.modify(it, c); // fast if c < previous value
	cout << que1.top() << '\n';

	int d, e;
	cin >> d >> e;
	
	fib_heap<int> que2;
	que2.push(d);
	que2.push(e);
	
	que1.join(que2);

	for (auto it : que1) cout << it << ' '; cout << '\n';	

	while(! que1.empty()) {
		cout << que1.top() << ' ';
		que1.pop();
	}
	cout << '\n';
}
