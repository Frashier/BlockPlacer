#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include "point2d.h"
#include "point3d.h"
#include "face.h"
#include "objectinfo.h"

using Pixel = std::pair<Point2D, Color>;
using zbufferVector = std::vector<std::vector<std::pair<int, int>>>;

// Class for each object displayed on the scene
class Object
{
public:
    std::vector<Point3D> vertices;                  // Collection of vertices
    std::vector<Face> faces;                        // Collection of faces
    bool test = false;
    ObjectInfo info;

    Object();

    virtual std::vector<Pixel> render(Point3D, zbufferVector&, int);

protected:
    int minCoordValue(std::vector<Point3D>, char);
    int maxCoordValue(std::vector<Point3D>, char);
    virtual std::vector<Face> convertToFaces(std::vector<Point3D>);
};

#endif // OBJECT_H
