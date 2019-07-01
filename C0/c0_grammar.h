#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include "config.h"
#include <stack>
using namespace std;

class global_Var{
    public:
    int g_address;
    int g_paranum;
    int g_const;
    int g_errorNum;
    void init();
};


class signal{
    public:
        TYPE ty_kwType;
        stack<string> stk_opArg;
        bool b_isConst;
        bool b_isNeg;
        bool b_isMain;
};


class symbol{
    public:
        // name of symbol
        string s_name;
        // 0:const 1:variable 2:function 3:parameters
        int i_type;
        // i_type==0: value of const| i_type=1: 0:int 1:char |i_type==2:function 0:void 1:int
        int i_value;
        int i_address;
        // number of parameters or size of array
        int i_para;
};


class symbolTable{
    public:
        vector<symbol> vec_symbols;
        int i_topIndex;
        int i_totalProgram;
        vector<int> vec_programIndex;

        void symbol_test();
        void insert_symbol(string name, int type, int value, int address, int para, global_Var& gn);
        int searchSymbol(string name, int type, global_Var& gn);
};

#endif