/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "main.h"

int main()
{

    double latitude = 56.833;
    double longitude = 60.583;
    int day = 16;
    month_t month = month_t::january;
    int year = 2020;
    double time = 17.4556;
    double windSpeed = 0;
    int cloudAmount = 5;
    int lowerCloudAmount = 3;
    bool fog = false;
    bool snow = true;

    StabilityCathegory stabilityCathegory(
        day,
        month,
        year,
        time,
        latitude,
        longitude,
        windSpeed,
        cloudAmount,
        lowerCloudAmount,
        fog,
        snow
    );

    smithParam_t smithParam = stabilityCathegory.GetSmithParam();

    return 0;
}