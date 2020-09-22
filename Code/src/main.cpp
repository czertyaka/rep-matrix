/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "main.h"

int main()
{
    Matrix matrix;
    matrix.OpenRP5File("/home/czert/workspace/rep-matrix/RP5-files/small-utf8.csv");

    return 0;
}