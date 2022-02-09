#include "column.h"

Column::Column()
{

}

Column::Column(std::vector<Point3D> vertices_, Color c)
{
    vertices = std::vector<Point3D>
    {
            Point3D(vertices_[0].x + 8, vertices_[0].y, vertices_[0].z + 8),
            Point3D(vertices_[1].x - 8, vertices_[1].y, vertices_[1].z + 8),
            Point3D(vertices_[2].x - 8, vertices_[2].y, vertices_[2].z + 8),
            Point3D(vertices_[3].x + 8, vertices_[3].y, vertices_[3].z + 8),
            Point3D(vertices_[4].x + 8, vertices_[4].y, vertices_[4].z - 8),
            Point3D(vertices_[5].x - 8, vertices_[5].y, vertices_[5].z - 8),
            Point3D(vertices_[6].x - 8, vertices_[6].y, vertices_[6].z - 8),
            Point3D(vertices_[7].x + 8, vertices_[7].y, vertices_[7].z - 8),
    };

    faces = convertToFaces(vertices);

    info.color = c;
    info.diffuseFactor = 0.8;
    info.specularFactor = 0.5;
    info.ambientFactor = 1;
    info.smoothness = 1;
}
