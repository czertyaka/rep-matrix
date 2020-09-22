/**
 * @file matrix.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>

#include "namespace.h"
#include "stability_cathegory.h"

class Matrix
{

public:

    Matrix() = default;
    ~Matrix() = default;

    void OpenRP5File(const char* rp5File);
    meteorology::matrix_t GetMatrix();

private:

    meteorology::matrix_t _matrix;

};

#endif /* _MATRIX_H_ */