#include "face.h"
#include "matrix.h"
#include "settings.h"
#include <limits>
#include <algorithm>
#include <cmath>

namespace
{
    int findMinY(Point2D* vertices)
    {
        int
                min = std::numeric_limits<int>::max(),  // starting from the highest possible y
                temp;

        for (int i = 0; i < 3; i++)
        {
            temp = vertices[i].y;
            if (temp < min)
            {
                min = temp;
            }
        }

        return min;
    }


    int findMaxY(Point2D* vertices)
    {
        int
                min = std::numeric_limits<int>::min(),  // starting from the lowest possible y
                temp;

        for (int i = 0; i < 3; i++)
        {
            temp = vertices[i].y;
            if (temp > min)
            {
                min = temp;
            }
        }

        return min;
    }

    // finding cross points
    std::vector<int> findCutX(int y, Point2D* vertices)
    {
        std::vector<int> vbuf;

        for (int i = 0; i < 3; i++)
        {
            const Point2D A = vertices[i];
            const Point2D B = vertices[(i + 1) % 3];

            if (A.y == B.y && B.y == y && vbuf.size() == 0)
            {
                vbuf.push_back(A.x);
                vbuf.push_back(B.x);
                break;
            }

            if (A.y == y)
            {
                vbuf.push_back(A.x);
                continue;
            }

            if ((A.y < y && B.y > y) || (A.y > y && B.y < y))
            {
                const int denominator = B.y - A.y;

                // in the case of division by zero continue
                if (denominator == 0)
                {
                    continue;
                }

                // formula for cross point
                const int tempx = A.x + ((y - A.y) * ((double) (B.x - A.x) / denominator));
                vbuf.push_back(tempx);

            }
        }

        std::sort(vbuf.begin(), vbuf.end());     // sort ascending
        return vbuf;
    }

    // modify vector so that it's length equals 1
    std::array<double, 3> normalizeVector(std::array<double, 3> v)
    {
        double length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

        return std::array<double, 3>  {v[0]/length, v[1]/length, v[2]/length};
    }

    // Get vector from given two points
    std::array<double, 3> createVector(Point3D p, Point3D q)
    {
        return std::array<double, 3> {(double) q.x - p.x, (double) q.y - p.y, (double) q.z - p.z};
    }

    // Multiplication of two 3x1 vectors
    std::array<double, 3> multiplyVectors(std::array<double, 3> w_1, std::array<double, 3> w_2)
    {
        return std::array<double, 3>    {w_1[1] * w_2[2] - w_2[1] * w_1[2],
                                         w_2[0] * w_1[2] - w_1[0] * w_2[2],
                                         w_1[0] * w_2[1] - w_2[0] * w_1[1]};
}

    // Get vector perpendicular to 3 given vertices
    std::array<double, 3> calculateNormalVector(Point3D p, Point3D q, Point3D r)
    {
        return multiplyVectors(createVector(q, p), createVector(q, r));
    }

    // get scalar product of 2 given 3x1 vectors
    double calculateScalarProduct(std::array<double, 3> N, std::array<double, 3> V)
    {
        return (N[0] * V[0] + N[1] * V[1] + N[2] * V[2]);
    }

    std::array<double, 3> createReflectionVector(std::array<double, 3> L, std::array<double, 3> N)
    {
        double scalar = 2 * calculateScalarProduct(L, N);
        std::array<double, 3> R;
        for (int i = 0; i < 3; i++)
        {
            R[i] = scalar * N[i] - L[i];
        }

        return R;
    }

    double calculateV(Point2D p, Point2D* vertices)
    {
        double A = ((p.x - vertices[0].x) * (vertices[2].y - vertices[0].y))
                - ((vertices[2].x - vertices[0].x) * (p.y - vertices[0].y));

        double B = ((vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y))
                - ((vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y));

        if (B == 0)
        {
            return -1;
        }

        return (double) A/B;

    }
    double calculateW(Point2D p, Point2D* vertices)
    {
        double A = ((vertices[1].x - vertices[0].x) * (p.y - vertices[0].y))
                - ((p.x - vertices[0].x) * (vertices[1].y - vertices[0].y));

        double B = ((vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y))
                - ((vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y));

        if (B == 0)
        {
            return -1;
        }

        return (double) A/B;
    }
}

Face::Face(){}

Face::Face(Point3D p, Point3D q, Point3D r, char facing_)
{
    vertices3D[0] = p;
    vertices3D[1] = q;
    vertices3D[2] = r;
    facing = facing_;
}

std::vector<Color> Face::getColors(ObjectInfo info, Point3D lightCoords)
{
    // Calculate normal vectors first, so that it is possible to tell whether
    // the face is visible.

    std::vector<std::array<double, 3>> normalVectors;
    std::vector<std::array<double, 3>> observerVectors;

    for (int i = 0; i < 3; i++)
    {
        // Getting previous and next vertex from the face.
        Point3D nextVertex;
        if (i - 1 < 0)
        {
            nextVertex = transformedVertices3D[2];
        }
        else
        {
            nextVertex = transformedVertices3D[i - 1];
        }

        const Point3D previousVertex = transformedVertices3D[(i + 1) % 3];

        const auto normalVector = normalizeVector(calculateNormalVector(previousVertex, transformedVertices3D[i], nextVertex));
        const auto observerVector = normalizeVector(createVector(transformedVertices3D[i], Point3D(0, 0, -Settings::perspective)));

        // Testing for visibility.
        // Vertex is visible when the scalar product of normal vector and observer vector
        // is positive.
        // If at least one vertex is not visible,
        // then face's surface is not visible.
        if (calculateScalarProduct(normalVector, observerVector) < 0)
        {
             return std::vector<Color>();
        }

        normalVectors.push_back(normalVector);
        observerVectors.push_back(observerVector);
    }

    std::vector<Color> colors;
    for (int i = 0; i < 3; i++)
    {
        const auto lightVector = normalizeVector(createVector(transformedVertices3D[i], lightCoords));
        const auto reflectionVector = normalizeVector(createReflectionVector(lightVector, normalVectors[i]));

        const double ambientValue = info.ambientFactor * Settings::backgroundLightIntensity;
        const double diffuseValue = Settings::lightIntensity * info.diffuseFactor * calculateScalarProduct(normalVectors[i],lightVector);
        const double specularValue = info.specularFactor * pow(calculateScalarProduct(reflectionVector, observerVectors[i]), info.smoothness);

        colors.push_back(info.color * (ambientValue + diffuseValue) + specularValue);
    }

    return colors;
}

void Face::transformVertices()
{
    for (int i = 0; i < 3; i++)
    {
        const Matrix matrixOfVertices = Settings::transformationMatrix
                                        .multiply(Matrix(std::vector<std::vector<double>>
                                                         {
                                                             {(double) vertices3D[i].x},
                                                             {(double) vertices3D[i].y},
                                                             {(double) vertices3D[i].z},
                                                             {1}
                                                         }));

        transformedVertices3D[i] = Point3D(matrixOfVertices.values[0][0], matrixOfVertices.values[1][0], matrixOfVertices.values[2][0]) ;
    }

    // convert 3D points to 2D points
    vertices2D[0] = Point2D(transformedVertices3D[0]);
    vertices2D[1] = Point2D(transformedVertices3D[1]);
    vertices2D[2] = Point2D(transformedVertices3D[2]);
}

std::vector<Pixel> Face::render(ObjectInfo info, Point3D lightCoords, zbufferVector& zbuffer, int index)
{
    // Get colors on vertices
    std::vector<Color> colors = getColors(info, lightCoords);

    // Empty vector means the face is not visible
    if (colors.empty())
    {
        visible = false;
        return std::vector<Pixel>();
    }

    visible = true;

    const int minY = findMinY(vertices2D);
    const int maxY = findMaxY(vertices2D);

    std::vector<Pixel> renderedPoints;
    for (int y = minY; y <= maxY; y++)
    {
        // If current y wouldn't be in frame's borders
        if (y < 0 || y >= Settings::frameHeight)
        {
            continue;
        }

        const auto cutX = findCutX(y, vertices2D);

        // It's safe to assume there are going to be either 0 or 2 cross points
        // found since we are dealing with triangles
        if (cutX.size() != 2)
        {
            continue;
        }

        for (int x = cutX[0]; x <= cutX[1]; x++)
        {
            // If current x wouldn't be in frame's borders
            if (x < 0 || x >= Settings::frameWidth)
            {
                continue;
            }
            const auto p = Point2D(x, y);

            // Barycentric coordinates
            const double V = calculateV(p, vertices2D);
            const double W = calculateW(p, vertices2D);
            const double U = 1 - V - W;

            // If coordinates are incorrect, continue
            // (above functions return -1 on division by zero)
            if (V < 0 || W < 0 || U < 0)
            {
                continue;
            }

            // Finding the depth of the point
            int z = transformedVertices3D[0].z * U + transformedVertices3D[1].z * V + transformedVertices3D[2].z * W;

            // Determine whether the pixel should be drawn
            if (z >= zbuffer[x][y].first)
            {
                continue;
            }

            // Update zbuffer values
            zbuffer[x][y].first = z;
            zbuffer[x][y].second = index;

            const auto c = colors[0] * U + colors[1] * V + colors[2] * W;
            renderedPoints.push_back(std::make_pair(p, c));
        }
    }

    return renderedPoints;
}
