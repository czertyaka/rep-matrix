#pragma once

#include <cstddef>
#include <iostream>

namespace mm
{
    enum windDir_t : const int
    {
        calm = 0,
        N    = 1,
        NNE  = 2,
        NE   = 3,
        ENE  = 4,
        E    = 5,
        ESE  = 6,
        SE   = 7,
        SSE  = 8,
        S    = 9,
        SSW  = 10,
        SW   = 11,
        WSW  = 12,
        W    = 13,
        WNW  = 14,
        NW   = 15,
        NNW  = 16
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
        int time; ///< гринвичевское время

        double latitude; ///< широта, град.
        double longitude; ///< долгота, град.

        windDir_t windDir; ///< направление ветра, румб
        smithParam_t smithParam; ///< параметр Смита (категория устойчивости атмосферы)
        double windSpeed; ///< скорость ветра, м/с

        int cloudAmount; ///< балл общей облачности, от 0 до 10
        int lowerCloudAmount; ///< балл нижней облачности, от 0 до 10
        bool fog; ///< наличие тумана
        bool snow; ///< наличие сплошного снежного покрова

        double temper; ///< температура, град. Цельсия
    };

    struct matrix_t
    {
        static const int N = 16; ///< Количество интервалов направления ветра
        static const int J = 7; ///< Количество интервалов категорий устойчивости атмосферы
        static const int K = 8; ///< Количество интервалов скорости ветра

        const double windDirVals[N] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225,
                                       247.5, 270, 292.5, 315, 337.5};
        const std::string smithParamVals[J] = {"A", "B", "C", "D", "E", "F", "G"};
        const double windSpeedVals[K] = { 0.5, 1, 2, 3, 4.5, 6.5, 9, 12 };

        int mCold[N][J][K] = {0}; ///< ненормированная матрица холодного времени года
        int mWarm[N][J][K] = {0}; ///< ненормированная матрица теплого времени года
        double wCold[N][J][K] = {0}; ///< нормированная матрица холодного времени года
        double wWarm[N][J][K] = {0}; ///< нормированная матрица теплого времени года

        int MCold = 0; ///< количество наблюдений в холодное время года
        int MWarm = 0; ///< количество наблюдений в теплое время года
        int MColdNoCalm = 0; ///< количество наблюдений в холодное время года без штилей
        int MWarmNoCalm = 0; ///< количество наблюдений в теплое время года без штилей

        double windRoseCold[N] = {0}; ///< повт. напр. ветра n-го румба в холодное время года
        double windRoseWarm[N] = {0}; ///< повт. напр. ветра n-го румба в теплое время года

        double windSpRepByCPCold[N][K] = {0}; ///< повт. скор. ветра k по напр. n в холодное время года
        double windSpRepByCPWarm[N][K] = {0}; ///< повт. скор. ветра k по напр. n в теплое время года

        double windSpRepCold[K] = {0}; ///< повт. скор. ветра k в холодное время года
        double windSpRepWarm[K] = {0}; ///< повт. скор. ветра k в теплое время года

        double calmRepCold = 0; ///< повторяемость штилей в холодное время года
        double calmRepWarm = 0; ///< повторяемость штилей в теплое время года

        double smithParamRepCold[J] = {0};; ///< повторяемость категорий устойчивости в холодное время года
        double smithParamRepWarm[J] = {0};; ///< повторяемость категорий устойчивости в теплое время года

        double avWindSpByCPCold[N] = {0};; ///< ср. скорость ветра n-го румб в холодное время года
        double avWindSpByCPWarm[N] = {0};; ///< ср. скорость ветра n-го румб в теплое время года

        double avWindSpBySPCold[J] = {0}; ///< ср. скор. ветра при j−ой кат. уст. в холодное время года
        double avWindSpBySPWarm[J] = {0}; ///< ср. скор. ветра при j−ой кат. уст. в теплое время года
    };
}