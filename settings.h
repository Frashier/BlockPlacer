#ifndef SETTINGS_H
#define SETTINGS_H
#include "matrix.h"
#include "point2d.h"
#include "color.h"

// This class' role is enabling the most primitive objects
// to access information set up in the user interface.

// It's most notable uses are converting 3D points to 2D and
// moving objects' vertices accordingly to the information
// specified by the user.

// While such global information could be deemed as not "clean" in coding sense,
// I found it less cumbersome and more fitting for such a simple application.

enum objectType {cube, slab, wall, column};

class Settings
{
public:

    // Perspective
    static constexpr int perspective = 590;

    // Frame's dimensions
    static int frameWidth;
    static int frameHeight;

    // Variables related to adding objects
    static int deleteMode;
    static int currentRotation;
    static objectType currentType;
    static Color currentColor;

    // Used in light calculations
    static double backgroundLightIntensity;
    static double lightIntensity;

    // Matrices used in transforming vertices
    static Matrix translationMatrix;
    static Matrix rotationMatrix_X;
    static Matrix rotationMatrix_Y;
    static Matrix rotationMatrix_Z;
    static Matrix transformationMatrix;

    static void createTransformationMatrix();
};


#endif // SETTINGS_H
