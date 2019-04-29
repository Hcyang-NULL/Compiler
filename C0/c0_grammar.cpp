#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "config.h"


using namespace std;

vector<pair<string, TYPE> > g_vec_grammarTokens;
vector<pair<string, TYPE> >::iterator g_iter_grammarTokens;
pair<string, TYPE> g_currentToken;
int g_errorNum = 0;

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

void error(string erro_type){
    cout << "发生错误" << erro_type << endl;
    g_errorNum++;
    system("pause");
    exit(1);
}

void look(){
    if(!g_vec_grammarTokens.empty()){
        g_currentToken = g_vec_grammarTokens[0];
        g_vec_grammarTokens.erase(g_iter_grammarTokens);
        cout << "当前token：" << g_currentToken.first << " 类型：" << g_currentToken.second << endl;
    }
    else{
        cout << "Token读取完成" << endl;
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
            look();
        }
        return true;
    }
    else{
        return false;
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

void assist_8(){
    if(match_S("+", true) || match_S("-", true)){
        return;
    }
    else{
        error("assist_8 none");
    }
}

void for_Sentence(){
    if(match_T(KW_FOR, true)){
        if(match_S("(", true)){
            if(match_T(TAG, true)){
                if(match_S("=", true)){
                    expression();
                    if(match_S(";", true)){
                        condition();
                        if(match_S(";", true)){
                            if(match_T(TAG, true)){
                                if(match_S("=", true)){
                                    if(match_T(TAG, true)){
                                        assist_8();
                                        if(match_T(NUM, true)){
                                            if(match_S(")", true)){
                                                sentence();
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    error("for_Sentence");
}

void condition_kid(){
    if(match_T(KW_CASE, true)){
        if(match_T(NUM, true) || match_T(CHAR, true)){
            if(match_S(":", true)){
                sentence();
                return;
            }
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

void default_case(){
    if(match_T(KW_DEFAULT, true)){
        if(match_S(":", true)){
            sentence();
            return;
        }
        error("default_case branch 1");
    }
    else{
        error("default_case none");
    }
}

void switch_Sentence(){
    if(match_T(KW_SWITCH, true)){
        if(match_S("(", true)){
            expression();
            if(match_S(")", true)){
                if(match_S("}", true)){
                    condition_List();
                    default_case();
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
        error("缺失右括号");
    }
    error("函数缺失左括号");
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
    else if(match_T(NUM, true) || match_T(CHAR, true)){
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
    if(match_S("*", true) || match_S("/", true)){
        item();
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
    if(match_S("+", true) || match_S("-", true)){
        item();
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
    else if(match_T(KW_FOR, false)){
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
    if(match_T(KW_IF, false) || match_T(KW_FOR, false) || match_S("{", false) || match_T(TAG, false) || match_S("scanf", false) || match_S("printf", false) || match_S(";", false) || match_T(KW_SWITCH, false) || match_T(KW_RETURN, false)){
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
            error("函数定义缺失右括号");
        }
        error("函数定义缺失左括号");
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
        error("定义时缺失变量名字");
    }
    else{
        return;
    }
}

void assist_2(){
    if(match_S("[", true)){
        if(match_T(NUM, true)){
            if(match_S("]", true)){
                return;
            }
        }
        error("assist_2");
    }
    else{
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

void assist_16(){
    if(match_T(TAG, true)){
        if(match_S("=", true)){
            if(match_T(NUM, true)){
                assist_17();
                return;
            }
        }
    }
}

void const_Define(){
    if(match_T(KW_INT, true)){
        assist_16();
    }
    else if(match_T(KW_CHAR, true)){
        assist_16();
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
            if(match_S("(", true)){
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

int grammar_analyze(){
    program();
    return g_errorNum;
}

void grammar_initialize(vector<pair<string, TYPE> > arg){
    g_vec_grammarTokens = arg;
    g_iter_grammarTokens = g_vec_grammarTokens.begin();
}