#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_

#include <cstddef>

namespace meteorology
{
    struct matrix_t
    {
        static const int N = 16; ///< Количество интервалов направления ветра
        static const int J = 7; ///< Количество интервалов категорий устойчивости атмосферы
        static const int K = 8; ///< Количество интервалов скорости ветра

        int mCold[N][J][K] = {0};
        int mWarm[N][J][K] = {0};
        double wCold[N][J][K] = {0};
        double wWarm[N][J][K] = {0};

        matrix_t() {};
        matrix_t(const matrix_t& copy)
        {
            for (std::size_t n = 0; n < N; n++) {
                for (std::size_t j = 0; j < J; j++) {
                    for (std::size_t k = 0; k < K; k++) {
                        mCold[n][j][k] = copy.mCold[n][j][k];
                        mWarm[n][j][k] = copy.mWarm[n][j][k];
                        wCold[n][j][k] = copy.wCold[n][j][k];
                        wWarm[n][j][k] = copy.wWarm[n][j][k];
                    }
                }
            }
        };
    };

    enum compPoint_t : const int
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

    enum month_t : const int
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

    enum smithParam_t : const int
    {
        cathA = 1,
        cathB = 2,
        cathC = 3,
        cathD = 4,
        cathE = 5,
        cathF = 6,
        cathG = 7
    };

    struct observation_t
    {
        int day; ///< день
        month_t month; ///< месяц
        int year; ///< год
        double time; ///< гринвичевское время

        compPoint_t windDir; ///< направление ветра, румб
        smithParam_t smithParam; ///< параметр Смита (категория устойчивости атмосферы)
        double windSpeed; ///< скорость ветра, м/с
    };
}

#endif /* _NAMESPACE_H */