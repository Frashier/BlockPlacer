#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include "face.h"
#include <vector>

using Pixel = std::pair<Point2D, Color>;

// Class representing a plane on which
// blocks are placed
class Plane : public Object
{
public:
    Plane();
    std::vector<Face> coloredFaces;     // Faces inherited from Object class are functional
                                        // but these are the ones that are colored

    virtual std::vector<Pixel> render(Point3D, zbufferVector&, int) override;
    virtual std::vector<Face> convertToFaces(std::vector<Point3D>) override;
};

#endif // PLANE_H
