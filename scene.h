#ifndef SCENE_H
#define SCENE_H

#include "plane.h"
#include "point3d.h"
#include <QImage>
#include <QObject>

using gridCube = std::pair<std::array<Point3D, 8>, bool>;
using zbufferVector = std::vector<std::vector<std::pair<int, int>>>;

class Scene : public QObject
{
    Q_OBJECT

public:

    QImage *img, *background;
    std::vector<gridCube> grid;                             // Grid on which objects are placed
    std::vector<std::pair<Object, int>> objects;            // Vector of objects and their indexes on the grid
    Plane plane;
    zbufferVector zbuffer;                                  // Zbuffer with "z" value and the grid index to which
                                                            // this value belongs.
    zbufferVector clearZbuffer;                             // For clearing zbuffer.
    Point3D lightCoords;

    Scene(QImage*);

    virtual ~Scene() {};
    void drawScene();
    void modifyScene(Point2D);  // This method interprets user's mouse press

private:

    std::vector<gridCube> createGrid();
    int findGridIndex(Point2D);
    void drawObject(Object&, int);
    void fillBackground();

signals:
    void sceneRendered();
};

#endif // SCENE_H
