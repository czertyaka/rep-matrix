/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "main.h"

int main()
{
    meteoData_t data;

    data.latitude = 56.833;
    data.longitude = 60.583;
    data.day = 16;
    data.month = month_t::august;
    data.year = 2020;
    data.time = 17.4556;
    data.windDir = compPoint_t::NNE;
    data.windSpeed = 2;
    data.cloudAmount = 5;
    data.lowerCloudAmount = 3;
    data.fog = false;
    data.snow = false;

    Observation observation(data);

    observation_t obs;
    observation.GetObservation(obs);

    return 0;
}