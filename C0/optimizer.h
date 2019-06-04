#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <stdlib.h>
#include <vector>
#include <string>
#include "config.h"
#include "c0_grammar.h"
#include "generate_intercode.h"

#define NAC 0.1
#define UNDEF 0.2

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
    std::vector<double> inVals;
    std::vector<double> outVals;
};


class DFG {
    public:
    string funcName;
    std::vector<optMidcode> midcodes;
    std::vector<optMidcode> paras;
    std::vector<Block> blocks;

    void markFirst();
    void splitBlocks();
    void linkBlocks();
};


class opt {
    public:
    std::vector<midcode> _exprOptimizer(std::vector<midcode> org_midcodes);
    void _DFG_Analysis(std::vector<midcode> org_midcodes);
};


class var {
    public:
    string name;
    int index;
};

var makeVar();

class ConstProp {
    public:
    DFG dfg;
    vector<var> vars;
    int glb_num;
    vector<double> boundVals;
    vector<double> initVals;

    void init(DFG dfg, vector<midcode> glbmicos);
    void analyze();

    private:
    double join(double left, double right);
    void join(Block& b);
    bool translate(Block& b);
    void translate(optMidcode m, vector<double>& in, vector<double>& out);
    int searchVar(string s);
};

#endif