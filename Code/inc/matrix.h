/**
 * @file matrix.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */

struct meteoData_t
{

};

struct observation_t
{
    int day;
    int month;
    int year;
    double time;

    double windSpeed;
    double smithParam;
    double windSpeed;
};

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
    ~Matrix();

    void OpenMatrix(const char* name);
    void SaveMatrix(const char* name);

    void ClearMatrix();
    void AddObservation(meteoData_t data);
    void AddObservationFromCsv(const char* path);

    typedef float*** matrix_t ;
    void GetColdMatrix(matrix_t matrix);
    void GetWarmMatrix(matrix_t matrix);

    static const int N = 16; ///< Количество интервалов направления ветра
    static const int J = 7; ///< Количество интервалов категорий устойчивости атмосферы
    static const int K = 8; ///< Количество интервалов скорости ветра
    static const int size = sizeof(int) * N * J * K; ///< Размер матриц в байтах

private:

    int _mCold[N][J][K]; ///< ненормированная матрица повторяемости в холодное время года
    int _mWarm[N][J][K]; ///< ненормированная матрица повторяемости в теплое время года

    int _wCold[N][J][K]; ///< матрица повторяемости в холодное время года
    int _wWarm[N][J][K]; ///< матрица повторяемости в теплое время года

    observation_t* _observation; ///< указатель на первый элемент динамически выделяемого массива
                                 ///< наблюдений
    int _obsNumber; ///< количество наблюдений
};
