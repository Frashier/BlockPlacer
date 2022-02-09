#include "point2d.h"
#include "mainwindow.h"
#include "settings.h"
#include <exception>

Point2D::Point2D() {};
Point2D::Point2D(int x_, int y_) : x(x_), y(y_) {};
Point2D::Point2D(Point3D p)
{
    if (p.z + Settings::perspective == 0)
    {
        return;
    }

    // Last addition is so that the 0,0 point is in the center of the frame
    x = (p.x * Settings::perspective) / (p.z + Settings::perspective) + Settings::frameWidth/2;
    y = (p.y * Settings::perspective) / (p.z + Settings::perspective) + Settings::frameHeight/2;
}

bool Point2D::operator==(Point2D p)
{
    return this->x == p.x && this->y == p.y;
}

Point2D Point2D::operator+(int n)
{
    return Point2D(this->x + n, this->y + n);
}

