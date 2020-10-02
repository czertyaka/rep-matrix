/**
 * @file main.cpp
 * @author czertyaka
 * @date 2020-08-15
 * @brief Главный файл проекта с исходным кодом
 */

#include "string.h"
#include "main.h"

using namespace mm;
using namespace std;

int main(int argc, char* argv[])
{
    vector<observation_t> vObs;
    RP5_CSV_Parser parser(argv[1], vObs, 12.441, 82.565);
    Matrix_Calculator mCalc(vObs);
    if (strcmp(argv[2], "-oc") == 0) {
        std::cout;
        mCalc.DataOStream(cout);
    }
    else if (strcmp(argv[2], "-of") == 0) {
        mCalc.DataOStream();
    }

    return 0;
}