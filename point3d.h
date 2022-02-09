#ifndef POINT3D_H
#define POINT3D_H

// Class representing 3D coordinates
class Point3D
{
public:

    int
        x,
        y,
        z;

    Point3D();
    Point3D(int, int, int);

    bool operator==(Point3D);    
};

#endif // POINT3D_H
