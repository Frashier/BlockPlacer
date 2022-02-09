#ifndef OBJECTINFO_H
#define OBJECTINFO_H
#include "color.h"

class ObjectInfo
{
public:
    Color color;

    // Variables defining how object behaves
    double ambientFactor, diffuseFactor, specularFactor;
    int smoothness;

    ObjectInfo();
};

#endif // OBJECTINFO_H
