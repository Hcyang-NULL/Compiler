#include <iostream>
#include <stdlib.h>
#include "config.h"
#include <vector>
#include <stack>
#include <string>
#include "sample_intercode.cpp"

using namespace std;

vector<pair<string, TYPE> > g_tokens;
vector<pair<string, TYPE> >::iterator start;
pair<string, TYPE> g_token;
stack<string> g_vars;
int erro_num = 0;

void Tdot();
void E();
void F();

string get_optvar(){
    string res = g_vars.top();
    g_vars.pop();
    return res;
}

void genRES(string new_var){
    g_vars.push(new_var);
}

void look(){
    g_token = g_tokens[0];
    g_tokens.erase(start);
}

bool match_S(string p, bool push){
    if(g_token.first == p){
        if(push){
            g_vars.push(g_token.first);
        }
        look();
        return true;
    }
    else{
        return false;
    }
}

bool match_T(TYPE p, bool push){
    if(g_token.second == p){
        if(push){
            g_vars.push(g_token.first);
        }
        look();
        return true;
    }
    else{
        return false;
    }
}

void erro(string erro_type){
    cout << erro_type << endl;
    erro_num++;
    system("pause");
    exit(1);
}

void Tdot(){
    if(match_S("*", false)){
        F();
        Tdot();
        if(erro_num == 0){
            genRES(genMUL(get_optvar(), get_optvar()));
        }
    }
    else{
        return;
    }
}

void F(){
    if(match_S("(", false)){
        E();
        if(match_S(")", false)){
            return;
        }
        else{
            erro("Tdot子程序缺失右括号");
        }
    }
    else if(match_T(NUM, true)){
        return;
    }
    else{
        erro("Tdot子程序无法匹配");
    }
}

void T(){
    F();
    Tdot();
}

void Edot(){
    if(match_S("+", false)){
        T();
        Edot();
        if(erro_num == 0){
            genRES(genADD(get_optvar(), get_optvar()));
        }
    }
    else{
        return;
    }
}

void E(){
    T();
    Edot();
}

vector<struct inter_code> grammar_analyze(){
    look();
    E();
    cout << "语法分析完成" << endl;
    return finish();
}

void grammar_initialize(vector<pair<string, TYPE> > arg){
    g_tokens = arg;
    start = g_tokens.begin();
}