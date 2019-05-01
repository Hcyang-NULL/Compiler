#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stack>
#include "config.h"
#include "generate_Intercode.cpp"

using namespace std;

vector<pair<string, TYPE> > g_vec_grammarTokens;
vector<pair<string, TYPE> >::iterator g_iter_grammarTokens;
pair<string, TYPE> g_currentToken;
int g_errorNum = 0;

//global handy variable
int i_address;


//symbol table define
class signal{
    public:
        string s_tagName;
        TYPE ty_kwType;
        stack<string> stk_opArg;
        bool b_isConst;
        bool b_isNeg;
        int i_math;  //0:none 1:+ 2:- 3:* 4:/
};
class symbol{
    public:
        // name of symbol
        string s_name;
        // 0:const 1:variable 2:function 3:parameters
        int i_type;
        // i_type==0: value of const i_type==2: 0:void 1:int
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
        void insert_symbol(string name, int type, int value, int address, int para){
            symbol sym_temp;
            sym_temp.s_name = name;
            sym_temp.i_type = type;
            sym_temp.i_value = value;
            sym_temp.i_address = address;
            sym_temp.i_para = para;
            vec_symbols.push_back(sym_temp);
        }
};

symbolTable g_symbolTab;
signal g_signal;
int i_genVarCount = 1;

void sentence();
void condition();
void expression();
void item();
void sentence_List();
void const_Declare();
void head_State();
void assist_2();
void varible_Declare();
void assist_16();
void arg_List();
void number();

void error(string erro_type){
    cout << "Erro: " << erro_type << endl;
    g_errorNum++;
    system("pause");
    exit(1);
}

string genVarName(){
    stringstream ss;
    ss << i_genVarCount;
    i_genVarCount++;
    string name = "$hcy_"+ss.str();
    g_signal.stk_opArg.push(name);
    return name;
}

void look(){
    if(!g_vec_grammarTokens.empty()){
        g_currentToken = g_vec_grammarTokens[0];
        g_vec_grammarTokens.erase(g_iter_grammarTokens);
        cout << "Current Token£º" << g_currentToken.first << " TYPE£º" << g_currentToken.second << endl;
    }
    else{
        cout << "No More Tokens !" << endl;
    }
}

bool match_S(string p, bool move){
    if(g_currentToken.first == p){
        if(move){
            look();
        }
        return true;
    }
    else{
        return false;
    }
}

bool match_T(TYPE p, bool move){
    if(g_currentToken.second == p){
        if(move){
            if(p == TAG){
                g_signal.stk_opArg.push(g_currentToken.first);
            }
            else if(p == NUM || p == CHAR){
                int temp_value;
                string temp_string;
                if(p == CHAR){
                    stringstream ss;
                    temp_value = g_currentToken.first[0];
                    ss << temp_value;
                    temp_string = ss.str();
                }
                else{
                    temp_string = g_currentToken.first;
                }
                if(g_signal.b_isNeg){
                    g_signal.stk_opArg.push("-"+temp_string);
                }
                else{
                    g_signal.stk_opArg.push(temp_string);
                }
            }
            else if(p >= KW_AUTO && p <= KW_SCANF){
                g_signal.ty_kwType = p;
            }
            look();
        }
        return true;
    }
    else{
        return false;
    }
}

string getSTK_Top(){
    if(!g_signal.stk_opArg.empty()){
        string temp = g_signal.stk_opArg.top();
        g_signal.stk_opArg.pop();
        return temp;
    }
    else{
        error("Stack is empty");
    }
}

void assist_7(){
    if(match_T(KW_ELSE, true)){
        sentence();
        return;
    }
    else{
        return;
    }
}

void if_Sentence(){
    if(match_T(KW_IF, true)){
        if(match_S("(", true)){
            condition();
            if(match_S(")", true)){
                sentence();
                assist_7();
                return;
            }
        }
    }
    error("if_Sentence");
}

void assist_15(){
    if(match_S("<", true) || match_S("<=", true) || match_S(">", true) || match_S(">=", true) || match_S("!=", true) || match_S("==", true)){
        expression();
        return;
    }
    else{
        return;
    }
}

void condition(){
    expression();
    assist_15();
    return;
}

void for_Sentence(){
    if(match_T(KW_WHILE, true)){
        if(match_S("(", true)){
            condition();
            if(match_S(")", true)){
                sentence();
                return;
            }
            error("Missing right paren nearing while");
        }
        error("Missing loop-condition");
    }
    error("uncanny mistake in for_Sentence");
}

void condition_kid(){
    if(match_T(KW_CASE, true)){
        if(match_T(CHAR, true)){
            ;
        }
        else{
            number();
        }
        if(match_S(":", true)){
            sentence();
            return;
        }
        error("condition_kid branch 1");
    }
    else{
        error("condition_kid none");
    }
}

void condition_List(){
    if(match_T(KW_CASE, false)){
        condition_kid();
        condition_List();
        return;
    }
    else{
        return;
    }
}

void switch_Sentence(){
    if(match_T(KW_SWITCH, true)){
        if(match_S("(", true)){
            expression();
            if(match_S(")", true)){
                if(match_S("{", true)){
                    condition_List();
                    if(match_S("}", true)){
                        return;
                    }
                }
            }
        }
    }
    error("switch_Sentence");
}

void assist_12(){
    if(match_S(",", true)){
        expression();
        assist_12();
        return;
    }
    else{
        return;
    }
}

void value_argList(){
    if(match_S("+", false) || match_S("-", false) || match_T(TAG, false) || match_T(NUM, false) || match_T(CHAR, false) || match_S("(", false)){
        expression();
        assist_12();
        return;        
    }
    else{
        return;
    }
}

void func_Call(){
    if(match_S("(", true)){
        value_argList();
        if(match_S(")", true)){
            return;
        }
        error("Missing right paren nearing the calling of function");
    }
    error("uncanny mistake in func_Call");
}

void assist_14(){
    if(match_S("[", true)){
        expression();
        if(match_S("]", true)){
            return;
        }
        error("assist_14");
    }
    if(match_S("(", false)){
        func_Call();
    }
    else{
        return;
    }
}

void factor(){
    if(match_T(TAG, true)){
        assist_14();
        return;
    }
    else if(match_S("+", false) || match_S("-", false) || match_T(NUM, false)){
        number();
        return;
    }
    else if(match_T(CHAR, true)){
        return;
    }
    else if(match_S("(", true)){
        expression();
        if(match_S(")", true)){
            return;
        }
        error("factor branch 1");
    }
    else{
        error("factor none");
    }
}

void assist_11(){
    if(match_S("*", true)){
        factor();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("mul", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_11();
        return;
    }
    else if(match_S("/", true)){
        factor();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("div", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_11();
        return;
    }
    else{
        return;
    }
}

void item(){
    factor();
    assist_11();
    return;
}

void assist_10(){
    if(match_S("+", true)){
        item();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("add", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_10();
        return;
    }
    else if(match_S("-", true)){
        item();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("sub", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_10();
        return;
    }
    else{
        return;
    }
}

void expression(){
    if(match_S("+", true)){
        item();
        assist_10();
        return;
    }
    else if(match_S("-", true)){
        item();
        string opArg_temp = getSTK_Top();  //because genVarName will be excute firstly
        genMidcode("sub", "0", opArg_temp, genVarName());
        assist_10();
        return;
    }
    else{
        item();
        assist_10();
        return;
    }
}

void assign_Sentence(){
    if(match_S("=", true)){
        expression();
        return;
    }
    else if(match_S("[", true)){
        expression();
        if(match_S("]", true)){
            if(match_S("=", true)){
                expression();
                return;
            }
        }
        error("assign_Sentence branch 1");
    }
    else{
        error("assign_Sentence none");
    }
}

void assist_13(){
    if(match_S("(", false)){
        func_Call();
        return;
    }
    else if(match_S("=", false) || match_S("[", false)){
        assign_Sentence();
        return;
    }
    else{
        error("assist_13");
    }
}

void assist_4(){
    if(match_S(",", true)){
        if(match_T(TAG, true)){
            assist_4();
            return;
        }
        error("assist_4");
    }
    else{
        return;
    }
}

void assist_9(){
    if(match_S(",", true)){
        expression();
        return;
    }
    else{
        return;
    }
}

void assist_5(){
    if(match_T(STR, true)){
        assist_9();
        return;
    }
    else{
        expression();
        return;
    }
}

void assist_6(){
    if(match_S("(", true)){
        expression();
        if(match_S(")", true)){
            return;
        }
        error("assist_6");
    }
    else{
        return;
    }
}

void sentence(){
    if(match_T(KW_IF, false)){
        if_Sentence();
        return;
    }
    else if(match_T(KW_WHILE, false)){
        for_Sentence();
        return;
    }
    else if(match_S("{", true)){
        sentence_List();
        if(match_S("}", true)){
            return;
        }
        error("sentence branch 1");
    }
    else if(match_T(TAG, true)){
        assist_13();
        if(match_S(";", true)){
            return;
        }
        error("sentence branch 2");
    }
    else if(match_T(KW_SCANF, true)){
        if(match_S("(", true)){
            if(match_T(TAG, true)){
                assist_4();
                if(match_S(")", true)){
                    if(match_S(";", true)){
                        return;
                    }
                }
            }
        }
        error("sentence branch 3");
    }
    else if(match_T(KW_PRINTF, true)){
        if(match_S("(", true)){
            assist_5();
            if(match_S(")", true)){
                if(match_S(";", true)){
                    return;
                }
            }
        }
        error("sentence branch 4");
    }
    else if(match_S(";", true)){
        return;
    }
    else if(match_T(KW_SWITCH, false)){
        switch_Sentence();
        return;
    }
    else if(match_T(KW_RETURN, true)){
        assist_6();
        if(match_S(";", true)){
            return;
        }
        error("sentence branch 5");
    }
    else{
        error("sentence none");
    }
}

void sentence_List(){
    if(match_T(KW_IF, false) || match_T(KW_WHILE, false) || match_S("{", false) || match_T(TAG, false) || match_S("scanf", false) || match_S("printf", false) || match_S(";", false) || match_T(KW_SWITCH, false) || match_T(KW_RETURN, false)){
        sentence();
        sentence_List();
        return;    
    }
    else{
        return;
    }
}

void compound_Sentence(){
    const_Declare();
    varible_Declare();
    sentence_List();
    return;
}

void assist_18(){
    if(match_S(",", true)){
        arg_List();
        return;
    }
    else{
        return;
    }
}

void arg_List(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        head_State();
        assist_18();
        return;
    }
    else{
        return;
    }
}

void void_func_Declare(){
    if(match_T(KW_VOID, true)){
        if(match_T(TAG, true)){
            if(match_S("(", true)){
                arg_List();
                if(match_S(")", true)){
                    if(match_S("{", true)){
                        compound_Sentence();
                        if(match_S("}", true)){
                            return;
                        }
                    }
                }
            }
        }
        error("void_func_Declare");
    }
}

void return_func_Declare(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        head_State();
        if(match_S("(", true)){
            arg_List();
            if(match_S(")", true)){
                if(match_S("{", true)){
                    compound_Sentence();
                    if(match_S("}", true)){
                        return;
                    }
                }

            }
            error("Missing right paren nearing the declaration of function");
        }
        error("uncanny mistake in return_func_Declare");
    }
    else{
        return;
    }
}

bool judge_main(){
    if(g_vec_grammarTokens.size() >= 1){
        if(g_vec_grammarTokens[0].first == "main"){
            return true;
        }
    }
    return false;
}

void assist_1(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        return_func_Declare();
        assist_1();
        return;
    }
    else if(match_T(KW_VOID, false)){
        if(judge_main()){
            return;
        }
        void_func_Declare();
        assist_1();
        return;
    }
    else{
        return;
    }
}

void assist_3(){
    if(match_S(",", true)){
        if(match_T(TAG, true)){
            assist_2();
            assist_3();
            return;
        }
        error("Missing name of variable");
    }
    else{
        return;
    }
}

void assist_2(){
    if(match_S("[", true)){
        if(match_T(NUM, true)){
            if(match_S("]", true)){
                //this is a declaration of array
                if(g_signal.ty_kwType == KW_INT){
                    string opArg_arrayValue = getSTK_Top();
                    genMidcode("inta", "", opArg_arrayValue, getSTK_Top());
                }
                else if(g_signal.ty_kwType == KW_CHAR){
                    string opArg_arrayValue = getSTK_Top();
                    genMidcode("chara", "", opArg_arrayValue, getSTK_Top());
                }
                else{
                    error("the type of declaration is not supported");
                }
                return;
            }
        }
        error("assist_2");
    }
    else{
        //this is a declaration of int or char
        if(g_signal.ty_kwType == KW_INT){
            genMidcode("int", "", "", getSTK_Top());
        }
        else if(g_signal.ty_kwType == KW_CHAR){
            genMidcode("char", "", "", getSTK_Top());
        }
        else{
            error("the type of declaration is not supported");
        }
        return;
    }
}

void type_Iden(){
    if(match_T(KW_INT, true) || match_T(KW_CHAR, true)){
        return;
    }
    else{
        error("type_iden");
    }
}

void head_State(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        type_Iden();
        if(match_T(TAG, true)){
            return;
        }
        error("head_State");
    }
}

bool LL2_func_variable(){
    if(g_vec_grammarTokens.size() >= 2){
        if(g_vec_grammarTokens[0].second == TAG){
            if(g_vec_grammarTokens[1].first == "("){
                return true;
            }
        }
    }
    return false;
}

void varible_Declare(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        //not a function declaration
        if(!LL2_func_variable()){
            head_State();
            assist_2();
            assist_3();
            if(match_S(";", true)){
                varible_Declare();
                return;
            }
            error("varible_Declare");
        }
        else{
            return;
        }
    }
    else{
        return;
    }
}

void assist_17(){
    if(match_S(",", true)){
        assist_16();
    }
    else{
        return;
    }
}

void number(){
    if(match_S("+", true)){
        if(match_T(NUM, true)){
            return;
        }
    }
    else if(match_S("-", true)){
        // recognize negtive number
        g_signal.b_isNeg = true;
        if(match_T(NUM, true)){
            return;
        }
        g_signal.b_isNeg = false;
    }
    else if(match_T(NUM, true)){
        return;
    }
    error("match number");
}

void assist_16(){
    if(match_T(TAG, true)){
        if(match_S("=", true)){
            if(match_S("+", false) || match_S("-", false) || match_T(NUM, false)){
                number();
                if(g_signal.b_isConst){
                    if(g_signal.ty_kwType == KW_INT){
                        string opArg_value = getSTK_Top();
                        genMidcode("const","int", opArg_value, getSTK_Top());
                    }
                    else{
                        error("Not support assign a int value to char type");
                    }
                }
                assist_17();
                return;
            }
            else if(match_T(CHAR, true)){
                if(g_signal.b_isConst){
                    if(g_signal.ty_kwType == KW_CHAR){
                        string opArg_value = getSTK_Top();
                        genMidcode("const","char", opArg_value, getSTK_Top());
                    }
                    else{
                        error("Not support assign a char value to int type");
                    }
                }
                assist_17();
                return;
            }
        }
    }
}

void const_Define(){
    if(match_T(KW_INT, true) || match_T(KW_CHAR, true)){
        g_signal.b_isConst = true;
        assist_16();
        g_signal.b_isConst = false;
    }
    else{
        error("const_Define none");
    }
}

void const_Declare(){
    if(match_T(KW_CONST, true)){
        const_Define();
        if(match_S(";", true)){
            const_Declare();
        }
        else{
            error("const_Declare");
        }
    }
    else{
        return;
    }
}

void main_Func(){
    if(match_T(KW_VOID, true)){
        if(match_S("main", true)){
            genMidcode("func", "", "", "main");
            if(match_S("(", true)){
                if(match_S(")", true)){
                    if(match_S("{", true)){
                        compound_Sentence();
                        if(match_S("}", true)){
                            genMidcode("end", "", "", "main");
                            return;
                        }
                    }
                }
            }
        }
    }
    error("main_Func");
}

void program(){
    look();
    const_Declare();
    varible_Declare();
    assist_1();
    main_Func();
    return;
}

void g_test(){
    mid_out(); 
    cout << g_signal.stk_opArg.size() << endl;
    int size = g_signal.stk_opArg.size();
    for(int i = 0; i < size; i++){
        string temp = getSTK_Top();
        cout << temp << endl;
    }
}

int grammar_analyze(){
    program();
    return g_errorNum;
}

void grammar_initialize(vector<pair<string, TYPE> > arg){
    g_vec_grammarTokens = arg;
    g_iter_grammarTokens = g_vec_grammarTokens.begin();
    g_signal.s_tagName = "";
    g_signal.b_isConst = false;
    g_signal.b_isNeg = false;
    while(!g_signal.stk_opArg.empty()){
        g_signal.stk_opArg.pop();
    }
}