/**
 * @file rp5_csv_parser.cpp
 * @author czertyaka
 * @brief 
 * @date 2020-09-30
 */

#include <string>
#include "rp5_csv_parser.h"
#include "csv.h"

using namespace io;
using namespace std;
using namespace mm;

RP5_CSV_Parser::RP5_CSV_Parser(const char* file, vector<observation_t>& vObs) :
                               MM_CSV_Parser(vObs),
                               _rp5File(file),
                               _obs({0}),
                               _sPrevE1(""),
                               _sPrevE2("")
{
    _PreParseTasks();
    _Parse();
}

void RP5_CSV_Parser::_PreParseTasks()
{

}

void RP5_CSV_Parser::_Parse()
{
    CSVReader<9, trim_chars<' ', '\t'>, double_quote_escape<';', '"'>, throw_on_overflow,
              single_and_empty_line_comment<'#'> > in(_rp5File);

    in.read_header(ignore_extra_column, "Местное время в Аргаяше", "T", "DD", "Ff", "N", "Nh", "VV",
                   "E", "E'");

    // loop reading .csv
    while(in.read_row(_sTime, _sT, _sDD, _sFf, _sN, _sNh, _sVV, _sE1, _sE2))
    {
        // restore ground surface!
        _sE1 = _sE1.empty() ? _sPrevE1 : _sE1;
        _sE2 = _sE2.empty() ? _sPrevE2 : _sE2;
        _sPrevE1 = _sE1;
        _sPrevE2 = _sE2;

        // check if we need to skip this observation
        if (
            _sTime.empty() || _sDD.empty() || _sFf.empty() || _sN.empty() || _sNh.empty() ||
            _sVV.empty() || _sT.empty() ||
            _sN == "Небо не видно из-за тумана и/или других метеорологических явлений." ||
            _sNh == "Небо не видно из-за тумана и/или других метеорологических явлений." ||
            (_sE1.empty() && _sE2.empty())
        )
        {
            continue;
        }

        bool debug = _obs.time == 11 && _obs.month == september && _obs.year == 2019;

        // parse
        try {
            _ParseTime();
            _ParseTemper();
            _ParseWindDir();
            _ParseWindSpeed();
            _ParseCloudAmount();
            _ParseFog();
            _ParseSnow();
        }
        catch(string message) {
            cerr << message << endl
                 << "Измерение от " << _sTime << " не будет включено в расчет в связи с некорректными значениями в файле данных." << endl;
            continue;
        }
        catch(...) {
            cerr << "Измерение от " << _sTime << " не будет включено в расчет в связи с некорректными значениями в файле данных." << endl;
            continue;
        }

        // add
        _vObs.push_back(_obs);
    }
}

void RP5_CSV_Parser::_ParseTime()
{
    const char* cLocalTime = _sTime.c_str();
    const char* format;
    if      (cLocalTime[3] == '0' && cLocalTime[11] == '0') { format = "%2i.0%i.%4i 0%i:00"; }
    else if (cLocalTime[3] == '0' && cLocalTime[11] != '0') { format = "%2i.0%i.%4i %2i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] == '0') { format = "%2i.%2i.%4i 0%i:00"; }
    else if (cLocalTime[3] != '0' && cLocalTime[11] != '0') { format = "%2i.%2i.%4i %2i:00"; }
    sscanf(cLocalTime, format, &_obs.day, reinterpret_cast<int*>(&_obs.month), &_obs.year,
           &_obs.time);
}

void RP5_CSV_Parser::_ParseTemper()
{
    _obs.temper = stod(_sT);
}

void RP5_CSV_Parser::_ParseWindSpeed()
{
    _obs.windSpeed = stod(_sFf);
}

void RP5_CSV_Parser::_ParseWindDir()
{
    windDir_t windDir;

    if      (_sDD == "Штиль, безветрие")                          { windDir = calm; }
    else if (_sDD == "Ветер, дующий с севера")                    { windDir = N;    }
    else if (_sDD == "Ветер, дующий с северо-северо-востока")     { windDir = NNE;  }
    else if (_sDD == "Ветер, дующий с северо-востока")            { windDir = NE;   }
    else if (_sDD == "Ветер, дующий с востоко-северо-востока")    { windDir = ENE;  }
    else if (_sDD == "Ветер, дующий с востока")                   { windDir = E;    }
    else if (_sDD == "Ветер, дующий с востоко-юго-востока")       { windDir = ESE;  }
    else if (_sDD == "Ветер, дующий с юго-востока")               { windDir = SE;   }
    else if (_sDD == "Ветер, дующий с юго-юго-востока")           { windDir = SSE;  }
    else if (_sDD == "Ветер, дующий с юга")                       { windDir = S;    }
    else if (_sDD == "Ветер, дующий с юго-юго-запада")            { windDir = SSW;  }
    else if (_sDD == "Ветер, дующий с юго-запада")                { windDir = SW;   }
    else if (_sDD == "Ветер, дующий с западо-юго-запада")         { windDir = WSW;  }
    else if (_sDD == "Ветер, дующий с запада")                    { windDir = W;    }
    else if (_sDD == "Ветер, дующий с западо-северо-запада")      { windDir = WNW;  }
    else if (_sDD == "Ветер, дующий с северо-запада")             { windDir = NW;   }
    else if (_sDD == "Ветер, дующий с северо-северо-запада")      { windDir = NNW;  }
    else
    {
        string msg = "Считано некорректное направление ветра: " + _sDD;
        throw  msg;
    }

    _obs.windDir = windDir;
}

void RP5_CSV_Parser::_ParseCloudAmount()
{
    int cloudAmount;
    int lowerCloudAmount;
    
    if      (_sN == "Облаков нет.")                                { cloudAmount = 0;  }
    else if (_sN == "10%  или менее, но не 0")                     { cloudAmount = 1;  }
    else if (_sN == "20–30%.")                                     { cloudAmount = 3;  }
    else if (_sN == "40%.")                                        { cloudAmount = 4;  }
    else if (_sN == "50%.")                                        { cloudAmount = 5;  }
    else if (_sN == "60%.")                                        { cloudAmount = 6;  }
    else if (_sN == "70 – 80%.")                                   { cloudAmount = 8;  }
    else if (_sN == "90  или более, но не 100%" || _sN == "100%." || _sN == "100%.") { cloudAmount = 10; }
    else
    {
        string msg = "Считана некорректная общая облачность: " + _sN;
        throw msg;
    }

    if      (_sNh == "Облаков нет.")                                { lowerCloudAmount = 0;  }
    else if (_sNh == "10%  или менее, но не 0")                     { lowerCloudAmount = 1;  }
    else if (_sNh == "20–30%.")                                     { lowerCloudAmount = 3;  }
    else if (_sNh == "40%.")                                        { lowerCloudAmount = 4;  }
    else if (_sNh == "50%.")                                        { lowerCloudAmount = 5;  }
    else if (_sNh == "60%.")                                        { lowerCloudAmount = 6;  }
    else if (_sNh == "70 – 80%.")                                   { lowerCloudAmount = 8;  }
    else if (_sNh == "90  или более, но не 100%" || _sN == "100%.") { lowerCloudAmount = 10; }
    else
    {
        string msg = "Считана некорректная нижняя облачность: " + _sNh;
        throw msg;
    }

    _obs.cloudAmount = cloudAmount;
    _obs.lowerCloudAmount = lowerCloudAmount;
}

void RP5_CSV_Parser::_ParseFog()
{
    if (_sVV == "менее 0.05") { _obs.fog = true; }
    else                      { _obs.fog = stod(_sVV) < 1 ? true : false; }
}

void RP5_CSV_Parser::_ParseSnow()
{
    _obs.snow = (
        _sE2 != "Неровный слой слежавшегося или мокрого снега покрывает почву полностью." &&
        _sE2 != "Неровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        _sE2 != "Ровный слой слежавшегося или мокрого снега покрывает поверхность почвы полностью." &&
        _sE2 != "Ровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        _sE2 != "Снег покрывает поверхность почвы полностью; глубокие сугробы."
        ) ? false : true;
}