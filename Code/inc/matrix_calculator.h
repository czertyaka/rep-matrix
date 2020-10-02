/**
 * @file smith_param_calculator.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */
#pragma once

#include <iostream>
#include <vector>
#include "my_meteorology.h"
#include "smith_param_calculator.h"

class Matrix_Calculator
{

public:

    Matrix_Calculator();
    Matrix_Calculator(std::vector<mm::observation_t>& vObs);

    mm::matrix_t GetMatrix();
    void DataOStream();
    void DataOStream(std::ostream& o);

private:

    void _CalcMatrix();

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

    mm::matrix_t _m;
    std::vector<mm::observation_t>& _vObs;
};