#include "object.h"
#include <future>
#include <QDebug>

Object::Object()
{

}

std::vector<Pixel> Object::render(Point3D lightCoords, zbufferVector& zbuffer, int index)
{
    std::vector<Pixel> renderedPixels;
    for (size_t i = 0; i < faces.size(); i++)
    {
        // Transform vertices and update face's common normal vector
        faces[i].transformVertices();


        // Render them
        std::vector<Pixel> temp = faces[i].render(this->info, lightCoords, zbuffer, index);
        renderedPixels.insert(renderedPixels.end(), temp.begin(), temp.end());
    }

    return renderedPixels;
}

// methods used for finding min and max values
// redundant, possible solution with std::min/max_element and comparator

int Object::minCoordValue(std::vector<Point3D> v, char c)
{
    int ans;

    switch(c)
    {
    case 'x':
        ans = v[0].x;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans > v[i].x)
            {
                ans = v[i].x;
            }
        }
        break;

    case 'y':
        ans = v[0].y;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans > v[i].y)
            {
                ans = v[i].y;
            }
        }
        break;

    case 'z':
        ans = v[0].z;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans > v[i].z)
            {
                ans = v[i].z;
            }
        }
        break;

    default:
        ans = 0;
        break;
    }

    return ans;
}

int Object::maxCoordValue(std::vector<Point3D> v, char c)
{
    int ans;

    switch(c)
    {
    case 'x':
        ans = v[0].x;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans < v[i].x)
            {
                ans = v[i].x;
            }
        }
        break;

    case 'y':
        ans = v[0].y;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans < v[i].y)
            {
                ans = v[i].y;
            }
        }
        break;

    case 'z':
        ans = v[0].z;
        for (size_t i = 1; i < v.size(); i++)
        {
            if (ans < v[i].z)
            {
                ans = v[i].z;
            }
        }
        break;

    default:
        ans = 0;
        break;
    }

    return ans;
}

std::vector<Face> Object::convertToFaces(std::vector<Point3D> vertices)
{
    // Creating sides
    // Side is a combination of 4 vertices and a char
    // Char is used in creation of side's faces to determine which
    // direction the char is facing

    std::vector<std::pair<char, std::array<Point3D, 4>>> sides;
    sides.push_back(std::make_pair('S', std::array<Point3D, 4> {vertices[0], vertices[3], vertices[2], vertices[1]}));
    sides.push_back(std::make_pair('N', std::array<Point3D, 4> {vertices[4], vertices[5], vertices[6], vertices[7]}));
    sides.push_back(std::make_pair('D', std::array<Point3D, 4> {vertices[0], vertices[1], vertices[5], vertices[4]}));
    sides.push_back(std::make_pair('E', std::array<Point3D, 4> {vertices[1], vertices[2], vertices[6], vertices[5]}));
    sides.push_back(std::make_pair('U', std::array<Point3D, 4> {vertices[2], vertices[3], vertices[7], vertices[6]}));
    sides.push_back(std::make_pair('W', std::array<Point3D, 4> {vertices[3], vertices[0], vertices[4], vertices[7]}));

    // Dividing sides into faces
    std::vector<Face> temp;
    for (size_t i = 0; i < sides.size(); i++)
    {
        char dir;
        std::array<Point3D, 4> vert;
        std::tie(dir, vert) = sides[i];

        auto midpoint = Point3D((vert[0].x + vert[2].x)/2, (vert[0].y + vert[2].y)/2,(vert[0].z + vert[2].z)/2);

        temp.push_back(Face (vert[0], midpoint, vert[1], dir));
        temp.push_back(Face (vert[1], midpoint, vert[2], dir));
        temp.push_back(Face (vert[2], midpoint, vert[3], dir));
        temp.push_back(Face (vert[3], midpoint, vert[0], dir));
    }


    return temp;
}
