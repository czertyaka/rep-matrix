/**
 * @file matrix.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */
#include <vector>
using namespace std;

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

    typedef float*** matrix_t ;
    struct observation_t
    {
        int day; ///< день
        month_t month; ///< месяц
        int year; ///< год
        double time; ///< гринвичевское время

        compPoint_t windDir; ///< направление ветра, румб
        smithParam_t smithParam; ///< параметр Смита (категория устойчивости атмосферы)
        double windSpeed; ///< скорость ветра, м/с
    };

    Matrix();

    void OpenMatrix(const char* name);
    void SaveMatrix(const char* name);

    void ClearMatrix();
    void AddObservation(observation_t observation);
    void AddObservationFromCsv(const char* filename, const char* path = 0);

    void GetColdMatrix(matrix_t matrix);
    void GetWarmMatrix(matrix_t matrix);

    static const int N = 16; ///< Количество интервалов направления ветра
    static const int J = 7; ///< Количество интервалов категорий устойчивости атмосферы
    static const int K = 8; ///< Количество интервалов скорости ветра
    static const int size = sizeof(double) * N * J * K; ///< Размер матриц в байтах

private:

    int _mCold[N][J][K]; ///< ненормированная матрица повторяемости в холодное время года
    int _mWarm[N][J][K]; ///< ненормированная матрица повторяемости в теплое время года

    double _wCold[N][J][K]; ///< матрица повторяемости в холодное время года
    double _wWarm[N][J][K]; ///< матрица повторяемости в теплое время года

    vector<observation_t> _observations; ///< контейнер с наблюдениями
};
