#include<iostream>
#include<stdlib.h>
#include<vector>
#include<string>
#include"config.h"

using namespace std;

vector<pair<string, TYPE> > tokens;
vector<pair<string, TYPE> >::iterator start;
pair<string, TYPE> token;
int erro_num = 0;

void erro(string erro_type){
    cout << erro_type << endl;
    erro_num++;
    exit(1);
}

bool match_S(string p){
    if(token.first == p){
        scan();
        return true;
    }
    else{
        return false;
    }
}

bool match_T(TYPE p){
    if(token.second == p){
        scan();
        return true;
    }
    else{
        return false;
    }
}

void scan(){
    token = tokens[0];
    tokens.erase(start);
}



void literal(){
    if(match_T(NUM)){
        return;
    }
}

void realarg(){
    if(match_S("!") || match_S("-") || match_S("&") || match_S("*")
     || match_S("++") || match_S("--")){
         factor();
    }
    else if(match_T(TAG)){
        idexpr();
    }
    else if(match_S("(")){
        expr();
        if(match_S(")")){
            return;
        }
        else{
            erro("子程序：realarg错误");
        }
    }
    else if(match_T(NUM)){
        return;
    }
    else{
        return;
    }
}

void idexpr(){
    if(match_S("[")){
        expr();
        if(match_S("]")){
            return;
        }
        else{
            erro("子程序：idexpr错误");
        }
    }
    else if(match_S("(")){
        realarg();
        if(match_S(")")){
            return;
        }
        else{
            erro("子程序：idexpr错误");
        }
    }
    else{
        return;
    }
}

void elem(){
    if(match_T(TAG)){
        idexpr();
    }
    else if(match_S("(")){
        expr();
        if(match_S(")")){
            return;
        }
        else{
            erro("子程序：elem错误");
        }
    }
    else{
        literal();
    }
}

void rop(){
    if(match_S("++") || match_S("--")){
        int a;
        ++--a;
        return;
    }
    else{
        return;
    }
}

void val(){
    elem();
    rop();
}

void factor(){
    if(match_S("!") || match_S("-") || match_S("&") || match_S("*") || match_S("++") || match_S("--")){
        factor();        
    }
    else{
        val();
    }
}

void itemtail(){
    if(match_S("*") || match_S("\\") || match_S("%")){
        factor();
        itemtail();
    }
    else{
        return;
    }
}

void item(){
    factor();
    itemtail();
}



void alotail(){
    if(match_S("+") || match_S("-")){
        item();
        alotail();
    }
    else{
        return;
    }
}

void aloexpr(){
    item();
    alotail();
}

void cmptail(){
    if(match_S("<") || match_S(">") || match_S("<=") || match_S(">=") || match_S("==") || match_S("!=")){
        aloexpr();
        cmptail();        
    }
    else{
        return;
    }
}
void cmpexpr(){
    aloexpr();
    cmptail();
}
void andtail(){
    if(match_S("&")){
        cmpexpr();
        andtail();
    }
    else{
        return;
    }
}
void andexpr(){
    cmpexpr();
    andtail();
}
void ortail(){
    if(match_S("||")){
        andexpr();
        ortail();
    }
    else{
        return;
    }
}
void orexpr(){
    andexpr();
    ortail();
}
void asstail(){
    if(match_S("=")){
        orexpr();
        asstail();
    }
    else{
        return;
    }
}
void assexpr(){
    orexpr();
    asstail();
}
void expr(){
    assexpr();
}
void init(){
    if(match_S("=")){
        expr();
    }
    else{
        return;
    }
}
void varrdef(){
    if(match_S("[")){
        if(match_T(NUM)){
            if(match_S("]")){
                return;
            }
        }
    }
    else{
        init();
        return;
    }
    erro("子程序：varrdef错误");
}
void defdata(){
    if(!match_T(TAG)){
        erro("子程序：defdata错误");
    }
    varrdef();
}
void deflist(){
    if(match_S(";")){
        return;
    }
    else if(match_S(",")){
        defdata();
        deflist();
    }
    else{
        erro("子程序：deflist错误");
    }
}
void localdef(){
    defdata();
    deflist();
}

void subprogram(){
    if(match_S("int")){
        localdef();
    }
    else if(TODO:statement){

    }
}

void block(){
    if(!match_S("{")){
        erro("匹配左大括号失败");
    }
    subprogram();
    if(!match_S("}")){
        erro("匹配右大括号失败");
    }
}
void funtail(){
    if(match_S(";")){
        return;
    }
    else{
        block();
    }
}
void paradatatail(){
    if(match_S("[")){
        if(!match_T(NUM)){
            erro("匹配数组长度定义出错");
        }
        if(!match_S("]")){
            erro("匹配数组定义右括号出错");
        }
    }
    else{
        return;
    }
}
void paradata(){
    if(!match_T(TAG)){
        erro("匹配标识符出错");
    }
    paradatatail();
}
void paralist(){
    if(match_S(",")){
        type();
        paradata();
        paralist();
    }
    else{
        return;
    }
}
void para(){
    if(match_S("int")){
        paradata();
        paralist();
    }
    else{
        return;
    }
}
void idtail(){
    if(match_S("(")){
        para();
        if(!match_S(")")){
            erro("匹配')'失败");
        }
        funtail();
    }
    else{
        varrdef();
        deflist();
    }
}
void def(){
    if(!match_T(TAG)){
        erro("匹配标识符出错");
    }
    idtail();
}
void type(){
    if(match_S("int")){
        return; 
    }
    erro("匹配type出错");
}
void segment(){
    type();
    def();
}
void program(){
    if(token.second == END){
        return;
    }
    else{
        segment();
        program();
    }
}
void grammar_analyze(){
    scan();
    program();
}

void grammar_initialize(vector<pair<string, TYPE> > arg){
    tokens = arg;
    start = tokens.begin();
}