#include <iostream>
using namespace std;

template <typename T> struct Point { T x, y; };

template <typename U> struct Triangle {
    Point<U> pts[3];
    U double_square() const;
};

//method
template <typename K>
K Triangle<K>::double_square() const
{
    K sq = pts[0].x * (pts[1].y - pts[2].y) + 
           pts[1].x * (pts[2].y - pts[0].y) + 
           pts[2].x * (pts[0].y - pts[1].y);
 
    return (sq > 0) ? sq : -sq;
}

int main()
{
    Triangle<double> t;
    t.pts[0] = Point<double> {0, 0}; // запись поля
    t.pts[1] = Point<double> {3.0, 0.0};
    t.pts[2] = Point<double> {3.0, 3.0};

    double a = t.double_square();

    cout << "Answer: " << a << "\n";

    return 0;
}
