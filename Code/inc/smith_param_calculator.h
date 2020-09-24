#pragma once

#include "namespace.h"

class SmithParamCalculator
{

public:

    SmithParamCalculator(meteorology::observation_t& observation);

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

    meteorology::observation_t& _obs;

    double _riseTime; ///< время восхода солнца, ч.
    double _setTime; ///< время захода солнца, ч
    double _sunDecl; ///< солнечное склонение, град.
    double _sunAngle; ///< высота солнца, град.
    int _insolClass; ///< класс инсоляции
};