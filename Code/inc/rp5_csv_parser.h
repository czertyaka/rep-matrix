/**
 * @file rp5_csv_parser.h
 * @author czertyaka
 * @brief 
 * @date 2020-09-30
 */

#pragma once

#include "mm_csv_parser.h"
#include <vector>
#include "my_meteorology.h"

class RP5_CSV_Parser : public MM_CSV_Parser
{
public:
    RP5_CSV_Parser(const char* file, std::vector<mm::observation_t>& vObs);
    ~RP5_CSV_Parser() = default;
private:
    void _Parse();
    void _PreParseTasks();
    void _ParseTime();
    void _ParseTemper();
    void _ParseWindSpeed();
    void _ParseWindDir();
    void _ParseCloudAmount();
    void _ParseFog();
    void _ParseSnow();
    const char* _rp5File;
    mm::observation_t _obs;
    std::string _sTime; // местное время
    std::string _sDD; // направление ветра высоте 10-12 м уср. за 10 мин до наблюдения (румбы)
    std::string _sFf; // скорость ветра на высоте 10-12 м уср. за 10 мин до наблюдения (м/с)
    std::string _sN; // общая облачность (%)
    std::string _sNh; // кол-во всех наблюдавшихся Cl (если их нет, кол-во Cm)
    std::string _sVV; // горизонатльная дальность видимости (км)
    std::string _sE1; // состояние поверхности почвы без снега или измеримого ледяного покрова
    std::string _sE2; // состояние поверхности почвы со снегом или измеримым ледяным покровом
    std::string _sPrevE1;
    std::string _sPrevE2;
    std::string _sT; // температура воздуха (град. Цельсия)
};