#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <stdlib.h>
#include <vector>
#include <string>
#include "config.h"


class optMidcode {
    public:
        std::string s_operation;
        std::string s_alphaVar;
        std::string s_betaVar;
        std::string s_result;
        bool first;
};


class Block {
    public:
    std::vector<optMidcode> blockCodes;
    std::vector<Block> preBlocks;
    std::vector<Block> sucBlocks;
};


class DFG {
    public:
    std::vector<optMidcode> midcodes;
    std::vector<Block> blocks;

    void markFirst();
    void splitBlocks();
    void linkBlocks();
};


class opt
{
    public:
    std::vector<midcode> _exprOptimizer(std::vector<midcode> org_midcodes);
    void _DFG_Analysis(std::vector<midcode> org_midcodes);
};

#endif