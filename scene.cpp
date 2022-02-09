#include "scene.h"
#include "settings.h"
#include "cube.h"
#include "slab.h"
#include "wall.h"
#include "column.h"
#include <limits>
#include <algorithm>

using Pixel = std::pair<Point2D, Color>;

namespace
{
    // Draw a pixel on a given point with a given color on a given QImage
    bool drawPixel(Point2D p, Color c, QImage *i)
    {
        // If given coordinates are not correct
        if (p.x < 0 || p.x > i->width() - 1 || p.y < 0 || p.y > i->height() - 1)
        {
            return false;
        }

        // Set the pointer at the beginning of y-line
        unsigned char *ptr = i->scanLine(p.y);

        ptr[4 * p.x] = c.blue;
        ptr[4 * p.x + 1] = c.green;
        ptr[4 * p.x + 2] = c.red;

        return true;
    }

    double calculateTriangleArea(std::array<Point2D, 3> vertices)
    {
        return abs((double) (vertices[0].x * (vertices[1].y - vertices[2].y) +
                vertices[1].x * (vertices[2].y - vertices[0].y) +
                vertices[2].x * (vertices[0].y - vertices[1].y))/2);
    }

    // Algorithm checking if point is inside a triangle
    bool isPointInFace(Point2D p, Face c)
    {
        double faceArea = calculateTriangleArea(std::array<Point2D, 3> {c.vertices2D[0], c.vertices2D[1], c.vertices2D[2]});

        double A1 = calculateTriangleArea(std::array<Point2D, 3> {p, c.vertices2D[0], c.vertices2D[1]});
        double A2 = calculateTriangleArea(std::array<Point2D, 3> {p, c.vertices2D[1], c.vertices2D[2]});
        double A3 = calculateTriangleArea(std::array<Point2D, 3> {p, c.vertices2D[2], c.vertices2D[0]});

        return A1 + A2 + A3 == faceArea;
    }
}

Scene::Scene(QImage *img_)
{
    // Assing values to QImage members
    img = img_;
    background = new QImage(Settings::frameWidth, Settings::frameHeight, QImage::Format_RGB32);
    fillBackground();

    // Create plane and grid
    plane = Plane();
    grid = createGrid();

    // Create light source
    lightCoords = Point3D(Settings::frameWidth, -Settings::frameHeight, -Settings::frameHeight);

    // Initialize zbuffer
    clearZbuffer = std::vector<std::vector<std::pair<int, int>>>
            (Settings::frameWidth, std::vector<std::pair<int, int>>
             (Settings::frameHeight, std::make_pair(std::numeric_limits<int>::max(), -2)));

    drawScene();
}

// Fill background with a given color
void Scene::fillBackground()
{
    const Color c = Color(70, 170, 180);
    for (int x = 0; x < Settings::frameWidth; x++)
    {
        for (int y = 0; y < Settings::frameHeight; y++)
        {
            drawPixel(Point2D(x, y), c, background);
        }
    }
}

// Method used in constructor to create a grid
// of cubes going upwards from the plane.
// Used for placing other objects on the scene.
std::vector<gridCube> Scene::createGrid()
{
    std::vector<gridCube> grid;

    // Loop increasing the y-level of the grid
    for (int y = 0; y > -32*16; y -= 32)
    {
        // Iterate through plane's faces
        for (auto it = plane.faces.begin(); it != plane.faces.end(); it += 2)
        {
            const gridCube temp = std::make_pair(std::array<Point3D, 8>
            {
                Point3D(it->vertices3D[0].x, y, it->vertices3D[0].z),
                Point3D(it->vertices3D[2].x, y, it->vertices3D[2].z),
                Point3D(it->vertices3D[2].x, y - 32, it->vertices3D[2].z),
                Point3D(it->vertices3D[0].x, y - 32, it->vertices3D[0].z),
                Point3D((it+1)->vertices3D[1].x, y, (it+1)->vertices3D[1].z),
                Point3D((it+1)->vertices3D[2].x, y, (it+1)->vertices3D[2].z),
                Point3D((it+1)->vertices3D[2].x, y - 32, (it+1)->vertices3D[2].z),
                Point3D((it+1)->vertices3D[1].x, y - 32, (it+1)->vertices3D[1].z)
            }, false);

            grid.push_back(temp);
        }
    }
    return grid;
}

// Find the neighbouring index on the grid
// based on the object clicked and on the object's
// face clicked. Face has a "facing" member that tells us
// which direction it's facing.
int Scene::findGridIndex(Point2D pointPressed)
{
    // Iterate through objects.
    // Objects other than a plane are prioritized
    // when determining the face clicked.
    for (auto object = objects.begin(); object != objects.end(); object++)
    {
        Object objectInstance;
        int objectIndex;
        std::tie(objectInstance, objectIndex) = *object;

        // Iterate through faces
        for (auto& face : objectInstance.faces)
        {
            // If the face is not visible,
            // or object index on zbuffer doesn't match
            // with current object's index, continue
            int zValue;
            int zValueObjectIndex;
            std::tie(zValue, zValueObjectIndex) = zbuffer[pointPressed.x][pointPressed.y];

            if (!face.visible || zValueObjectIndex != objectIndex)
            {
                continue;
            }

            // If a face is clicked return index
            if (isPointInFace(pointPressed, face))
            { 
                // If in delete mode, throw clicked index instead
                if (Settings::deleteMode)
                {
                    throw objectIndex;
                }

                // Neighbouring index returned depends on
                // the direction the face is facing
                switch(face.facing)
                {
                case 'N':
                    return objectIndex + 16;
                    break;

                case 'E':
                    return objectIndex + 1;
                    break;

                case 'S':
                    return objectIndex - 16;
                    break;

                case 'W':
                    return objectIndex - 1;
                    break;

                case 'U':
                    return objectIndex + 16*16;
                    break;

                case 'D':
                    return objectIndex - 16*16;
                    break;

                default:
                    return -1;
                    break;
                }
            }
        }
    }

    // If in delete mode, don't search the plane
    if (Settings::deleteMode)
    {
        return -1;
    }

    // Iterate through plane
    for (size_t i = 0; i < plane.faces.size(); i++)
    {
        // If a face is clicked return index above
        // 2 faces
        if (isPointInFace(pointPressed, plane.faces[i]))
        {
            return i/2;
        }

    }

    return -1;
}

void Scene::modifyScene(Point2D pointPressed)
{

    int index;

    try
    {
        index = findGridIndex(pointPressed);

        // If index is not valid
        if (index < 0 || index >= 16 * 16 * 16)
        {
            return;
        }

        // If an index was not found or if index is already occupied, return
        std::array<Point3D, 8> vertices;
        bool taken;
        std::tie(vertices, taken) = grid[index];

        if (taken)
        {
            return;
        }

        switch(Settings::currentType)
        {
            case cube:
                objects.push_back(std::make_pair(Cube(std::vector<Point3D>(vertices.begin(), vertices.end()), Settings::currentColor), index));
                break;

            case slab:
                objects.push_back(std::make_pair(Slab(std::vector<Point3D>(vertices.begin(), vertices.end()), Settings::currentColor), index));
                break;

            case wall:
                objects.push_back(std::make_pair(Wall(std::vector<Point3D>(vertices.begin(), vertices.end()), Settings::currentColor, Settings::currentRotation), index));
                break;

            case column:
                objects.push_back(std::make_pair(Column(std::vector<Point3D>(vertices.begin(), vertices.end()), Settings::currentColor), index));
                break;

            default:
                return;
        }

        // Create object and set grid index to true (occupied)
        grid[index].second = true;
    }
    catch (int indexPressed)
    {
        index = indexPressed;

        if (index < 0)
        {
            return;
        }

        // Delete object and set the grid index to false (free)
        objects.erase(std::find_if(objects.begin(), objects.end(),
                      [&](const std::pair<Object, int>& object) {return object.second == index;}));
        grid[index].second = false;
    }


    drawScene();
}

void Scene::drawObject(Object& o, int index)
{
    std::vector<Pixel> pixels = o.render(lightCoords, zbuffer, index);

    for (auto& pixel : pixels)
    {
        Point2D point;
        Color color;
        std::tie(point, color) = pixel;

        drawPixel(point, color, img);
    }
}

void Scene::drawScene()
{
    // Overwrite zbuffer
    zbuffer = clearZbuffer;

    // Create transformation matrix with given user information
    Settings::createTransformationMatrix();

    // Resetting the image
    *img = background->copy(0, 0, Settings::frameWidth, Settings::frameHeight);

    // Render every object on the scene and draw it
    drawObject(plane, -1);
    for (auto& object : objects)
    {
        drawObject(object.first, object.second);
    }

    // Send signal that the scene has been rendered so that MainWindow can update
    emit sceneRendered();
}

