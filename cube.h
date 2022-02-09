#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object
{
public:
    Cube();
    Cube(std::vector<Point3D>, Color);
};

#endif // CUBE_H
