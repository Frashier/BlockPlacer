#ifndef FACE_H
#define FACE_H

#include <vector>
#include <array>
#include "point3d.h"
#include "point2d.h"
#include "objectinfo.h"

using Pixel = std::pair<Point2D, Color>;
using zbufferVector = std::vector<std::vector<std::pair<int, int>>>;

class Face
{
public:
    Point3D vertices3D[3];                  // Assigned value only once at construction.
    Point3D transformedVertices3D[3];       // Values after transforming vertices3D.
    Point2D vertices2D[3];                  // After casting 3D to 2D.
    char facing = '0';                      // Which direction face is facing.
    bool visible = false;                   // Visibility after casting

    Face();
    Face(Point3D, Point3D, Point3D, char);

    // Applying transformation matrix to vertices3D
    void transformVertices();

    // Passing ObjectInfo and light's coords
    // as an argument so that the method
    // applies lightning and color correctly
    std::vector<Pixel> render(ObjectInfo, Point3D, zbufferVector&, int);

private:
    // Getting vertices' colors
    // using information about the object
    // and light's coordinates
    std::vector<Color> getColors(ObjectInfo, Point3D);
};

#endif // FACE_H
