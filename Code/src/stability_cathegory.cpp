/**
 * @file stability_cathegory.cpp
 * @author your name (you@domain.com)
 * @date 2020-08-16
 * @brief 
 */

#include <cmath>
#include "stability_cathegory.h"

using namespace meteorology;

double DegToRad(double degree) { return degree / 180 *M_PI; }

StabilityCathegory::StabilityCathegory(observation_t& observation):
    _day(observation.day),
    _month(observation.month),
    _year(observation.year),
    _time(observation.time),
    _latitude(observation.latitude),
    _longitude(observation.longitude),
    _windSpeed(observation.windSpeed),
    _cloudAmount(observation.cloudAmount),
    _lowerCloudAmount(observation.lowerCloudAmount),
    _fog(observation.fog),
    _snow(observation.snow)
{
    _CalcSmithParam();
}

meteorology::smithParam_t StabilityCathegory::GetSmithParam()
{
    return _smithParam;
}

void StabilityCathegory::_CalcSmithParam()
{
    _CalcSunDeclination();
    _CalcSunAngle();
    _CalcSetOfDay();
    _CalcInsolClass();

    if      (_windSpeed >= 0    && _windSpeed < 1.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathG;
        case -2:    _smithParam = meteorology::smithParam_t::cathF;
        case -1:    _smithParam = meteorology::smithParam_t::cathF;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathC;
        case 2:     _smithParam = meteorology::smithParam_t::cathB;
        case 3:     _smithParam = meteorology::smithParam_t::cathA;
        case 4:     _smithParam = meteorology::smithParam_t::cathA;
        case 5:     _smithParam = meteorology::smithParam_t::cathA;
        default:    break;
        }
    }
    else if (_windSpeed >= 1.5  && _windSpeed < 2.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathG;
        case -2:    _smithParam = meteorology::smithParam_t::cathF;
        case -1:    _smithParam = meteorology::smithParam_t::cathE;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathC;
        case 2:     _smithParam = meteorology::smithParam_t::cathB;
        case 3:     _smithParam = meteorology::smithParam_t::cathB;
        case 4:     _smithParam = meteorology::smithParam_t::cathA;
        case 5:     _smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_windSpeed >= 2.5  && _windSpeed < 3.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathF;
        case -2:    _smithParam = meteorology::smithParam_t::cathF;
        case -1:    _smithParam = meteorology::smithParam_t::cathE;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathD;
        case 2:     _smithParam = meteorology::smithParam_t::cathC;
        case 3:     _smithParam = meteorology::smithParam_t::cathB;
        case 4:     _smithParam = meteorology::smithParam_t::cathB;
        case 5:     _smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_windSpeed >= 3.5  && _windSpeed < 4.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathF;
        case -2:    _smithParam = meteorology::smithParam_t::cathE;
        case -1:    _smithParam = meteorology::smithParam_t::cathD;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathD;
        case 2:     _smithParam = meteorology::smithParam_t::cathC;
        case 3:     _smithParam = meteorology::smithParam_t::cathB;
        case 4:     _smithParam = meteorology::smithParam_t::cathB;
        case 5:     _smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_windSpeed >= 4.5  && _windSpeed < 5.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathE;
        case -2:    _smithParam = meteorology::smithParam_t::cathE;
        case -1:    _smithParam = meteorology::smithParam_t::cathD;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathD;
        case 2:     _smithParam = meteorology::smithParam_t::cathC;
        case 3:     _smithParam = meteorology::smithParam_t::cathC;
        case 4:     _smithParam = meteorology::smithParam_t::cathB;
        case 5:     _smithParam = meteorology::smithParam_t::cathB;
        default: break;
        }
    }
    else if (_windSpeed >= 5.5  && _windSpeed < 6.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathE;
        case -2:    _smithParam = meteorology::smithParam_t::cathD;
        case -1:    _smithParam = meteorology::smithParam_t::cathD;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathD;
        case 2:     _smithParam = meteorology::smithParam_t::cathC;
        case 3:     _smithParam = meteorology::smithParam_t::cathC;
        case 4:     _smithParam = meteorology::smithParam_t::cathC;
        case 5:     _smithParam = meteorology::smithParam_t::cathB;
        default: break;
        }
    }
    else if (_windSpeed >= 6.5  && _windSpeed < 7.5)
    {
        switch (_insolClass)
        {
        case -3:    _smithParam = meteorology::smithParam_t::cathD;
        case -2:    _smithParam = meteorology::smithParam_t::cathD;
        case -1:    _smithParam = meteorology::smithParam_t::cathD;
        case 0:     _smithParam = meteorology::smithParam_t::cathD;
        case 1:     _smithParam = meteorology::smithParam_t::cathD;
        case 2:     _smithParam = meteorology::smithParam_t::cathD;
        case 3:     _smithParam = meteorology::smithParam_t::cathC;
        case 4:     _smithParam = meteorology::smithParam_t::cathC;
        case 5:     _smithParam = meteorology::smithParam_t::cathC;
        default: break;
        }
    }
    else if (_windSpeed >= 7.5)
    {
        _smithParam = meteorology::smithParam_t::cathD;
    }
}

void StabilityCathegory::_CalcSunAngle()
{
    double hourAngle = M_PI * ( _time / 12 - 1 );
    double sunAngleSin = sin(_sunDecl) * sin(DegToRad(_latitude)) +
                         cos(_sunDecl) * cos(DegToRad(_latitude)) *
                         cos(hourAngle);
    _sunAngle = 180 / M_PI * asin(sunAngleSin);
}

void StabilityCathegory::_CalcSetOfDay()
{
    double beta = - ( 0.0145 + sin(_sunDecl) * sin(DegToRad(_latitude)) ) /
                    ( cos(_sunDecl) * cos(DegToRad(_latitude)) );
    if (beta > 1)
    {
        beta = 1;
    }
    else if (beta < -1)
    {
        beta = -1;
    }
    _setTime = 12 * (1 + 1/M_PI * acos(beta));
    _riseTime = 24 - _setTime;
}

void StabilityCathegory::_CalcSunDeclination()
{
    int dayNumber = 30 * ((int)_month - 1) + _day;
    double sunLongitude = 4.909 + 1.705e-2 * dayNumber;
    _sunDecl = asin(0.39795 * sin(sunLongitude));
}

void StabilityCathegory::_CalcInsolClass()
{
    if (_time > _riseTime + 1 && _time < _setTime - 1) // дневной индекс инсоляции
    {
        if      (_sunAngle >= 0  && _sunAngle < 15) { _insolClass = 1; }
        else if (_sunAngle >= 15 && _sunAngle < 30) { _insolClass = 2; }
        else if (_sunAngle >= 30 && _sunAngle < 45) { _insolClass = 3; }
        else if (_sunAngle >= 45 && _sunAngle < 60) { _insolClass = 4; }
        else if (_sunAngle >= 60)                   { _insolClass = 5; }
    }
    else if (_time < _riseTime && _time > _setTime) // ночной индекс инсоляции
    {
        double timeDiff = _time - _setTime;
        if      (timeDiff >= 0 && timeDiff < 2) {_insolClass = -1 ; }
        else if (timeDiff >= 2 && timeDiff < 7) {_insolClass = -2 ; }
        else if (timeDiff >= 7)                 {_insolClass = -3 ; }
    }
    else // в течение 1 ч до захода и после восхода солнца
    {
        double r = _CalcAlbedo();
        double N = 0.1 * _cloudAmount;

        if ( (1 - r) * (990 * sin(DegToRad(_sunAngle)) - 30) * (1 - 0.75 * pow(N, 3.4)) >
             110 - 60 * N )
        {
            _insolClass = 0;
        }
        else
        {
            _insolClass = -1;
        }
    }

    _CloudCorrection();
    _FogCorrection();
    _SnowCorrection();
}

double StabilityCathegory::_CalcAlbedo()
{
    if (_latitude <= 90 && _latitude > 70) { return 0.80; }

    switch (_month)
    {
    case meteorology::january: // зима
    case meteorology::february:
    case meteorology::december:
        if (_latitude <= 70 && _latitude > 60) { return 0.70; }
        if (_latitude <= 60 && _latitude > 50) { return 0.60; }
        if (_latitude <= 50 && _latitude > 40) { return 0.49; }
        if (_latitude <= 40 && _latitude > 30) { return 0.34; }
        if (_latitude <= 30 && _latitude > 20) { return 0.26; }
        if (_latitude <= 20 && _latitude > 10) { return 0.24; }
        if (_latitude <= 10 && _latitude >= 0) { return 0.21; }
    case meteorology::march: // весна
    case meteorology::april:
    case meteorology::may:
        if (_latitude <= 70 && _latitude > 60) { return 0.70; }
        if (_latitude <= 60 && _latitude > 50) { return 0.57; }
        if (_latitude <= 50 && _latitude > 40) { return 0.42; }
        if (_latitude <= 40 && _latitude > 30) { return 0.28; }
        if (_latitude <= 30 && _latitude > 20) { return 0.26; }
        if (_latitude <= 20 && _latitude > 10) { return 0.24; }
        if (_latitude <= 10 && _latitude >= 0) { return 0.21; }
    case meteorology::june: // лето
    case meteorology::july:
    case meteorology::august:
        if (_latitude <= 70 && _latitude > 60) { return 0.26; }
        if (_latitude <= 60 && _latitude > 50) { return 0.18; }
        if (_latitude <= 50 && _latitude > 40) { return 0.20; }
        if (_latitude <= 40 && _latitude > 30) { return 0.22; }
        if (_latitude <= 30 && _latitude > 20) { return 0.24; }
        if (_latitude <= 20 && _latitude > 10) { return 0.22; }
        if (_latitude <= 10 && _latitude >= 0) { return 0.19; }
    case meteorology::september: // осень
    case meteorology::october:
    case meteorology::november:
        if (_latitude <= 70 && _latitude > 60) { return 0.24; }
        if (_latitude <= 60 && _latitude > 50) { return 0.18; }
        if (_latitude <= 50 && _latitude > 40) { return 0.20; }
        if (_latitude <= 40 && _latitude > 30) { return 0.22; }
        if (_latitude <= 30 && _latitude > 20) { return 0.24; }
        if (_latitude <= 20 && _latitude > 10) { return 0.22; }
        if (_latitude <= 10 && _latitude >= 0) { return 0.19; }
    default:
        return 0;
    }
}

void StabilityCathegory::_CloudCorrection()
{
    bool dayTime = _time > _riseTime && _time < _setTime;

    switch (_cloudAmount)
    {

    case 0:

        switch (_lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 1:

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

        switch (_lowerCloudAmount)
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

void StabilityCathegory::_FogCorrection()
{
    if (_fog) { _CorrectionVI(); }
}

void StabilityCathegory::_SnowCorrection()
{
    if (_snow) { _CorrectionVII(); }
}

/**
 * @brief Поправка индекса инсоляции I
 */
void StabilityCathegory::_CorrectionI() {}


/**
 * @brief Поправка индекса инсоляции II
 */
void StabilityCathegory::_CorrectionII()
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
void StabilityCathegory::_CorrectionIII()
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
void StabilityCathegory::_CorrectionIV()
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
void StabilityCathegory::_CorrectionV()
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

void StabilityCathegory::_CorrectionVI()
{
    _insolClass = 0;
}

void StabilityCathegory::_CorrectionVII()
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