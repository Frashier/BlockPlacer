#ifndef WALL_H
#define WALL_H

#include "object.h"

using Pixel = std::pair<Point2D, Color>;

class Wall : public Object
{
public:
    Wall();
    Wall(std::vector<Point3D>, Color, int);
};

#endif // WALL_H
