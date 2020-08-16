/**
 * @file observation.cpp
 * @author your name (you@domain.com)
 * @date 2020-08-16
 * @brief 
 */

#include <cmath>
#include "obseravtion.h"

double DegToRad(double degree) { return degree / 180 *M_PI; }

/**
 * @brief Construct a new observation t::observation t object by default
 */
Observation::Observation() : _observation({0, month_t::january, 0, 0, compPoint_t::N,
                                          stabilityClass_t::cathA, 0}),
                             _sunDeclination(0),
                             _insolClass(0) {}

/**
 * @brief Construct a new observation t::observation t object with parameter
 * @details Создает объект, соответсвующий входным данным
 * @param data Метеоданные, на основе которых рассчитываются пол объекта
 */
Observation::Observation(meteoData_t data)
{
    this->Initialize(data);
}

/**
 * @brief Метод, рассчитывающий направление и скорость ветра и категорию устойчивости на основе
 * входных метеоданных
 * @param data Метеоданные, на основе которых рассчитываются пол объекта
 */
void Observation::Initialize(meteoData_t data)
{
    _sunDeclination = _CalcSunDeclination(data.day, data.month);
    _insolClass = 0;

    _observation.day   = data.day;
    _observation.month = data.month;
    _observation.year  = data.year;
    _observation.time  = data.time;

    _observation.windDir     = data.windDir;
    _observation.windSpeed   = data.windSpeed;
    _observation.smithParam  = _CalcSmithParam(data);
}

/**
 * @brief Выдает данные поля _observation
 * 
 * @param observationOut Ссылка, в которую копируются значения
 */
void Observation::GetObservation(observation_t& observationOut)
{
    observationOut.day          = _observation.day;
    observationOut.month        = _observation.month;
    observationOut.year         = _observation.year;
    observationOut.time         = _observation.time;
    observationOut.windDir      = _observation.windDir;
    observationOut.windSpeed    = _observation.windSpeed;
    observationOut.smithParam   = _observation.smithParam;
}

/**
 * @brief Метод рассчитывает и возваращает категорию неустойчивости атмосферы
 * 
 * @param data Исходные данные для расчета
 * @return double Категория неустойчивости атмосферы
 */
stabilityClass_t Observation::_CalcSmithParam(meteoData_t data)
{
    double sunAngle = _CalcSunAngle(data);
    double setOfDay = _CalcSetOfDay(data);
    double riseOfDay = 24 - setOfDay;
    _CalcInsolClass(data, riseOfDay, setOfDay, sunAngle);

    if      (data.windSpeed >= 0    && data.windSpeed < 1.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathG;
        case -2:    return stabilityClass_t::cathF;
        case -1:    return stabilityClass_t::cathF;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathC;
        case 2:     return stabilityClass_t::cathB;
        case 3:     return stabilityClass_t::cathA;
        case 4:     return stabilityClass_t::cathA;
        case 5:     return stabilityClass_t::cathA;
        default:    break;
        }
    }
    else if (data.windSpeed >= 1.5  && data.windSpeed < 2.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathG;
        case -2:    return stabilityClass_t::cathF;
        case -1:    return stabilityClass_t::cathE;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathC;
        case 2:     return stabilityClass_t::cathB;
        case 3:     return stabilityClass_t::cathB;
        case 4:     return stabilityClass_t::cathA;
        case 5:     return stabilityClass_t::cathA;
        default: break;
        }
    }
    else if (data.windSpeed >= 2.5  && data.windSpeed < 3.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathF;
        case -2:    return stabilityClass_t::cathF;
        case -1:    return stabilityClass_t::cathE;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathD;
        case 2:     return stabilityClass_t::cathC;
        case 3:     return stabilityClass_t::cathB;
        case 4:     return stabilityClass_t::cathB;
        case 5:     return stabilityClass_t::cathA;
        default: break;
        }
    }
    else if (data.windSpeed >= 3.5  && data.windSpeed < 4.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathF;
        case -2:    return stabilityClass_t::cathE;
        case -1:    return stabilityClass_t::cathD;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathD;
        case 2:     return stabilityClass_t::cathC;
        case 3:     return stabilityClass_t::cathB;
        case 4:     return stabilityClass_t::cathB;
        case 5:     return stabilityClass_t::cathA;
        default: break;
        }
    }
    else if (data.windSpeed >= 4.5  && data.windSpeed < 5.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathE;
        case -2:    return stabilityClass_t::cathE;
        case -1:    return stabilityClass_t::cathD;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathD;
        case 2:     return stabilityClass_t::cathC;
        case 3:     return stabilityClass_t::cathC;
        case 4:     return stabilityClass_t::cathB;
        case 5:     return stabilityClass_t::cathB;
        default: break;
        }
    }
    else if (data.windSpeed >= 5.5  && data.windSpeed < 6.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathE;
        case -2:    return stabilityClass_t::cathD;
        case -1:    return stabilityClass_t::cathD;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathD;
        case 2:     return stabilityClass_t::cathC;
        case 3:     return stabilityClass_t::cathC;
        case 4:     return stabilityClass_t::cathC;
        case 5:     return stabilityClass_t::cathB;
        default: break;
        }
    }
    else if (data.windSpeed >= 6.5  && data.windSpeed < 7.5)
    {
        switch (_insolClass)
        {
        case -3:    return stabilityClass_t::cathD;
        case -2:    return stabilityClass_t::cathD;
        case -1:    return stabilityClass_t::cathD;
        case 0:     return stabilityClass_t::cathD;
        case 1:     return stabilityClass_t::cathD;
        case 2:     return stabilityClass_t::cathD;
        case 3:     return stabilityClass_t::cathC;
        case 4:     return stabilityClass_t::cathC;
        case 5:     return stabilityClass_t::cathC;
        default: break;
        }
    }
    else if (data.windSpeed >= 7.5)
    {
        return stabilityClass_t::cathD;
    }
}

/**
 * @brief Метод расчета высоты солнца
 * 
 * @param data Исходные данные для расчета
 * @return double Высота солнца, град.
 */
double Observation::_CalcSunAngle(meteoData_t data)
{
    double hourAngle = M_PI * ( data.time / 12 - 1 );
    double sunAngleSin = sin(_sunDeclination) * sin(DegToRad(data.latitude)) + 
                         cos(_sunDeclination) * cos(DegToRad(data.latitude)) *
                         cos(hourAngle);
    return 180 / M_PI * asin(sunAngleSin);
}

/**
 * @brief Метод расчета времени захода солнца
 * 
 * @param data Исходные данные для расчета
 * @return double Время захода солнца, ч
 */
double Observation::_CalcSetOfDay(meteoData_t data)
{
    double beta = - ( 0.0145 + sin(_sunDeclination) * sin(DegToRad(data.latitude)) ) /
                    ( cos(_sunDeclination) * cos(DegToRad(data.latitude)) );
    if (beta > 1)
    {
        beta = 1;
    }
    else if (beta < -1)
    {
        beta = -1;
    }
    double setOfDay = 12 * (1 + 1/M_PI * acos(beta));
    return setOfDay;
}

/**
 * @brief Расчет солнечного склонения
 * 
 * @param data Исходные данные для расчета
 * @return double Солнечное склонение, рад.
 */
double Observation::_CalcSunDeclination(int day, int month)
{
    int dayNumber = 30 * ((int)month - 1) + day;
    double sunLongitude = 4.909 + 1.705e-2 * dayNumber;
    double sunDeclination = asin(0.39795 * sin(sunLongitude));
    return sunDeclination;
}

/**
 * @brief Метод расчета класса инсоляции
 * 
 * @param data Исходные данные для расчета
 * @param riseTime Время восхода солнца, ч
 * @param setTime Время захода солнца, ч
 * @param sunAngle Высота солнца, град.
 */
void Observation::_CalcInsolClass(meteoData_t data, double riseTime, double setTime,
                                  double sunAngle)
{
    if (data.time > riseTime + 1 && data.time < setTime - 1) // дневной индекс инсоляции
    {
        if      (sunAngle >= 0  && sunAngle < 15)   { _insolClass = 1; }
        else if (sunAngle >= 15 && sunAngle < 30)   { _insolClass = 2; }
        else if (sunAngle >= 30 && sunAngle < 45)   { _insolClass = 3; }
        else if (sunAngle >= 45 && sunAngle < 60)   { _insolClass = 4; }
        else if (sunAngle >= 60)                    { _insolClass = 5;}
    }
    else if (data.time < riseTime && data.time > setTime) // ночной индекс инсоляции
    {
        double timeDiff = data.time - setTime;
        if      (timeDiff >= 0 && timeDiff < 2) {_insolClass = -1 ; }
        else if (timeDiff >= 2 && timeDiff < 7) {_insolClass = -2 ; }
        else if (timeDiff >= 7)                 {_insolClass = -3 ; }
    }
    else // в течение 1 ч до захода и после восхода солнца
    {
        double r = _CalcAlbedo(data.latitude, data.month);
        double N = 0.1 * data.cloudAmount;

        if ( (1 - r) * (990 * sin(DegToRad(sunAngle)) - 30) * (1 - 0.75 * pow(N, 3.4)) >
             110 - 60 * N )
        {
            _insolClass = 0;
        }
        else
        {
            _insolClass = -1;
        }
    }

    _CloudCorrection(data.cloudAmount, data.lowerCloudAmount, data.time, riseTime, setTime);
    _FogCorrection(data.fog);
    _SnowCorrection(data.snow);
}

/**
 * @brief Метод считает альбедо подстилающей поверхности
 * 
 * @param latitude Широта, град.
 * @param month Месяц
 * @return double Альбедо подстилающей поверхности
 */
double Observation::_CalcAlbedo(double latitude, month_t month)
{
    if (latitude <= 90 && latitude > 70) { return 0.80; }

    switch (month)
    {
    case january: // зима
    case february:
    case december:
        if (latitude <= 70 && latitude > 60) { return 0.70; }
        if (latitude <= 60 && latitude > 50) { return 0.60; }
        if (latitude <= 50 && latitude > 40) { return 0.49; }
        if (latitude <= 40 && latitude > 30) { return 0.34; }
        if (latitude <= 30 && latitude > 20) { return 0.26; }
        if (latitude <= 20 && latitude > 10) { return 0.24; }
        if (latitude <= 10 && latitude >= 00) { return 0.21; }
        break;
    case march: // весна
    case april:
    case may:
        if (latitude <= 70 && latitude > 60) { return 0.70; }
        if (latitude <= 60 && latitude > 50) { return 0.57; }
        if (latitude <= 50 && latitude > 40) { return 0.42; }
        if (latitude <= 40 && latitude > 30) { return 0.28; }
        if (latitude <= 30 && latitude > 20) { return 0.26; }
        if (latitude <= 20 && latitude > 10) { return 0.24; }
        if (latitude <= 10 && latitude >= 00) { return 0.21; }
        break;
    case june: // лето
    case july:
    case august:
        if (latitude <= 70 && latitude > 60) { return 0.26; }
        if (latitude <= 60 && latitude > 50) { return 0.18; }
        if (latitude <= 50 && latitude > 40) { return 0.20; }
        if (latitude <= 40 && latitude > 30) { return 0.22; }
        if (latitude <= 30 && latitude > 20) { return 0.24; }
        if (latitude <= 20 && latitude > 10) { return 0.22; }
        if (latitude <= 10 && latitude >= 00) { return 0.19; }
        break;
    case september: // осень
    case october:
    case november:
        if (latitude <= 70 && latitude > 60) { return 0.24; }
        if (latitude <= 60 && latitude > 50) { return 0.18; }
        if (latitude <= 50 && latitude > 40) { return 0.20; }
        if (latitude <= 40 && latitude > 30) { return 0.22; }
        if (latitude <= 30 && latitude > 20) { return 0.24; }
        if (latitude <= 20 && latitude > 10) { return 0.22; }
        if (latitude <= 10 && latitude >= 00) { return 0.19; }
        break;
    default:
        break;
    }
}

/**
 * @brief Поправка индекса исноляции на облачность
 * 
 * @param cloudAmount Балл общей облачности
 * @param lowerCloudAmount Балл нижней облачности
 * @param time Время, ч
 * @param riseTime Время восхода солнца, ч
 * @param setTime Время захода солнца, ч
 */
void Observation::_CloudCorrection(int cloudAmount, int lowerCloudAmount, double time,
                                   double riseTime, double setTime)
{
    bool dayTime = time > riseTime && time < setTime;

    switch (cloudAmount)
    {

    case 0:

        switch (lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 1:

        switch (lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        case 1:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 2:
    case 3:

        switch (lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        case 1:
            _CorrectionI();
            break;
        case 2:
        case 3:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 4:

        switch (lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        case 1:
            _CorrectionI();
            break;
        case 2:
        case 3:
            _CorrectionI();
            break;
        case 4:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 5:

        switch (lowerCloudAmount)
        {
        case 0:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 1:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 4:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 5:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        default:
            break;
        }

        break;

    case 6:

        switch (lowerCloudAmount)
        {
        case 0:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 1:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 4:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 5:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 6:
            _CorrectionII();
            break;
        default:
            break;
        }

        break;

    case 7:
    case 8:

        switch (lowerCloudAmount)
        {
        case 0:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 1:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 4:
            _CorrectionII();
            break;
        case 5:
            _CorrectionII();
            break;
        case 6:
            _CorrectionII();
            break;
        case 7:
        case 8:
            _CorrectionIV();
            break;
        default:
            break;
        }

        break;

    case 9:

        switch (lowerCloudAmount)
        {
        case 0:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 1:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { _CorrectionI(); } else { _CorrectionII(); }
            break;
        case 4:
            if (dayTime) { _CorrectionII(); } else { _CorrectionIII(); }
            break;
        case 5:
            if (dayTime) { _CorrectionII(); } else { _CorrectionIII(); }
            break;
        case 6:
            _CorrectionIV();
            break;
        case 7:
        case 8:
            _CorrectionIV();
            break;
        case 9:
            _CorrectionIV();
            break;
        default:
            break;
        }

        break;

    case 10:

        switch (lowerCloudAmount)
        {
        case 0:
            _CorrectionIII();
            break;
        case 1:
            _CorrectionIII();
            break;
        case 2:
        case 3:
            _CorrectionIII();
            break;
        case 4:
            _CorrectionIII();
            break;
        case 5:
            _CorrectionIV();
            break;
        case 6:
            _CorrectionIV();
            break;
        case 7:
        case 8:
            _CorrectionIV();
            break;
        case 9:
            _CorrectionV();
            break;
        case 10:
            _CorrectionV();
            break;
        default:
            break;
        }

        break;

    default:
        break;
    }
}

/**
 * @brief Поправка индекса инсоляции на туман
 */
void Observation::_FogCorrection(bool fog)
{
    if (fog) { _CorrectionVI(); }
}

/**
 * @brief Поправка индекса инсоляции на снежный покров
 */
void Observation::_SnowCorrection(bool snow)
{
    if (snow) { _CorrectionVII(); }
}

/**
 * @brief Поправка индекса инсоляции I
 */
void Observation::_CorrectionI() {}


/**
 * @brief Поправка индекса инсоляции II
 */
void Observation::_CorrectionII()
{
    switch (_insolClass)
    {
    case -3:
    case -2:
        _insolClass++;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        _insolClass--;
        break;
    default:
        break;
    }
}

/**
 * @brief Поправка индекса инсоляции III
 */
void Observation::_CorrectionIII()
{
    switch (_insolClass)
    {
    case -3:
    case -2:
        _insolClass = -1;
        break;
    case 2:
        _insolClass = 1;
    case 3:
    case 4:
    case 5:
        _insolClass--;
        break;
    default:
        break;
    }
}

/**
 * @brief Поправка индекса инсоляции IV
 */
void Observation::_CorrectionIV()
{
    switch (_insolClass)
    {
    case -3:
    case -2:
        _insolClass = -1;
        break;
    case 2:
        _insolClass = 1;
    case 3:
    case 4:
    case 5:
        _insolClass -= 2;
        break;
    default:
        break;
    }
}

/**
 * @brief Поправка индекса инсоляции V
 */
void Observation::_CorrectionV()
{
    switch (_insolClass)
    {
    case -3:
    case -2:
    case -1:
    case 1:
        _insolClass = 0;
        break;
    case 2:
    case 3:
    case 4:
        _insolClass = 1;
    case 5:
        _insolClass = 2;
        break;
    default:
        break;
    }
}

/**
 * @brief Поправка индекса инсоляции VI
 */
void Observation::_CorrectionVI()
{
    _insolClass = 0;
}

/**
 * @brief Поправка индекса инсоляции VII
 */
void Observation::_CorrectionVII()
{
    switch (_insolClass)
    {
    case -1:
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        _insolClass--;
        break;
    case 5:
        _insolClass = 3;
        break;
    default:
        break;
    }
}