#include "cube.h"
#include <QDebug>

Cube::Cube()
{

}

Cube::Cube(std::vector<Point3D> vertices_, Color c)
{
    vertices = vertices_;
    faces = convertToFaces(vertices);

    info.color = c;
    info.diffuseFactor = 0.8;
    info.specularFactor = 0.5;
    info.ambientFactor = 1;
    info.smoothness = 1;
}
