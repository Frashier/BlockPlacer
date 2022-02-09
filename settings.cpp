#include <settings.h>
#include <vector>
#include <math.h>

int Settings::frameWidth = 0;
int Settings::frameHeight = 0;

double Settings::backgroundLightIntensity = 0;
double Settings::lightIntensity = 0;

int Settings::deleteMode = 0;
objectType Settings::currentType = cube;
int Settings::currentRotation = 0;
Color Settings::currentColor = Color(60, 60, 60);

Matrix Settings::translationMatrix = Matrix(std::vector<std::vector<double>>
                                  {
                                      {1, 0, 0, 0},
                                      {0, 1, 0, 100},
                                      {0, 0, 1, 0},
                                      {0, 0, 0, 1}
                                  });
Matrix Settings::rotationMatrix_X = Matrix(std::vector<std::vector<double>>
                       {
                           {1, 0, 0, 0},
                           {0, cos(0), -sin(0), 0},
                           {0, sin(0), cos(0), 0},
                           {0, 0, 0, 1}
                       });
Matrix Settings::rotationMatrix_Y = Matrix(std::vector<std::vector<double>>
                       {
                           {cos(0), 0, sin(0), 0},
                           {0, 1, 0, 0},
                           {-sin(0), 0, cos(0), 0},
                           {0, 0, 0, 1}
                       });
Matrix Settings::rotationMatrix_Z = Matrix(std::vector<std::vector<double>>
                       {
                           {cos(0), -sin(0), 0, 0},
                           {sin(0), cos(0), 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}
                       });

Matrix Settings::transformationMatrix = Matrix();

void Settings::createTransformationMatrix()
{
    Settings::transformationMatrix =    Settings::translationMatrix
                                        .multiply(Settings::rotationMatrix_X)
                                        .multiply(Settings::rotationMatrix_Y)
                                        .multiply(Settings::rotationMatrix_Z);
}
