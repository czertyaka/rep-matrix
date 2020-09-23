/**
 * @file matrix.h
 * @author czertyaka
 * @date 2020-08-10
 * @brief Файл с классои Matrix - основным классом расчетного модуля матрицы повторяемости
 */
#pragma once

#include <iostream>
#include "namespace.h"
#include "stability_cathegory.h"

class Matrix
{

public:

    Matrix(const char* rp5File, double latitude, double longitude);
    ~Matrix() = default;

    meteorology::matrix_t GetMatrix();

private:

    void _CalcMatrix(const char* rp5File, double latitude, double longitude);

    void _ParseTime(std::string localTime, int& day, meteorology::month_t& month, int& year,
                   int& time);
    double _ParseWindSpeed(std::string Ff);
    meteorology::windDir_t _ParseWindDir(std::string DD);
    int _ParseCloudAmount(std::string N);
    bool _ParseFog(std::string VV);
    bool _ParseSnow(std::string E1, std::string E2);

    void _AddObservation(meteorology::observation_t observation);
    int _CalcJ(meteorology::smithParam_t smithParam);
    int _CalcK(double windSpeed);
    int _CalcN(meteorology::windDir_t windDir_t);

    void _NormalizeMatrix();

    meteorology::matrix_t _matrix;
    int _observCounter;

};