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
    bool _CheckIfAdded(meteorology::observation_t observation);
    int _CalcN(meteorology::compPoint_t windDir);
    int _CalcJ(meteorology::smithParam_t smithParam);
    int _CalcK(double windSpeed);

    void _CheckConsistency(double windSpeed, meteorology::smithParam_t smithParam);

    void _Normalize(meteorology::unnormMatrix_t unnormalized, meteorology::matrix_t normalized);

    meteorology::unnormMatrix_t _mCold; ///< ненормированная матрица повторяемости в холодное время года
    meteorology::unnormMatrix_t _mWarm; ///< ненормированная матрица повторяемости в теплое время года

    meteorology::matrix_t _wCold; ///< матрица повторяемости в холодное время года
    meteorology::matrix_t _wWarm; ///< матрица повторяемости в теплое время года

    std::vector<meteorology::observation_t> _observations; ///< контейнер с наблюдениями
};

#endif /* _MATRIX_H_ */