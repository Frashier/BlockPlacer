#include "point3d.h"

Point3D::Point3D() {}

Point3D::Point3D(int x_, int y_, int z_) :
    x(x_), y(y_), z(z_) {}

bool Point3D::operator==(Point3D p)
{
    return (this->x == p.x && this->y == p.y && this->z == p.z);
}
