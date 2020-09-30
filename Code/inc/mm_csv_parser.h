/**
 * @file mm_csv_parser.h
 * @author czertyaka
 * @brief 
 * @date 2020-09-25
 */

#pragma once

#include <vector>
#include "my_meteorology.h"

class MM_CSV_Parser
{
protected:
    MM_CSV_Parser(std::vector<mm::observation_t>& vObs);
    ~MM_CSV_Parser() = default;
    std::vector<mm::observation_t>& _vObs;
private:
    virtual void _Parse() = 0;
    virtual void _PreParseTasks() = 0;
};