/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "main.h"
using namespace meteorology;

void oStreamFunc(std::ostream& o)
{
    o << "hey!" << std::endl;
}

int main()
{
    matrix_t matrix;
    MatrixCalculator matrixCalculator(matrix,
                                      "/home/czert/workspace/rep-matrix/RP5-files/big-utf-8.csv",
                                      55.6971868, 60.8041044);
    matrixCalculator.DataOStream();

    return 0;
}