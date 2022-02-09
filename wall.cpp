#include "wall.h"

Wall::Wall()
{

}

Wall::Wall(std::vector<Point3D> vertices_, Color c, int rotation)
{
    switch (rotation)
    {
        case 0:
            vertices = std::vector<Point3D>
            {
                    vertices_[0],
                    vertices_[1],
                    vertices_[2],
                    vertices_[3],
                    Point3D(vertices_[4].x, vertices_[4].y, vertices_[4].z - 16),
                    Point3D(vertices_[5].x, vertices_[5].y, vertices_[5].z - 16),
                    Point3D(vertices_[6].x, vertices_[6].y, vertices_[6].z - 16),
                    Point3D(vertices_[7].x, vertices_[7].y, vertices_[7].z - 16)
            };
            break;

        case 1:
            vertices = std::vector<Point3D>
            {
                    vertices_[0],
                    Point3D(vertices_[1].x - 16, vertices_[1].y, vertices_[1].z),
                    Point3D(vertices_[2].x - 16, vertices_[2].y, vertices_[2].z),
                    vertices_[3],
                    vertices_[4],
                    Point3D(vertices_[5].x - 16, vertices_[5].y, vertices_[5].z),
                    Point3D(vertices_[6].x - 16, vertices_[6].y, vertices_[6].z),
                    vertices_[7],
            };
            break;

        case 2:
            vertices = std::vector<Point3D>
            {

                    Point3D(vertices_[0].x, vertices_[0].y, vertices_[0].z + 16),
                    Point3D(vertices_[1].x, vertices_[1].y, vertices_[1].z + 16),
                    Point3D(vertices_[2].x, vertices_[2].y, vertices_[2].z + 16),
                    Point3D(vertices_[3].x, vertices_[3].y, vertices_[3].z + 16),
                    vertices_[4],
                    vertices_[5],
                    vertices_[6],
                    vertices_[7],
            };
            break;

        case 3:
            vertices = std::vector<Point3D>
            {
                    Point3D(vertices_[0].x + 16, vertices_[0].y, vertices_[0].z),
                    vertices_[1],
                    vertices_[2],
                    Point3D(vertices_[3].x + 16, vertices_[3].y, vertices_[3].z),
                    Point3D(vertices_[4].x + 16, vertices_[4].y, vertices_[4].z),
                    vertices_[5],
                    vertices_[6],
                    Point3D(vertices_[7].x + 16, vertices_[7].y, vertices_[7].z),
            };
            break;
    }

    faces = convertToFaces(vertices);

    info.color = c;
    info.diffuseFactor = 0.8;
    info.specularFactor = 0.5;
    info.ambientFactor = 1;
    info.smoothness = 1;
}
