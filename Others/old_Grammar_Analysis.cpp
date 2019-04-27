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
            erro("�ӳ���realarg����");
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
            erro("�ӳ���idexpr����");
        }
    }
    else if(match_S("(")){
        realarg();
        if(match_S(")")){
            return;
        }
        else{
            erro("�ӳ���idexpr����");
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
            erro("�ӳ���elem����");
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
    erro("�ӳ���varrdef����");
}
void defdata(){
    if(!match_T(TAG)){
        erro("�ӳ���defdata����");
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
        erro("�ӳ���deflist����");
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
        erro("ƥ���������ʧ��");
    }
    subprogram();
    if(!match_S("}")){
        erro("ƥ���Ҵ�����ʧ��");
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
            erro("ƥ�����鳤�ȶ������");
        }
        if(!match_S("]")){
            erro("ƥ�����鶨�������ų���");
        }
    }
    else{
        return;
    }
}
void paradata(){
    if(!match_T(TAG)){
        erro("ƥ���ʶ������");
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
            erro("ƥ��')'ʧ��");
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
        erro("ƥ���ʶ������");
    }
    idtail();
}
void type(){
    if(match_S("int")){
        return; 
    }
    erro("ƥ��type����");
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