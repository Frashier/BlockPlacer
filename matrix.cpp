#include "matrix.h"
#include <stdlib.h>

Matrix::Matrix(){};

Matrix::~Matrix(){};

Matrix::Matrix(size_t m, size_t n)
{
    values = std::vector<std::vector<double>> (m, std::vector<double>(n));
}

Matrix::Matrix(std::vector<std::vector<double>> values_) : values(values_){};

Matrix Matrix::add(Matrix a, Matrix b)
{
    const int aRows = a.values.size();
    const int aColumns = a.values[0].size();

    Matrix temp(aRows, aColumns);

    for (int i = 0; i < aRows; i++)
    {
        for (int j = 0; j < aColumns; j++)
        {
            temp.values[i][j] = a.values[i][j] + b.values[i][j];
        }
    }

    return temp;
}

Matrix Matrix::multiply(Matrix a, Matrix b)
{
    const int aRows = a.values.size();
    const int aColumns = a.values[0].size();
    const int bColumns = b.values[0].size();

    Matrix temp(aRows, bColumns);

    for (int i = 0; i < aRows; i++)
    {
        for (int j = 0; j < bColumns; j++)
        {
            for (int k = 0; k < aColumns; k++)
            {
                temp.values[i][j] += a.values[i][k] * b.values[k][j];
            }
        }
    }

    return temp;
}

Matrix Matrix::multiply(Matrix a)
{
    return Matrix::multiply(*this, a);
}

QString Matrix::toString()
{
    QString temp = "";

    int
            rows = values.size(),
            columns = values[0].size();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            temp.append(QString::number(values[i][j]) + " ");
        }

        temp.append("\n");
    }

    return temp;
}

















