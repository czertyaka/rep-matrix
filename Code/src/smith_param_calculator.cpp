/**
 * @file stability_cathegory.cpp
 * @author your name (you@domain.com)
 * @date 2020-08-16
 * @brief 
 */

#include <cmath>
#include "smith_param_calculator.h"

using namespace meteorology;

double DegToRad(double degree) { return degree / 180 *M_PI; }

SmithParamCalculator::SmithParamCalculator(observation_t& observation):
    _obs(observation)
{
    _CalcSmithParam();
}

void SmithParamCalculator::_CalcSmithParam()
{
    _CalcSunDeclination();
    _CalcSunAngle();
    _CalcSetOfDay();
    _CalcInsolClass();

    if      (_obs.windSpeed >= 0    && _obs.windSpeed < 1.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathG;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathA;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathA;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathA;
        default:    break;
        }
    }
    else if (_obs.windSpeed >= 1.5  && _obs.windSpeed < 2.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathG;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathA;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 2.5  && _obs.windSpeed < 3.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 3.5  && _obs.windSpeed < 4.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathF;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathA;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 4.5  && _obs.windSpeed < 5.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathB;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathB;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 5.5  && _obs.windSpeed < 6.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathE;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathB;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 6.5  && _obs.windSpeed < 7.5)
    {
        switch (_insolClass)
        {
        case -3:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case -2:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case -1:    _obs.smithParam = meteorology::smithParam_t::cathD;
        case 0:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 1:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 2:     _obs.smithParam = meteorology::smithParam_t::cathD;
        case 3:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 4:     _obs.smithParam = meteorology::smithParam_t::cathC;
        case 5:     _obs.smithParam = meteorology::smithParam_t::cathC;
        default: break;
        }
    }
    else if (_obs.windSpeed >= 7.5)
    {
        _obs.smithParam = meteorology::smithParam_t::cathD;
    }
}

void SmithParamCalculator::_CalcSunAngle()
{
    double hourAngle = M_PI * ( _obs.time / 12 - 1 );
    double sunAngleSin = sin(_sunDecl) * sin(DegToRad(_obs.latitude)) +
                         cos(_sunDecl) * cos(DegToRad(_obs.latitude)) *
                         cos(hourAngle);
    _sunAngle = 180 / M_PI * asin(sunAngleSin);
}

void SmithParamCalculator::_CalcSetOfDay()
{
    double beta = - ( 0.0145 + sin(_sunDecl) * sin(DegToRad(_obs.latitude)) ) /
                    ( cos(_sunDecl) * cos(DegToRad(_obs.latitude)) );
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

void SmithParamCalculator::_CalcSunDeclination()
{
    int dayNumber = 30 * ((int)_obs.month - 1) + _obs.day;
    double sunLongitude = 4.909 + 1.705e-2 * dayNumber;
    _sunDecl = asin(0.39795 * sin(sunLongitude));
}

void SmithParamCalculator::_CalcInsolClass()
{
    if (_obs.time > _riseTime + 1 && _obs.time < _setTime - 1) // дневной индекс инсоляции
    {
        if      (_sunAngle >= 0  && _sunAngle < 15) { _insolClass = 1; }
        else if (_sunAngle >= 15 && _sunAngle < 30) { _insolClass = 2; }
        else if (_sunAngle >= 30 && _sunAngle < 45) { _insolClass = 3; }
        else if (_sunAngle >= 45 && _sunAngle < 60) { _insolClass = 4; }
        else if (_sunAngle >= 60)                   { _insolClass = 5; }
    }
    else if (_obs.time < _riseTime && _obs.time > _setTime) // ночной индекс инсоляции
    {
        double timeDiff = _obs.time - _setTime;
        if      (timeDiff >= 0 && timeDiff < 2) {_insolClass = -1 ; }
        else if (timeDiff >= 2 && timeDiff < 7) {_insolClass = -2 ; }
        else if (timeDiff >= 7)                 {_insolClass = -3 ; }
    }
    else // в течение 1 ч до захода и после восхода солнца
    {
        double r = _CalcAlbedo();
        double N = 0.1 * _obs.cloudAmount;

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

double SmithParamCalculator::_CalcAlbedo()
{
    if (_obs.latitude <= 90 && _obs.latitude > 70) { return 0.80; }

    switch (_obs.month)
    {
    case meteorology::january: // зима
    case meteorology::february:
    case meteorology::december:
        if (_obs.latitude <= 70 && _obs.latitude > 60) { return 0.70; }
        if (_obs.latitude <= 60 && _obs.latitude > 50) { return 0.60; }
        if (_obs.latitude <= 50 && _obs.latitude > 40) { return 0.49; }
        if (_obs.latitude <= 40 && _obs.latitude > 30) { return 0.34; }
        if (_obs.latitude <= 30 && _obs.latitude > 20) { return 0.26; }
        if (_obs.latitude <= 20 && _obs.latitude > 10) { return 0.24; }
        if (_obs.latitude <= 10 && _obs.latitude >= 0) { return 0.21; }
    case meteorology::march: // весна
    case meteorology::april:
    case meteorology::may:
        if (_obs.latitude <= 70 && _obs.latitude > 60) { return 0.70; }
        if (_obs.latitude <= 60 && _obs.latitude > 50) { return 0.57; }
        if (_obs.latitude <= 50 && _obs.latitude > 40) { return 0.42; }
        if (_obs.latitude <= 40 && _obs.latitude > 30) { return 0.28; }
        if (_obs.latitude <= 30 && _obs.latitude > 20) { return 0.26; }
        if (_obs.latitude <= 20 && _obs.latitude > 10) { return 0.24; }
        if (_obs.latitude <= 10 && _obs.latitude >= 0) { return 0.21; }
    case meteorology::june: // лето
    case meteorology::july:
    case meteorology::august:
        if (_obs.latitude <= 70 && _obs.latitude > 60) { return 0.26; }
        if (_obs.latitude <= 60 && _obs.latitude > 50) { return 0.18; }
        if (_obs.latitude <= 50 && _obs.latitude > 40) { return 0.20; }
        if (_obs.latitude <= 40 && _obs.latitude > 30) { return 0.22; }
        if (_obs.latitude <= 30 && _obs.latitude > 20) { return 0.24; }
        if (_obs.latitude <= 20 && _obs.latitude > 10) { return 0.22; }
        if (_obs.latitude <= 10 && _obs.latitude >= 0) { return 0.19; }
    case meteorology::september: // осень
    case meteorology::october:
    case meteorology::november:
        if (_obs.latitude <= 70 && _obs.latitude > 60) { return 0.24; }
        if (_obs.latitude <= 60 && _obs.latitude > 50) { return 0.18; }
        if (_obs.latitude <= 50 && _obs.latitude > 40) { return 0.20; }
        if (_obs.latitude <= 40 && _obs.latitude > 30) { return 0.22; }
        if (_obs.latitude <= 30 && _obs.latitude > 20) { return 0.24; }
        if (_obs.latitude <= 20 && _obs.latitude > 10) { return 0.22; }
        if (_obs.latitude <= 10 && _obs.latitude >= 0) { return 0.19; }
    default:
        return 0;
    }
}

void SmithParamCalculator::_CloudCorrection()
{
    bool dayTime = _obs.time > _riseTime && _obs.time < _setTime;

    switch (_obs.cloudAmount)
    {

    case 0:

        switch (_obs.lowerCloudAmount)
        {
        case 0:
            _CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 1:

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

        switch (_obs.lowerCloudAmount)
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

void SmithParamCalculator::_FogCorrection()
{
    if (_obs.fog) { _CorrectionVI(); }
}

void SmithParamCalculator::_SnowCorrection()
{
    if (_obs.snow) { _CorrectionVII(); }
}

/**
 * @brief Поправка индекса инсоляции I
 */
void SmithParamCalculator::_CorrectionI() {}


/**
 * @brief Поправка индекса инсоляции II
 */
void SmithParamCalculator::_CorrectionII()
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
void SmithParamCalculator::_CorrectionIII()
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
void SmithParamCalculator::_CorrectionIV()
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
void SmithParamCalculator::_CorrectionV()
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

void SmithParamCalculator::_CorrectionVI()
{
    _insolClass = 0;
}

void SmithParamCalculator::_CorrectionVII()
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