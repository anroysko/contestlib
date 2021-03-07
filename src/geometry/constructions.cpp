        // Returns the unique intersection point of non-parallel lines s1, t1 and s2, t2
        static Point intersect(const Point& s1, const Point& t1, const Point& s2, const Point& t2) {
                static_assert(! is_integral<T>::value && ! is_integral<TL>::value); // Constructions need reals!
                TL d = cross(t1 - s1, t2 - s2); // 0 if parallel
                TL p = cross(t1 - s1, s2 - s1);
                TL q = cross(t2 - s2, s1 - s2);
                return (s1 * p + t1 * q) / d;
        }

        // Center of circumcircle of triangle abc
        static Point circumcenter(const Point& a, const Point& b, const Point& c) {
                static_assert(! is_integral<T>::value && ! is_integral<TL>::value); // Constructions need reals!

                TL m = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
                TL bx = -(a.sqLength() * (b.y - c.y) + b.sqLength() * (c.y - a.y) + c.sqLength() * (a.y - b.y));
                TL by = (a.sqLength() * (b.x - c.x) + b.sqLength() * (c.x - a.x) + c.sqLength() * (a.x - b.x));
                return {bx / m, by / m};
        }

        // Radius of circumcircle of triangle abc
        static TL circumradius(const Point& a, const Point& b, const Point& c) {
                static_assert(! is_integral<T>::value && ! is_integral<TL>::value); // Constructions need reals!

                TL div = 2 * cross(b - a, c-a);
                return sqrt((b - a).sqDist() * (c - b).sqDist() * (a - c).sqDist() / (div*div));
        }

