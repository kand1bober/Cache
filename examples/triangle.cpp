#include <iostream>
using namespace std;

struct Point { double x, y; };
struct Triangle {
    Point pts[3];
    double square() const;
};

//method 
double Triangle::square() const
{
    double sq = pts[0].x * (pts[1].y - pts[2].y) + 
                pts[1].x * (pts[2].y - pts[0].y) + 
                pts[2].x * (pts[0].y - pts[1].y);
 
    return abs(sq) / 2.0;
}

int main()
{
    Triangle t;
    t.pts[0] = Point {0, 0}; // запись поля
    t.pts[1] = Point {3.0, 0.0};
    t.pts[2] = Point {3.0, 3.0};

    double a = t.square();

    cout << "Answer: " << a << "\n";

    return 0;
}
