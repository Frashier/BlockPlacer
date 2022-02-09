#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <QString>

// Class used for construction of transformation matrix
// needed to correctly draw 3D objects
class Matrix
{
public:
    std::vector<std::vector<double>> values;

    Matrix();
    Matrix(size_t m, size_t n);                         // Used for initialization with given row
                                                        // and column count (to 0 value)
    Matrix(std::vector<std::vector<double>> values_);

    ~Matrix();

    static Matrix add(Matrix a, Matrix b);
    static Matrix multiply(Matrix a, Matrix b);
    Matrix multiply(Matrix a);

    QString toString();
};

#endif // MATRIX_H
