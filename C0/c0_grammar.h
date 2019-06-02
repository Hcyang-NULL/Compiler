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
    void init() {
        g_address = 0;
        g_paranum = 0;
        g_const = 0;
        g_errorNum = 0;
    }
};


void error(string erro_type, global_Var g){
    cout << "Erro: " << erro_type << endl;
    g.g_errorNum++;
    system("pause");
    exit(1);
}


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
    public:
        void symbol_test(){
            cout << "------- Symbol Table --------" << endl;
            cout << "Total : " << i_topIndex << endl;
            cout << "Total function : " << i_totalProgram << endl;
            cout << "-----------------------------" << endl;
            cout << "Global defination :" << endl;
            for(int i = 0; i < vec_programIndex[0]; i++){
                cout << vec_symbols[i].s_name;
                switch(vec_symbols[i].i_type){
                    case 0:cout << " [type->Const";break;
                    case 1:cout << " [type->Variable";break;
                    case 2:cout << " [type->Error-funciton";break;
                    case 3:cout << " [type->Error-parameter";break;
                    default:cout << " [type->Error-none";break;
                }
                symbol temp = vec_symbols[i];
                cout << " value->" << temp.i_value << " address->" << temp.i_address << " para->" << temp.i_para << "]";
                cout << endl;
            }
            cout << "----------------------------" << endl;
            for(int i = 0; i < i_totalProgram; i++){
                int j = vec_programIndex[i];
                int limit = -1;
                if(i == i_totalProgram-1){
                    limit = i_topIndex;
                }
                else{
                    limit = vec_programIndex[i+1];
                }
                cout << "Function: " << vec_symbols[j].s_name;
                if(vec_symbols[j].i_value == 0){
                    cout << " type->void";
                }
                else if(vec_symbols[j].i_value == 1){
                    cout << " type->int";
                }
                else{
                    cout << " type->char";
                }
                cout << " para-num->" << vec_symbols[j].i_para << endl;
                for(j++; j < limit; j++){
                    cout << vec_symbols[j].s_name;
                    switch(vec_symbols[j].i_type){
                        case 0:cout << " [type->Erro-const";break;
                        case 1:cout << " [type->Variable";break;
                        case 2:cout << " [type->Error-funciton";break;
                        case 3:cout << " [type->parameter";break;
                        default:cout << " [type->Error-none";break;
                    }
                    symbol temp = vec_symbols[j];
                    cout << " value->" << temp.i_value << " address->" << temp.i_address << " para->" << temp.i_para << "]";
                    cout << endl;
                }
                cout << "----------------------------" << endl;
            }
        }
        void insert_symbol(string name, int type, int value, int address, int para, global_Var gn){
            if(type == 2){
                //insert a name of function
                for(int i = 0; i < i_totalProgram; i++){
                    if(vec_symbols[vec_programIndex[i]].s_name == name){
                        error("Multiply defination of function", gn);
                        return;
                    }
                }
                vec_programIndex.push_back(i_topIndex);
                i_totalProgram++;
                gn.g_paranum = 0;
            }
            else{
                //insert a name of variable
                if(i_totalProgram != 0){
                    for(int i = vec_programIndex[i_totalProgram-1]; i < i_topIndex; i++){
                        if(vec_symbols[i].s_name == name){
                            error("Multiply defination of variable", gn);
                            return;
                        }
                    }
                }
            }
            //real insert operation
            symbol sym_temp;
            sym_temp.s_name = name;
            sym_temp.i_type = type;
            cout << "test-type = " << type << endl;
            sym_temp.i_value = value;
            sym_temp.i_address = address;
            sym_temp.i_para = para;
            i_topIndex++;
            vec_symbols.push_back(sym_temp);
        }
        int searchSymbol(string name, int type, global_Var gn){
            if(type == 2){
                //search a name of function
                for(int i = 0; i < i_totalProgram; i++){
                    if(vec_symbols[vec_programIndex[i]].s_name == name){
                        if(vec_symbols[vec_programIndex[i]].i_para == gn.g_paranum){
                            return 1;
                        }
                        else{
                            return 0;
                        }
                    }
                }
                return -1;
            }
            else{
                //search a name of variable
                for(int i = vec_programIndex[i_totalProgram-1]; i < i_topIndex; i++){
                    if(vec_symbols[i].s_name == name){
                        return vec_symbols[i].i_value;
                    }
                }
                for(int i = 0; i < vec_programIndex[0]; i++){
                    if(vec_symbols[i].s_name == name){
                        if(vec_symbols[i].i_type == 0){
                            gn.g_const = 1;
                        }
                        else{
                            gn.g_const = 0;
                        }
                        return vec_symbols[i].i_value;
                    }
                }
                return -2;
            }
        }
};

#endif