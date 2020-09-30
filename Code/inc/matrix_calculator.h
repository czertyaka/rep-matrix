/**
 * @file smith_param_calculator.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */
#pragma once

#include <iostream>
#include "my_meteorology.h"
#include "smith_param_calculator.h"

class Matrix_Calculator
{

public:

    Matrix_Calculator(mm::matrix_t& matrix, const char* rp5File, double latitude,
                    double longitude);

    void DataOStream();
    void DataOStream(std::ostream& o);

private:

    void _CalcMatrix(const char* rp5File, double latitude, double longitude);

    void _ParseTime(std::string localTime, int& day, mm::month_t& month, int& year,
                   int& time);
    double _ParseTemper(std::string T);
    double _ParseWindSpeed(std::string Ff);
    mm::windDir_t _ParseWindDir(std::string DD);
    int _ParseCloudAmount(std::string N);
    bool _ParseFog(std::string VV);
    bool _ParseSnow(std::string E1, std::string E2);

    void _AddObservation(mm::observation_t observation);
    int _CalcJ(mm::smithParam_t smithParam);
    int _CalcK(double windSpeed);
    int _CalcN(mm::windDir_t windDir_t);

    // расчетные методы
    void _NormalizeMatrix();
    void _CalcWindRose();
    void _CalcWindSpeedRepeatabilityByCompPoint();
    void _CalcWindSpeedRepeatability();
    void _CalcCalmRepeatability();
    void _CalcSmithParamRepeatability();
    void _CalcAverageWindSpeedByCompPoint();
    void _CalcAverageWindSpeedBySmithParam();

    // методы потокового вывода данных
    void _oStreamWindRose(std::ostream& o);
    void _oStreamWindSpeedRepeatability(std::ostream& o);
    void _oStreamCalmRepeatability(std::ostream& o);
    void _oStreamSmithParamRepeatability(std::ostream& o);
    void _oStreamAverageWindSpeedByCompPoint(std::ostream& o);
    void _oStreamAverageWindSpeedBySmithParam(std::ostream& o);

    mm::matrix_t& _matrix;
};