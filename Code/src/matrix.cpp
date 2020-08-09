#include "matrix.h"

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

void Matrix::AddObservation(observation_t observation)
{
    int n = _WindDirIndex(observation);
    int j = _SmithParamIndex(observation);
    int k = _WindSpeedIndex(observation);

    _m[n][j][k]++;
    _CalculateMatrix();    
}

void Matrix::ClearMatrix()
{
    for (int n = 0; n < _n; n++)
    {
        for (int j = 0; j < _j; j++)
        {
            for (int  k = 0; k < _k; k++)
            {
                _m[n][j][k] = 0;
                _w[n][j][k] = 0;
            }
        }
    }
}

void Matrix::GetMatrix(float*** pppMatrix)
{
    for (int n = 0; n < _n; n++)
    {
        for (int j = 0; j < _j; j++)
        {
            for (int  k = 0; k < _k; k++)
            {
                pppMatrix[n][j][k] = _w[n][j][k];
            }
        }
    }
}

const int Matrix::GetN() { return _n; }
const int Matrix::GetJ() { return _j; }
const int Matrix::GetK() { return _k; }
const int Matrix::GetSize() { return _n * _j * _k; }

void Matrix::_CalculateMatrix()
{

}

int Matrix::_WindDirIndex(observation_t observation)
{
    return 0;
}

int Matrix::_SmithParamIndex(observation_t observation)
{
    return 0;
}

int Matrix::_WindSpeedIndex(observation_t observation)
{
    return 0;
}