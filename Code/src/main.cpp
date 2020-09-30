/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "main.h"

using namespace mm;
using namespace std;

int main()
{
    vector<observation_t> vObs;
    RP5_CSV_Parser parser("/home/czert/workspace/rep-matrix/RP5-files/big-utf-8.csv", vObs);

    return 0;
}