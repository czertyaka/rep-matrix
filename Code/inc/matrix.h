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

/**
 * @brief Класс матрицы повторяемости
 * @details Содержит три основных поля - два трехмерных массива матриц повторяемости для теплого
 * и холодного времени года, а также указатель на первый элемент массив структур observation_t.
 * Работа с последним предполагает динамическое выделение памяти. Класс умеет открывать матрицу
 * из БД, сохранять свою матрицу в БД, очищать свои поля и добавлять в матрицу измерения -
 * как единственное, так и спарсенное из .CSV
 */
class Matrix
{

public:

    Matrix();

    void OpenMatrix(const char* name);
    void SaveMatrix(const char* name);

    void ClearMatrix();
    void AddObservationFromCsv(const char* filename, const char* path = 0);

    void GetColdMatrix(meteorology::matrix_t matrix);
    void GetWarmMatrix(meteorology::matrix_t matrix);

private:

    void _AddObservation(meteorology::observation_t observation);
    bool _CheckIfAdded();
    int _CalcN();
    int _CalcJ();
    int _CalcK();

    void _CheckConsistency();
    void _Normalize();

    meteorology::matrix_t _matrix; ///< матрица повторяемости
    meteorology::observation_t _currentObservation; ///< обрабатываемое наблюдение
    std::vector<meteorology::observation_t> _observations; ///< контейнер с наблюдениями
};

#endif /* _MATRIX_H_ */