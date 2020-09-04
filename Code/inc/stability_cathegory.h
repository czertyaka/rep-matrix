#ifndef _STAB_CATH_H_
#define _STAB_CATH_H_

#include "namespace.h"

class StabilityCathegory
{

public:

    StabilityCathegory(int day, meteorology::month_t month, int year, double time,
                       double latitude, double longitude,
                       double windSpeed,
                       int cloudAmount, int lowerCloudAmount,
                       bool fog, bool snow);

    meteorology::smithParam_t GetSmithParam();

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

    meteorology::smithParam_t _smithParam; ///< категория усточивости

    int _day; ///< день
    meteorology::month_t _month; ///< месяц
    int _year; ///< год
    double _time; ///< местное время, ч
    double _latitude; ///< широта, град.
    double _longitude; ///< долгота, град.
    double _windSpeed; ///< скорость ветра, м/с
    int _cloudAmount; ///< балл общей облачности, от 1 до 10
    int _lowerCloudAmount; ///< балл нижней облачности, от 1 до 10
    bool _fog; ///< наличие тумана
    bool _snow; ///< наличие сплошного снежного покрова

    double _riseTime; ///< время восхода солнца, ч.
    double _setTime; ///< время захода солнца, ч
    double _sunDecl; ///< солнечное склонение, град.
    double _sunAngle; ///< высота солнца, град.
    int _insolClass; ///< класс инсоляции
};

#endif /* _STAB_CATH_H_ */