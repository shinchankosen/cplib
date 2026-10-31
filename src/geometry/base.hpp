// https://github.com/shinchankosen/cplib/blob/main/src/geometry/base.hpp
using Point = complex<double>;
using Line = vector<Point>;

#define X real()
#define Y imag()
const double EPS = 1e-10;

inline double dot(const Point& a, const Point& b) { return a.X * b.X + a.Y * b.Y; }
inline double cross(const Point& a, const Point& b) { return a.X * b.Y - a.Y * b.X; }
inline double abs(const Point& a) { return sqrt(dot(a, a)); }
