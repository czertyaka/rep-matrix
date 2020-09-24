/**
 * @file MatrixCalculator.cpp
 * @author czertyaka
 * @date 2020-08-10
 * @brief Реализация методов MatrixCalculator
 */

#include "csv.h"
#include <fstream>
#include "matrix_calculator.h"

using namespace std;
using namespace io;
using namespace meteorology;

MatrixCalculator::MatrixCalculator(matrix_t& matrix, const char* rp5File,
                                   double latitude, double longitude) :
                                   _matrix(matrix)
{
    _CalcMatrix(rp5File, latitude, longitude);
}

void MatrixCalculator::DataOStream()
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

void MatrixCalculator::DataOStream(ostream& o)
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

void MatrixCalculator::_CalcMatrix(const char* rp5File, double latitude, double longitude)
{
    string localTime;   // местное время
    string DD;          // направление ветра высоте 10-12 м уср. за 10 мин до наблюдения (румбы)
    string Ff;          // скорость ветра на высоте 10-12 м уср. за 10 мин до наблюдения (м/с)
    string N;           // общая облачность (%)
    string Nh;          // кол-во всех наблюдавшихся Cl (если их нет, кол-во Cm)
    string VV;          // горизонатльная дальность видимости (км)
    string E1;          // состояние поверхности почвы без снега или измеримого ледяного покрова
    string E2;          // состояние поверхности почвы со снегом или измеримым ледяным покровом
    string prevE1 = ""; 
    string prevE2 = ""; 
    string T;           // температура воздуха (град. Цельсия)

    CSVReader<9, trim_chars<' ', '\t'>, double_quote_escape<';', '"'>, throw_on_overflow,
              single_and_empty_line_comment<'#'> > in(rp5File);

    in.read_header(ignore_extra_column, "Местное время в Аргаяше", "T", "DD", "Ff", "N", "Nh", "VV",
                   "E", "E'");

    // loop reading .csv
    while(in.read_row(localTime, T, DD, Ff, N, Nh, VV, E1, E2))
    {
        // restore ground surface!
        E1 = E1.empty() ? prevE1 : E1;
        E2 = E2.empty() ? prevE2 : E2;
        prevE1 = E1;
        prevE2 = E2;

        // check if we need to skip this observation
        if (
            localTime.empty() || DD.empty() || Ff.empty() || N.empty() || Nh.empty() ||
            VV.empty() || T.empty() ||
            N == "Небо не видно из-за тумана и/или других метеорологических явлений." ||
            Nh == "Небо не видно из-за тумана и/или других метеорологических явлений." ||
            (E1.empty() && E2.empty())
        )
        {
            continue;
        }

        // parse
        observation_t observation;
        _ParseTime(localTime, observation.day, observation.month,
                   observation.year, observation.time);
        observation.latitude = latitude;
        observation.longitude = longitude;
        observation.temper = _ParseTemper(T);
        observation.windDir = _ParseWindDir(DD);
        observation.windSpeed = _ParseWindSpeed(Ff);
        observation.cloudAmount = _ParseCloudAmount(N);
        observation.lowerCloudAmount = _ParseCloudAmount(Nh);
        observation.fog = _ParseFog(VV);
        observation.snow = _ParseSnow(E1, E2);

        // calculate smith parameter
        SmithParamCalculator smithParamCalculator(observation);

        // and finally...
        _AddObservation(observation);
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

void MatrixCalculator::_ParseTime(string localTime, int& day, month_t& month, int& year, int& time)
{
    const char* cLocalTime = localTime.c_str();
    const char* format;
    if      (cLocalTime[3] == '0' && cLocalTime[11] == '0') { format = "%2i.0%i.%4i 0%i:00"; }
    else if (cLocalTime[3] == '0' && cLocalTime[11] != '0') { format = "%2i.0%i.%4i %2i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] == '0') { format = "%2i.%2i.%4i 0%i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] != '0') { format = "%2i.%2i.%4i %2i:00"; }
    sscanf(cLocalTime, format, &day, reinterpret_cast<int*>(&month), &year, &time);
}

double MatrixCalculator::_ParseTemper(string T)
{
    return stod(T);
}

double MatrixCalculator::_ParseWindSpeed(std::string Ff)
{
    return stod(Ff);
}

windDir_t MatrixCalculator::_ParseWindDir(string DD)
{
    windDir_t windDir;

    if      (DD == "Штиль, безветрие")                          { windDir = calm; }
    else if (DD == "Ветер, дующий с севера")                    { windDir = N;    }
    else if (DD == "Ветер, дующий с северо-северо-востока")     { windDir = NNE;  }
    else if (DD == "Ветер, дующий с северо-востока")            { windDir = NE;   }
    else if (DD == "Ветер, дующий с востоко-северо-востока")    { windDir = ENE;  }
    else if (DD == "Ветер, дующий с востока")                   { windDir = E;    }
    else if (DD == "Ветер, дующий с востоко-юго-востока")       { windDir = ESE;  }
    else if (DD == "Ветер, дующий с юго-востока")               { windDir = SE;   }
    else if (DD == "Ветер, дующий с юго-юго-востока")           { windDir = SSE;  }
    else if (DD == "Ветер, дующий с юга")                       { windDir = S;    }
    else if (DD == "Ветер, дующий с юго-юго-запада")            { windDir = SSW;  }
    else if (DD == "Ветер, дующий с юго-запада")                { windDir = SW;   }
    else if (DD == "Ветер, дующий с западо-юго-запада")         { windDir = WSW;  }
    else if (DD == "Ветер, дующий с запада")                    { windDir = W;    }
    else if (DD == "Ветер, дующий с западо-северо-запада")      { windDir = WNW;  }
    else if (DD == "Ветер, дующий с северо-запада")             { windDir = NW;   }
    else if (DD == "Ветер, дующий с северо-северо-запада")      { windDir = NNW;  }
    else
    {
        std::cerr << "Считано некорректное направление ветра: " << DD << std::endl;
        std::exit(-1);
    }

    return windDir;
}

int MatrixCalculator::_ParseCloudAmount(string N)
{
    int cloudAmount;
    
    if      (N == "Облаков нет.")                               { cloudAmount = 0;  }
    else if (N == "10%  или менее, но не 0")                    { cloudAmount = 1;  }
    else if (N == "20–30%.")                                    { cloudAmount = 3;  }
    else if (N == "40%.")                                       { cloudAmount = 4;  }
    else if (N == "50%.")                                       { cloudAmount = 5;  }
    else if (N == "60%.")                                       { cloudAmount = 6;  }
    else if (N == "70 – 80%.")                                  { cloudAmount = 8;  }
    else if (N == "90  или более, но не 100%" || N == "100%.")  { cloudAmount = 10; }
    else
    {
        std::cerr << "Считана некорректная облачность: " << N << std::endl;
        std::exit(-1);
    }

    return cloudAmount;
}

bool MatrixCalculator::_ParseFog(std::string VV)
{
    if (VV == "менее 0.05") { return true; }
    return stod(VV) < 1 ? true : false;
}

bool MatrixCalculator::_ParseSnow(string E1, string E2)
{
    return (
        E2 != "Неровный слой слежавшегося или мокрого снега покрывает почву полностью." &&
        E2 != "Неровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        E2 != "Ровный слой слежавшегося или мокрого снега покрывает поверхность почвы полностью." &&
        E2 != "Ровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        E2 != "Снег покрывает поверхность почвы полностью; глубокие сугробы."
        ) ? false : true;
}

void MatrixCalculator::_AddObservation(observation_t observation)
{
    int j = _CalcJ(observation.smithParam);
    int k = _CalcK(observation.windSpeed);
    int n = _CalcN(observation.windDir);

    if (observation.temper > 0) {
        _matrix.mCold[n][j][k]++;
        _matrix.MCold++;
        if (k != 0) { _matrix.MColdNoCalm++; }
    }
    else {
        _matrix.mWarm[n][j][k]++;
        _matrix.MWarm++;
        if (k != 0) { _matrix.MWarmNoCalm++; }
    }
}

int MatrixCalculator::_CalcJ(smithParam_t smithParam)
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

int MatrixCalculator::_CalcK(double windSpeed)
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

int MatrixCalculator::_CalcN(windDir_t windDir)
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

void MatrixCalculator::_NormalizeMatrix()
{
    // нормирование для элементов с k > 0
    for (std::size_t n = 0; n < _matrix.N; n++) {
        for (std::size_t j = 0; j < _matrix.J; j++) {
            for (std::size_t k = 1; k < _matrix.K; k++) {
                _matrix.wCold[n][j][k] = static_cast<double>(_matrix.mCold[n][j][k]) /
                                         static_cast<double>(_matrix.MCold);
                _matrix.wWarm[n][j][k] = static_cast<double>(_matrix.mWarm[n][j][k]) /
                                         static_cast<double>(_matrix.MWarm);
            }
        }
    }

    // нормирование для элементов с k = 0
    int sum2Cold = 0;
    int sum2Warm = 0;
    for (std::size_t n = 0; n < _matrix.N; n++) {
        for (std::size_t j = 0; j < _matrix.J; j++) {
            sum2Cold += _matrix.mCold[n][j][1];
            sum2Warm += _matrix.mWarm[n][j][1];
        }
    }
    
    for (std::size_t n = 0; n < _matrix.N; n++) {

        int sum1Cold = 0;
        int sum1Warm = 0;
        for (std::size_t j = 0; j < _matrix.J; j++) {
            sum1Cold += _matrix.mCold[n][j][1];
            sum1Warm += _matrix.mWarm[n][j][1];
        }

        for (std::size_t j = 0; j < _matrix.J; j++) {
            _matrix.wCold[n][j][0] = static_cast<double>(_matrix.mCold[0][j][0]) /
                                     static_cast<double>(_matrix.MCold) *
                                     static_cast<double>(sum1Cold) /
                                     static_cast<double>(sum2Cold);
            _matrix.wWarm[n][j][0] = static_cast<double>(_matrix.mWarm[0][j][0]) /
                                     static_cast<double>(_matrix.MWarm) *
                                     static_cast<double>(sum1Warm) /
                                     static_cast<double>(sum2Warm);
        }
    }

    // проверка нормированности
    double wColdSum = 0;
    double wWarmSum = 0;
    for (std::size_t n = 0; n < _matrix.N; n++) {
        for (std::size_t j = 0; j < _matrix.J; j++) {
            for (std::size_t k = 0; k < _matrix.K; k++) {
                wColdSum += _matrix.wCold[n][j][k];
                wWarmSum += _matrix.wWarm[n][j][k];
            }
        }
    }

    if (abs(wColdSum - 1) > 0.0001) {
        cerr << "Проверка нормированности холодной матрицы не сходится. Сумма элементов: "
             << wColdSum << endl;
    }
    
    if (abs(wWarmSum - 1) > 0.0001) {
        cerr << "Проверка нормированности теплой матрицы не сходится. Сумма элементов: "
             << wWarmSum << endl;
    }
}

void MatrixCalculator::_CalcWindRose()
{
    for (size_t n = 0; n < _matrix.N; n++) {

        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t j = 0; j < _matrix.J; j++) {
            for (size_t k = 1; k < _matrix.K; k++) {
                sumCold += _matrix.mCold[n][j][k];
                sumWarm += _matrix.mWarm[n][j][k];
            }  
        }

        // calc rose
        _matrix.windRoseCold[n] = static_cast<double>(sumCold) /
                                  static_cast<double>(_matrix.MColdNoCalm);
        _matrix.windRoseWarm[n] = static_cast<double>(sumWarm) /
                                  static_cast<double>(_matrix.MWarmNoCalm);
    }
}

void MatrixCalculator::_CalcWindSpeedRepeatabilityByCompPoint()
{
    for (size_t n = 0; n < _matrix.N; n++) {
        for (size_t k = 0; k < _matrix.K; k++) {
            
            // calc sums
            int sumCold = 0;
            int sumWarm = 0;
            for (size_t j = 0; j < _matrix.J; j++) {
                sumCold += _matrix.mCold[n][j][k];
                sumWarm += _matrix.mWarm[n][j][k];
            }

            // calc reps
            _matrix.windSpRepByCPCold[n][k] = static_cast<double>(sumCold) /
                                              static_cast<double>(_matrix.MCold);
            _matrix.windSpRepByCPWarm[n][k] = static_cast<double>(sumWarm) /
                                              static_cast<double>(_matrix.MWarm);
        }
    }
}

void MatrixCalculator::_CalcWindSpeedRepeatability()
{
    for (size_t k = 0; k < _matrix.K; k++) {
        
        _matrix.windSpRepCold[k] = 0;
        _matrix.windSpRepWarm[k] = 0;

        for (size_t n = 0; n < _matrix.N; n++) {
            
            _matrix.windSpRepCold[k] += _matrix.windSpRepByCPCold[n][k];
            _matrix.windSpRepWarm[k] += _matrix.windSpRepByCPWarm[n][k];
        }
    }
}

void MatrixCalculator::_CalcCalmRepeatability()
{
    _matrix.calmRepCold = _matrix.windSpRepByCPCold[0][0];    
    _matrix.calmRepWarm = _matrix.windSpRepByCPWarm[0][0];
}

void MatrixCalculator::_CalcSmithParamRepeatability()
{
    for (size_t j = 0; j < _matrix.J; j++) {
        
        // cals sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t n = 0; n < _matrix.N; n++) {
            for (size_t k = 0; k < _matrix.K; k++) {
                sumCold += _matrix.mCold[n][j][k];
                sumWarm += _matrix.mWarm[n][j][k];
            }
        }

        // calc reps
        _matrix.smithParamRepCold[j] = static_cast<double>(sumCold) /
                                       static_cast<double>(_matrix.MCold);
        _matrix.smithParamRepWarm[j] = static_cast<double>(sumWarm) /
                                       static_cast<double>(_matrix.MWarm);
    }
    
}

void MatrixCalculator::_CalcAverageWindSpeedByCompPoint()
{
    double avSpeed[_matrix.K] = { 0.5, 1, 2, 3, 4.5, 6.5, 9, 12 };

    for (size_t n = 0; n < _matrix.N; n++) {
        
        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < _matrix.K; k++) {
            for (size_t j = 0; j < _matrix.J; j++) {
                sumCold += _matrix.mCold[n][j][k] * _matrix.windSpeedVals[k];
                sumWarm += _matrix.mWarm[n][j][k] * _matrix.windSpeedVals[k];
            }
        }

        // calc speeds
        _matrix.avWindSpByCPCold[n] = static_cast<double>(sumCold) / static_cast<double>(_matrix.MCold) ;
        _matrix.avWindSpByCPWarm[n] = static_cast<double>(sumWarm) / static_cast<double>(_matrix.MWarm) ;
    } 
}

void MatrixCalculator::_CalcAverageWindSpeedBySmithParam()
{
    for (size_t j = 0; j < _matrix.J; j++) {
        
        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < _matrix.K; k++) {
            for (size_t n = 0; n < _matrix.N; n++) {
                sumCold += _matrix.mCold[n][j][k] * _matrix.windSpeedVals[k];
                sumWarm += _matrix.mWarm[n][j][k] * _matrix.windSpeedVals[k];
            }
        }

        // calc speeds
        _matrix.avWindSpBySPCold[j] = static_cast<double>(sumCold) / static_cast<double>(_matrix.MCold) ;
        _matrix.avWindSpBySPWarm[j] = static_cast<double>(sumWarm) / static_cast<double>(_matrix.MWarm) ;
    } 
}

void MatrixCalculator::_oStreamWindRose(ostream& o)
{
    o << "# повторяемость направлений ветра n −го румба (роза ветров)" << endl
      << "\"Румб, град.\";\"Роза ветров в холодное время года\";\"Роза ветров в теплое время года\"" << endl;
    for (size_t n = 0; n < _matrix.N; n++) {
        o << "\"" << _matrix.windDirVals[n] << "\";\""
          << _matrix.windRoseCold[n] << "\";\""
          << _matrix.windRoseWarm[n] << "\"" << endl;
    }
}

void MatrixCalculator::_oStreamWindSpeedRepeatability(ostream& o)
{
    o << "# повторяемость модуля скорости ветра по градации k " << endl
      << "\"Cкорость ветра, м/с\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t k = 0; k < _matrix.K; k++) {
        o << "\"" << _matrix.windSpeedVals[k] << "\";\""
          << _matrix.windSpRepCold[k] << "\";\""
          << _matrix.windSpRepWarm[k] << "\"" << endl;
    }
}

void MatrixCalculator::_oStreamCalmRepeatability(ostream& o)
{
    o << "# повторяемость штилей  " << endl
      << "\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl
      << "\"" << _matrix.calmRepCold << "\";\""
      << "\"" << _matrix.calmRepCold << "\"" << endl; 
}

void MatrixCalculator::_oStreamSmithParamRepeatability(ostream& o)
{
    o << "# повторяемость категорий устойчивости " << endl
      << "\"Категория устойчивости\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t j = 0; j < _matrix.J; j++) {
        o << "\"" << _matrix.smithParamVals[j] << "\";\""
          << _matrix.smithParamRepCold[j] << "\";\""
          << _matrix.smithParamRepWarm[j] << "\"" << endl;
    }
}

void MatrixCalculator::_oStreamAverageWindSpeedByCompPoint(ostream& o)
{
    o << "# приземная средняя скорость ветра n−го румба " << endl
      << "\"Румб, град.\";\"Скорость ветра в холодное время года, м/с\";\"Скорость ветра в теплое время года, м/с\"" << endl;
    for (size_t n = 0; n < _matrix.N; n++) {
        o << "\"" << _matrix.windDirVals[n] << "\";\""
          << _matrix.avWindSpByCPCold[n] << "\";\""
          << _matrix.avWindSpByCPWarm[n] << "\"" << endl;
    }
}

void MatrixCalculator::_oStreamAverageWindSpeedBySmithParam(ostream& o)
{
    o << "# приземная средняя скорость ветра при j−ой категории устойчивости " << endl
      << "\"Категория устойчивости\";\"Повторяемость в холодное время года\";\"Повторяемость в теплое время года\"" << endl;
    for (size_t j = 0; j < _matrix.J; j++) {
        o << "\"" << _matrix.smithParamVals[j] << "\";\""
          << _matrix.avWindSpBySPCold[j] << "\";\""
          << _matrix.avWindSpBySPWarm[j] << "\"" << endl;
    }
}
