#include "plane.h"
#include<QDebug>
Plane::Plane()
{
    std::vector<Point3D> vertices_ = std::vector<Point3D>
    {
        Point3D(-256, 0, 256),
        Point3D(256, 0, 256),
        Point3D(256, 0, -256),
        Point3D(-256, 0, -256)
    };

    vertices = vertices_;
    faces = Plane::convertToFaces(vertices);

    // Add colored faces
    const Point3D midPoint = Point3D(0, 0, 0);
    coloredFaces.push_back(Face(vertices[0], midPoint, vertices[1], 'U'));
    coloredFaces.push_back(Face(vertices[1], midPoint, vertices[2], 'U'));
    coloredFaces.push_back(Face(vertices[2], midPoint, vertices[3], 'U'));
    coloredFaces.push_back(Face(vertices[3], midPoint, vertices[0], 'U'));

    info.color = Color(40, 90, 50);
    info.diffuseFactor = 0.8;
    info.specularFactor = 0.5;
    info.ambientFactor = 1;
    info.smoothness = 1;
}

std::vector<Face> Plane::convertToFaces(std::vector<Point3D> vertices)
{
    // In the case of a plane y is the same for each point
    const int y = vertices[0].y;

    // Finding min and max values
    const int maxX = maxCoordValue(vertices, 'x');
    const int minX = minCoordValue(vertices, 'x');
    const int maxZ = maxCoordValue(vertices, 'z');
    const int minZ = minCoordValue(vertices, 'z');

    // Making a grid of vertices
    std::vector<Point3D> grid;
    for (int z = minZ; z <= maxZ; z += (maxZ - minZ)/16)
    {
        for (int x = minX; x <= maxX; x += (maxX - minX)/16)
        {
            grid.push_back(Point3D(x, y, z));
        }
    }

    // Connecting grid into faces
    std::vector<Face> faces;
    for (size_t i = 0; i < grid.size() - 17; i++)
    {
        // continue if in the last column
        if (i%17 == 16)
        {
            continue;
        }

        // First face
        faces.push_back(Face(grid[i], grid[i+18], grid[i+1], 'U'));

        // Second face
        faces.push_back(Face(grid[i], grid[i + 17], grid[i + 18], 'U'));
    }

    return faces;
}

std::vector<Pixel> Plane::render(Point3D lightCoords, zbufferVector& zbuffer, int index)
{
    // Transform functional faces
    for (auto& face : faces)
    {
        face.transformVertices();
    }

    // Transform and render colored faces
    std::vector<Pixel> renderedPixels;
    for (auto& coloredFace : coloredFaces)
    {
        coloredFace.transformVertices();

        std::vector<Pixel> temp = coloredFace.render(this->info, lightCoords, zbuffer, index);
        renderedPixels.insert(renderedPixels.end(), temp.begin(), temp.end());
    }

    return renderedPixels;
}












