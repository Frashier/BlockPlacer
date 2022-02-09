#ifndef POINT2D_H
#define POINT2D_H
#include "point3d.h"

// Class representing a 2D point
class Point2D
{
public:
    int x, y;

    Point2D();
    Point2D(int, int);

    // constructor converting Point3D to 2D
    Point2D(Point3D);

    bool operator==(Point2D);
    Point2D operator+(int);
};

#endif // POINT2D_H
