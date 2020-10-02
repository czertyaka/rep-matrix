/**
 * @file smith_param_calculator.h
 * @author czertyaka
 * @brief 
 * @date 2020-09-25
 */

#pragma once

#include "my_meteorology.h"

class Smith_Param_Calculator
{

public:

    Smith_Param_Calculator(mm::observation_t& obs);

private:

    void _CalcSmithParam();
    void _CalcSunAngle();
    void _CalcSetOfDay();
    void _CalcSunDeclination();
    void _CalcInsolClass();
    double _CalcAlbedo();
    void _CloudCorrection();
    void _FogCorrection();
    void _SnowCorrection();
    void _CorrectionI();
    void _CorrectionII();
    void _CorrectionIII();
    void _CorrectionIV();
    void _CorrectionV();
    void _CorrectionVI();
    void _CorrectionVII();

    mm::observation_t& _obs;

    double _riseTime; ///< время восхода солнца, ч.
    double _setTime; ///< время захода солнца, ч
    double _sunDecl; ///< солнечное склонение, град.
    double _sunAngle; ///< высота солнца, град.
    int _insolClass; ///< класс инсоляции
};