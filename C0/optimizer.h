#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <stdlib.h>
#include <vector>
#include <string>
#include "config.h"

class opt
{
    public:
    static std::vector<midcode> _exprOptimizer(std::vector<midcode> orig_midcodes);
};

#endif