#ifndef SLAB_H
#define SLAB_H

#include "object.h"

class Slab : public Object
{
public:
    Slab();
    Slab(std::vector<Point3D>, Color);
};

#endif // SLAB_H
