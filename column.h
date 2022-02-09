#ifndef COLUMN_H
#define COLUMN_H

#include "object.h"

class Column : public Object
{
public:
    Column();
    Column(std::vector<Point3D>, Color);

};

#endif // COLUMN_H
