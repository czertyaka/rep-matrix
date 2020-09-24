/**
 * @file matrix.cpp
 * @author czertyaka
 * @date 2020-08-10
 * @brief Реализация методов Matrix
 */

#include "csv.h"
#include "matrix.h"

using namespace std;
using namespace io;
using namespace meteorology;

Matrix::Matrix(const char* rp5File, double latitude, double longitude)
{
    _CalcMatrix(rp5File, latitude, longitude);
}

matrix_t Matrix::GetMatrix()
{
    return _matrix;
}

void Matrix::_CalcMatrix(const char* rp5File, double latitude, double longitude)
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
        StabilityCathegory smithParam(observation);
        observation.smithParam = smithParam.GetSmithParam();

        // and finally...
        _AddObservation(observation);
    }

    _NormalizeMatrix();
}

void Matrix::_ParseTime(string localTime, int& day, month_t& month, int& year, int& time)
{
    const char* cLocalTime = localTime.c_str();
    const char* format;
    if      (cLocalTime[3] == '0' && cLocalTime[11] == '0') { format = "%2i.0%i.%4i 0%i:00"; }
    else if (cLocalTime[3] == '0' && cLocalTime[11] != '0') { format = "%2i.0%i.%4i %2i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] == '0') { format = "%2i.%2i.%4i 0%i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] != '0') { format = "%2i.%2i.%4i %2i:00"; }
    sscanf(cLocalTime, format, &day, reinterpret_cast<int*>(&month), &year, &time);
}

double Matrix::_ParseTemper(string T)
{
    return stod(T);
}

double Matrix::_ParseWindSpeed(std::string Ff)
{
    return stod(Ff);
}

windDir_t Matrix::_ParseWindDir(string DD)
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

int Matrix::_ParseCloudAmount(string N)
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

bool Matrix::_ParseFog(std::string VV)
{
    if (VV == "менее 0.05") { return true; }
    return stod(VV) < 1 ? true : false;
}

bool Matrix::_ParseSnow(string E1, string E2)
{
    return (
        E2 != "Неровный слой слежавшегося или мокрого снега покрывает почву полностью." &&
        E2 != "Неровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        E2 != "Ровный слой слежавшегося или мокрого снега покрывает поверхность почвы полностью." &&
        E2 != "Ровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        E2 != "Снег покрывает поверхность почвы полностью; глубокие сугробы."
        ) ? false : true;
}

void Matrix::_AddObservation(observation_t observation)
{
    int j = _CalcJ(observation.smithParam);
    int k = _CalcK(observation.windSpeed);
    int n = _CalcN(observation.windDir);

    if (observation.temper > 0) { _matrix.mCold[n][j][k]++; }
    else                        { _matrix.mWarm[n][j][k]++; }
}

int Matrix::_CalcJ(smithParam_t smithParam)
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

int Matrix::_CalcK(double windSpeed)
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

int Matrix::_CalcN(windDir_t windDir)
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

void Matrix::_NormalizeMatrix()
{
    int MCold = 0;
    int MWarm = 0;
    for (std::size_t n = 0; n < _matrix.N; n++) {
        for (std::size_t j = 0; j < _matrix.J; j++) {
            for (std::size_t k = 0; k < _matrix.K; k++) {
                MCold += _matrix.mCold[n][j][k];
                MWarm += _matrix.mWarm[n][j][k];
            }
        }
    }

    // нормирование для элементов с k > 0
    for (std::size_t n = 0; n < _matrix.N; n++) {
        for (std::size_t j = 0; j < _matrix.J; j++) {
            for (std::size_t k = 1; k < _matrix.K; k++) {
                _matrix.wCold[n][j][k] = static_cast<double>(_matrix.mCold[n][j][k]) /
                                         static_cast<double>(MCold);
                _matrix.wWarm[n][j][k] = static_cast<double>(_matrix.mWarm[n][j][k]) /
                                         static_cast<double>(MWarm);
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
                                     static_cast<double>(MCold) *
                                     static_cast<double>(sum1Cold) /
                                     static_cast<double>(sum2Cold);
            _matrix.wWarm[n][j][0] = static_cast<double>(_matrix.mWarm[0][j][0]) /
                                     static_cast<double>(MWarm) *
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