/**
 * @file obseravtion.h
 * @author your name (you@domain.com)
 * @date 2020-08-16
 * @brief Класс для хранения и расчета направления и скорости ветра, категории неустойчивости
 * атмосферы на основе метеоданных
 */

/**
 * @brief Румбы
 */
enum compPoint_t : int
{
    N   = 0,
    NNE = 1,
    NE  = 2,
    ENE = 3,
    E   = 4,
    ESE = 5,
    SE  = 6,
    SSE = 7,
    S   = 8,
    SSW = 9,
    SW  = 10,
    WSW = 11,
    W   = 12,
    WNW = 13,
    NW  = 14,
    NNW = 15
};

/**
 * @brief Месяца
 */
enum month_t : int
{
    january     = 1,
    february    = 2,
    march       = 3,
    april       = 4,
    may         = 5,
    june        = 6,
    july        = 7,
    august      = 8,
    september   = 9,
    october     = 10,
    november    = 11,
    december    = 12
};

/**
 * @brief Классы устойчивости атмосферы
 */
enum stabilityClass_t : int
{
    cathA = 1,
    cathB = 2,
    cathC = 3,
    cathD = 4,
    cathE = 5,
    cathF = 6,
    cathG = 7
};

/**
 * @brief Структура метеоданных
 * @details Такая структура будет заполняться при парсинге одной строчки .csv-файла. В ней все
 * необходимые параметры для расчета полей структуры observaion_t
 */
struct meteoData_t
{
    double latitude; ///< широта, град.
    double longitude; ///< долгота, град.

    int day; ///< день
    month_t month; ///< месяц
    int year; ///< год
    double time; ///< гринвичевское время, ч

    compPoint_t windDir; ///< направление ветра, румб
    double windSpeed; ///< скорость ветра, м/с
    int cloudAmount; ///< балл общей облачности
    int lowerCloudAmount; ///< балл нижней облачности
    bool fog; ///< наличие сильного тумана (видимость < 1 км)
    bool snow; ///< наличие сплошного снежного покрова
};

/**
 * @brief Структура наблюдения
 * @details Струкутра наблюдения для заполнения ненормированной матрицы повторяемости. Каждой 
 * матрице повторяемости ставится в соответсвие массив из таких структур, вместе с матрицой хранится
 * в БД.
 */
struct observation_t
{
    int day; ///< день
    month_t month; ///< месяц
    int year; ///< год
    double time; ///< гринвичевское время

    compPoint_t windDir; ///< направление ветра, румб
    stabilityClass_t smithParam; ///< параметр Смита (категория устойчивости атмосферы)
    double windSpeed; ///< скорость ветра, м/с
};

/**
 * @brief Класс для расчета структуры наблюдения
 */
class Observation
{

public:

    Observation();
    Observation(meteoData_t data);

    void Initialize(meteoData_t data);
    void GetObservation(observation_t& observationOut);

private:

    stabilityClass_t _CalcSmithParam(meteoData_t data);
    double _CalcSunAngle(meteoData_t data);
    double _CalcSetOfDay(meteoData_t data);
    double _CalcSunDeclination(int day, int month);
    void _CalcInsolClass(meteoData_t data, double riseTime, double setTime, double sunAngle);
    double _CalcAlbedo(double latitude, month_t month);
    void _CloudCorrection(int cloudAmount, int lowerCloudAmount, double time, double riseTime,
                          double setTime);
    void _FogCorrection(bool fog);
    void _SnowCorrection(bool snow);
    void _CorrectionI();
    void _CorrectionII();
    void _CorrectionIII();
    void _CorrectionIV();
    void _CorrectionV();
    void _CorrectionVI();
    void _CorrectionVII();

    observation_t _observation;
    double _sunDeclination;
    int _insolClass;

};