/**
 * @file matrix_calculator.cpp
 * @author czertyaka
 * @date 2020-08-10
 * @brief Реализация методов Matrix_Calculator
 */

#include "csv.h"
#include <fstream>
#include "matrix_calculator.h"

using namespace std;
using namespace io;
using namespace mm;

Matrix_Calculator::Matrix_Calculator(vector<observation_t>& vObs) : _vObs(vObs)
{
    _CalcMatrix();
}

void Matrix_Calculator::DataOStream()
{
   ofstream fout;
   fout.open("../Output/Роза ветров.csv");
   _oStreamWindRose(fout);
   fout.close();
   fout.open("../Output/Повторяемость модуля скорости ветра по градации k.csv");
   _oStreamWindSpeedRepeatability(fout);
   fout.close();
   fout.open("../Output/Повторяемость штилей.csv");
   _oStreamCalmRepeatability(fout);
   fout.close();
   fout.open("../Output/Повторяемость категорий устойчивости.csv");
   _oStreamSmithParamRepeatability(fout);
   fout.close();
   fout.open("../Output/Приземная средняя скорость ветра n−го румба.csv");
   _oStreamAverageWindSpeedByCompPoint(fout);
   fout.close();
   fout.open("../Output/Приземная средняя скорость ветра при j−ой категории устойчивости.csv");
   _oStreamAverageWindSpeedBySmithParam(fout);
   fout.close();
}

void Matrix_Calculator::DataOStream(ostream& o)
{
    _oStreamWindRose(o);
    o << endl;
    _oStreamWindSpeedRepeatability(o);
    o << endl;
    _oStreamCalmRepeatability(o);
    o << endl;
    _oStreamSmithParamRepeatability(o);
    o << endl;
    _oStreamAverageWindSpeedByCompPoint(o);
    o << endl;
    _oStreamAverageWindSpeedBySmithParam(o);
}

void Matrix_Calculator::_CalcMatrix()
{
    for (vector<observation_t>::iterator iter = _vObs.begin(); iter != _vObs.end(); iter++)
    {
        _AddObservation(*iter);
    }    

    _NormalizeMatrix();
    _CalcWindRose();
    _CalcWindSpeedRepeatabilityByCompPoint();
    _CalcWindSpeedRepeatability();
    _CalcCalmRepeatability();
    _CalcSmithParamRepeatability();
    _CalcAverageWindSpeedByCompPoint();
    _CalcAverageWindSpeedBySmithParam();
}

void Matrix_Calculator::_AddObservation(observation_t observation)
{
    int j = _CalcJ(observation.smithParam);
    int k = _CalcK(observation.windSpeed);
    int n = _CalcN(observation.windDir);

    if (observation.temper < 0) {
        _m.mCold[n][j][k]++;
        _m.MCold++;
        if (k != 0) { _m.MColdNoCalm++; }
    }
    else {
        _m.mWarm[n][j][k]++;
        _m.MWarm++;
        if (k != 0) { _m.MWarmNoCalm++; }
    }
}

int Matrix_Calculator::_CalcJ(smithParam_t smithParam)
{
    switch (smithParam)
    {
    case cathA: return 0;
    case cathB: return 1;
    case cathC: return 2;
    case cathD: return 3;
    case cathE: return 4;
    case cathF: return 5;
    case cathG: return 6;
    default:
        cerr << "Некорректная категория устойчивости: " << (int)smithParam << endl;
        exit(-1);
    }

    return 0;
}

int Matrix_Calculator::_CalcK(double windSpeed)
{
    if (windSpeed < 0) {
        cerr << "Отрицательная скорость ветра: " << windSpeed << " м/с." << endl;
        exit(-1);
        }
    else if (windSpeed < 0.75) { return 0; }
    else if (windSpeed < 1.5)  { return 1; }
    else if (windSpeed < 2.5)  { return 2; }
    else if (windSpeed < 3.5)  { return 3; }
    else if (windSpeed < 5.5)  { return 4; }
    else if (windSpeed < 7.5)  { return 5; }
    else if (windSpeed < 10)   { return 6; }
    else                       { return 7; }

    return 0;
}

int Matrix_Calculator::_CalcN(windDir_t windDir)
{
    switch (windDir)
    {
    case calm: return 0;
    case N:    return 0;
    case NNE:  return 1;
    case NE:   return 2;
    case ENE:  return 3;
    case E:    return 4;
    case ESE:  return 5;
    case SE:   return 6;
    case SSE:  return 7;
    case S:    return 8;
    case SSW:  return 9;
    case SW:   return 10;
    case WSW:  return 11;
    case W:    return 12;
    case WNW:  return 13;
    case NW:   return 14;
    case NNW:  return 15;
    default:
        cerr << "Некорректное направление ветра: " << (int)windDir << endl;
        exit(-1);
    }

    return 0;
}

void Matrix_Calculator::_NormalizeMatrix()
{
    // нормирование для элементов с k > 0
    for (std::size_t n = 0; n < _m.N; n++) {
        for (std::size_t j = 0; j < _m.J; j++) {
            for (std::size_t k = 1; k < _m.K; k++) {
                _m.wCold[n][j][k] = _m.MCold != 0 ?
                                    static_cast<double>(_m.mCold[n][j][k]) /
                                    static_cast<double>(_m.MCold) : 0;
                _m.wWarm[n][j][k] = _m.MWarm != 0 ?
                                    static_cast<double>(_m.mWarm[n][j][k]) /
                                    static_cast<double>(_m.MWarm) : 0;
            }
        }
    }

    // нормирование для элементов с k = 0
    int sum2Cold = 0;
    int sum2Warm = 0;
    for (std::size_t n = 0; n < _m.N; n++) {
        for (std::size_t j = 0; j < _m.J; j++) {
            sum2Cold += _m.mCold[n][j][1];
            sum2Warm += _m.mWarm[n][j][1];
        }
    }
    
    for (std::size_t n = 0; n < _m.N; n++) {

        int sum1Cold = 0;
        int sum1Warm = 0;
        for (std::size_t j = 0; j < _m.J; j++) {
            sum1Cold += _m.mCold[n][j][1];
            sum1Warm += _m.mWarm[n][j][1];
        }

        for (std::size_t j = 0; j < _m.J; j++) {
            _m.wCold[n][j][0] = _m.MCold != 0 ?
                                static_cast<double>(_m.mCold[0][j][0]) /
                                static_cast<double>(_m.MCold) *
                                static_cast<double>(sum1Cold) /
                                static_cast<double>(sum2Cold) :
                                0;
            _m.wWarm[n][j][0] = _m.MWarm != 0 ?
                                static_cast<double>(_m.mWarm[0][j][0]) /
                                static_cast<double>(_m.MWarm) *
                                static_cast<double>(sum1Warm) /
                                static_cast<double>(sum2Warm) :
                                0;
        }
    }

    // проверка нормированности
    double wColdSum = 0;
    double wWarmSum = 0;
    for (std::size_t n = 0; n < _m.N; n++) {
        for (std::size_t j = 0; j < _m.J; j++) {
            for (std::size_t k = 0; k < _m.K; k++) {
                wColdSum += _m.wCold[n][j][k];
                wWarmSum += _m.wWarm[n][j][k];
            }
        }
    }

    if (abs(wColdSum - 1) > 0.0001 && _m.MCold != 0) {
        cerr << "Проверка нормированности холодной матрицы не сходится. Сумма элементов: "
             << wColdSum << endl;
    }
    
    if (abs(wWarmSum - 1) > 0.0001 && _m.MWarm != 0) {
        cerr << "Проверка нормированности теплой матрицы не сходится. Сумма элементов: "
             << wWarmSum << endl;
    }
}

void Matrix_Calculator::_CalcWindRose()
{
    for (size_t n = 0; n < _m.N; n++) {

        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t j = 0; j < _m.J; j++) {
            for (size_t k = 1; k < _m.K; k++) {
                sumCold += _m.mCold[n][j][k];
                sumWarm += _m.mWarm[n][j][k];
            }  
        }

        // calc rose
        _m.windRoseCold[n] = _m.MColdNoCalm != 0 ?
                             static_cast<double>(sumCold) /
                             static_cast<double>(_m.MColdNoCalm) : 0;
        _m.windRoseWarm[n] = _m.MWarmNoCalm != 0 ?
                             static_cast<double>(sumWarm) /
                             static_cast<double>(_m.MWarmNoCalm) : 0;
    }
}

void Matrix_Calculator::_CalcWindSpeedRepeatabilityByCompPoint()
{
    for (size_t n = 0; n < _m.N; n++) {
        for (size_t k = 0; k < _m.K; k++) {
            
            // calc sums
            int sumCold = 0;
            int sumWarm = 0;
            for (size_t j = 0; j < _m.J; j++) {
                sumCold += _m.mCold[n][j][k];
                sumWarm += _m.mWarm[n][j][k];
            }

            // calc reps
            _m.windSpRepByCPCold[n][k] = _m.MCold != 0 ?
                                         static_cast<double>(sumCold) /
                                         static_cast<double>(_m.MCold) : 0;
            _m.windSpRepByCPWarm[n][k] = _m.MWarm != 0 ? 
                                         static_cast<double>(sumWarm) /
                                         static_cast<double>(_m.MWarm) : 0;
        }
    }
}

void Matrix_Calculator::_CalcWindSpeedRepeatability()
{
    for (size_t k = 0; k < _m.K; k++) {
        
        _m.windSpRepCold[k] = 0;
        _m.windSpRepWarm[k] = 0;

        for (size_t n = 0; n < _m.N; n++) {
            
            _m.windSpRepCold[k] += _m.windSpRepByCPCold[n][k];
            _m.windSpRepWarm[k] += _m.windSpRepByCPWarm[n][k];
        }
    }
}

void Matrix_Calculator::_CalcCalmRepeatability()
{
    _m.calmRepCold = _m.windSpRepByCPCold[0][0];    
    _m.calmRepWarm = _m.windSpRepByCPWarm[0][0];
}

void Matrix_Calculator::_CalcSmithParamRepeatability()
{
    for (size_t j = 0; j < _m.J; j++) {
        
        // cals sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t n = 0; n < _m.N; n++) {
            for (size_t k = 0; k < _m.K; k++) {
                sumCold += _m.mCold[n][j][k];
                sumWarm += _m.mWarm[n][j][k];
            }
        }

        // calc reps
        _m.smithParamRepCold[j] = _m.MCold != 0 ?
                                  static_cast<double>(sumCold) /
                                  static_cast<double>(_m.MCold) : 0;
        _m.smithParamRepWarm[j] = _m.MWarm != 0 ?
                                  static_cast<double>(sumWarm) /
                                  static_cast<double>(_m.MWarm) : 0;
    }
    
}

void Matrix_Calculator::_CalcAverageWindSpeedByCompPoint()
{
    double avSpeed[_m.K] = { 0.5, 1, 2, 3, 4.5, 6.5, 9, 12 };

    for (size_t n = 0; n < _m.N; n++) {
        
        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < _m.K; k++) {
            for (size_t j = 0; j < _m.J; j++) {
                sumCold += _m.mCold[n][j][k] * _m.windSpeedVals[k];
                sumWarm += _m.mWarm[n][j][k] * _m.windSpeedVals[k];
            }
        }

        // calc speeds
        _m.avWindSpByCPCold[n] = _m.MCold != 0 ?
                                 static_cast<double>(sumCold) / static_cast<double>(_m.MCold) :
                                 0;
        _m.avWindSpByCPWarm[n] = _m.MCold != 0 ?
                                 static_cast<double>(sumWarm) / static_cast<double>(_m.MWarm) :
                                 0;
    } 
}

void Matrix_Calculator::_CalcAverageWindSpeedBySmithParam()
{
    for (size_t j = 0; j < _m.J; j++) {
        
        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < _m.K; k++) {
            for (size_t n = 0; n < _m.N; n++) {
                sumCold += _m.mCold[n][j][k] * _m.windSpeedVals[k];
                sumWarm += _m.mWarm[n][j][k] * _m.windSpeedVals[k];
            }
        }

        // calc speeds
        _m.avWindSpBySPCold[j] = _m.MCold != 0 ?
                                 static_cast<double>(sumCold) / static_cast<double>(_m.MCold) :
                                 0;
        _m.avWindSpBySPWarm[j] = _m.MWarm != 0 ?
                                 static_cast<double>(sumWarm) / static_cast<double>(_m.MWarm) :
                                 0;
    } 
}

void Matrix_Calculator::_oStreamWindRose(ostream& o)
{
    o << "# повторяемость направлений ветра n −го румба (роза ветров)" << endl
      << "\"Румб, град.\";\"Роза ветров в холодное время года\";\"Роза ветров в теплое время года\"" << endl;
    for (size_t n = 0; n < _m.N; n++) {
        o << "\"" << _m.windDirVals[n] << "\";\""
          << _m.windRoseCold[n] << "\";\""
          << _m.windRoseWarm[n] << "\"" << endl;
    }
}

void Matrix_Calculator::_oStreamWindSpeedRepeatability(ostream& o)
{
    o << "# повторяемость модуля скорости ветра по градации k " << endl
      << "\"Cкорость ветра, м/с\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t k = 0; k < _m.K; k++) {
        o << "\"" << _m.windSpeedVals[k] << "\";\""
          << _m.windSpRepCold[k] << "\";\""
          << _m.windSpRepWarm[k] << "\"" << endl;
    }
}

void Matrix_Calculator::_oStreamCalmRepeatability(ostream& o)
{
    o << "# повторяемость штилей  " << endl
      << "\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl
      << "\"" << _m.calmRepCold << "\";\""
      << "\"" << _m.calmRepWarm << "\"" << endl; 
}

void Matrix_Calculator::_oStreamSmithParamRepeatability(ostream& o)
{
    o << "# повторяемость категорий устойчивости " << endl
      << "\"Категория устойчивости\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t j = 0; j < _m.J; j++) {
        o << "\"" << _m.smithParamVals[j] << "\";\""
          << _m.smithParamRepCold[j] << "\";\""
          << _m.smithParamRepWarm[j] << "\"" << endl;
    }
}

void Matrix_Calculator::_oStreamAverageWindSpeedByCompPoint(ostream& o)
{
    o << "# приземная средняя скорость ветра n−го румба " << endl
      << "\"Румб, град.\";\"Скорость ветра в холодное время года, м/с\";\"Скорость ветра в теплое время года, м/с\"" << endl;
    for (size_t n = 0; n < _m.N; n++) {
        o << "\"" << _m.windDirVals[n] << "\";\""
          << _m.avWindSpByCPCold[n] << "\";\""
          << _m.avWindSpByCPWarm[n] << "\"" << endl;
    }
}

void Matrix_Calculator::_oStreamAverageWindSpeedBySmithParam(ostream& o)
{
    o << "# приземная средняя скорость ветра при j−ой категории устойчивости " << endl
      << "\"Категория устойчивости\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t j = 0; j < _m.J; j++) {
        o << "\"" << _m.smithParamVals[j] << "\";\""
          << _m.avWindSpBySPCold[j] << "\";\""
          << _m.avWindSpBySPWarm[j] << "\"" << endl;
    }
}
