/**
 * @file matrix.cpp
 * @author czertyaka
 * @date 2020-08-10
 * @brief Реализация методов Matrix
 */

#include "matrix.h"

using namespace meteorology;

/**
 * @brief Construct a new Matrix:: Matrix object
 * @details Инициазирует значения по умолчанию: номер измерений - нулем, а массив наблюдений - 
 * ссылкой на массив нулевой длины. В теле конструктора открывается база данных и заполняются 
 * массивы матриц повторяемости некой дефолтной матрицей. Дефолтная матрица в БД доступна только
 * на чтение. При этом соответсвующие ей журнал измерений _observation не заполняются - 
 * предполагается, что такой массив будет чрезмерно велик.
 */
Matrix::Matrix()
{
    /* code opening DB and initializing the _wCold and _wWarm */
}

/**
 * @brief Открывает данные из БД
 * @details В БД ищется единица данных "холодная матрица, теплая матрицы, измерения", заполняются
 * массивы _wCold и _wWarm, выделяется память под измерения.
 * @param name Имя единицы данных "холодная матрица, теплая матрицы, измерения"
 * @warning Необходима проверка, насколько хорошо будет работать выделение памяти из кучи для
 * больших массивов измерений.
 */
void Matrix::OpenMatrix(const char* name)
{
    /* code, filling the _wCold and _wWarm, getting number of observations and memory allocationg
    for observations */
}

/**
 * @brief Сохраняет данные в БД
 * @details В БД создается (если нужно) единица данных "холодная матрица, теплая матрицы, 
 * измерения", в которую сохраняются текущие данные в полях _wCold, _wWarm и _observation
 * @param name Имя единицы данных "холодная матрица, теплая матрицы, измерения"
 * @warning Метод НЕ должен использоваться для сохранения под именем $default - будет добавлено
 * исключение.
 */
void Matrix::SaveMatrix(const char* name)
{
    /* code checking if name != $default and saving the data to DB*/
}

/**
 * @brief Стирает матрицу и журнал измерений
 */
void Matrix::ClearMatrix()
{
    /* code clearing _wCold, _wWarm, _obseravtion and setting _obsNumber = 0 */
}

/**
 * @brief Добавляет в матрицу единичное измерение
 * @param data Структура со всеми необходимыми для расчета данными
 */
void Matrix::_AddObservation(observation_t observation)
{
    _CheckConsistency(observation.windDir, observation.smithParam);
    bool isAlreadyAdded = _CheckIfAdded(observation);

    if (!isAlreadyAdded)
    {
        int n = _CalcN(observation.windDir);
        int j = _CalcJ(observation.smithParam);
        int k = _CalcK(observation.windSpeed);

        switch (observation.month)
        {
        case meteorology::november:
        case meteorology::december:
        case meteorology::january:
        case meteorology::february:
        case meteorology::march:
            _mCold[n][j][k]++;
            _Normalize(_mCold, _wCold);
            break;
        default:
            _mWarm[n][j][k]++;
            _Normalize(_mWarm, _wWarm);
            break;
        }
    }
}

/**
 * @brief Добавляет в матрицу измерения, содержащиеся в .CSV файле
 * @details Парсит .CSV файл и добавляет измерения из него (вызов AddObservation(meteoData_t data)).
 * @param filename Имя файла
 * @param path Путь до файла (при передаче значения по умолчанию = 0, поиск будет производитя в 
 * текущем каталоге)
 */

void Matrix::AddObservationFromCsv(const char* filename, const char* path/* = 0*/)
{
    /* code, parsing file, and adding the observations */
}

/**
 * @brief Заполняет переданный указатель матрицей повторяемости в холодное время года
 * @param matrix Указатель на трехмерный массив
 * @warning Пользователь должен убедиться, что выделил достаточно места (поле size).
 */
void Matrix::GetColdMatrix(matrix_t matrix)
{
    /* code, copying the _mCold to matrix */
}

/**
 * @brief Заполняет переданный указатель матрицей повторяемости в теплое время года
 * @param matrix Указатель на трехмерный массив
 * @warning Пользователь должен убедиться, что выделил достаточно места (поле size).
 */
void Matrix::GetWarmMatrix(matrix_t matrix)
{
    /* code, copying the _mWarm to matrix */
}

bool Matrix::_CheckIfAdded(observation_t observation)
{
    /* code, checking if observation is already in _observations */
    return false;
}