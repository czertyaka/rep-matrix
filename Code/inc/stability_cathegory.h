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

enum smithParam_t : int
{
    cathA = 1,
    cathB = 2,
    cathC = 3,
    cathD = 4,
    cathE = 5,
    cathF = 6,
    cathG = 7
};

class StabilityCathegory
{

public:

    StabilityCathegory(int day, month_t month, int year, double time,
                       double latitude, double longitude,
                       double windSpeed,
                       int cloudAmount, int lowerCloudAmount,
                       bool fog, bool snow);

    smithParam_t GetSmithParam();

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

    smithParam_t _smithParam; ///< категория усточивости

    int _day; ///< день
    month_t _month; ///< месяц
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