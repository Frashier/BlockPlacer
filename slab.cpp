#include "slab.h"
#include <QDebug>

Slab::Slab()
{

}

Slab::Slab(std::vector<Point3D> vertices_, Color c)
{

    vertices = std::vector<Point3D>
    {
            vertices_[0],
            vertices_[1],
            Point3D(vertices_[2].x, vertices_[2].y + 16, vertices_[2].z),
            Point3D(vertices_[3].x, vertices_[3].y + 16, vertices_[3].z),
            vertices_[4],
            vertices_[5],
            Point3D(vertices_[6].x, vertices_[6].y + 16, vertices_[6].z),
            Point3D(vertices_[7].x, vertices_[7].y + 16, vertices_[7].z),
    };

    faces = convertToFaces(vertices);

    info.color = c;
    info.diffuseFactor = 0.8;
    info.specularFactor = 0.5;
    info.ambientFactor = 1;
    info.smoothness = 1;
}
