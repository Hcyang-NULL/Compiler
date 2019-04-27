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

void error(string erro_type){
    cout << erro_type << endl;
    g_errorNum++;
    system("pause");
    exit(1);
}

void look(){
    g_currentToken = g_vec_grammarTokens[0];
    g_vec_grammarTokens.erase(g_iter_grammarTokens);
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

void func_Call(){
    if(match_S("(", true)){
        value_argList();
        if(match_S(")", true)){
            return;
        }
    }
    error("func_Call");
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
    else if(match_S("scanf", true)){
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
    else if(match_S("printf", true)){
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

void arg_List(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        head_State();
        arg_List();
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
        }
        error("return_func_Declare");
    }
    else{
        return;
    }
}

void assist_1(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        return_func_Declare();
        assist_1();
        return;
    }
    else if(match_T(KW_VOID, false)){
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
        assist_2();
        assist_3();
        return;
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

void varible_Declare(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
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

void const_Define(){
    if(match_T(KW_INT, true)){
        if(match_T(TAG, true)){
            if(match_S("=", true)){
                if(match_T(NUM, true)){
                    return;
                }
            }
        }
        error("const_Define branch 1");
    }
    else if(match_T(KW_CHAR, true)){
        if(match_T(TAG, true)){
            if(match_S("=", true)){
                if(match_T(CHAR, true)){
                    return;
                }
            }
        }
        error("const_Define branch 2");
    }
    else if(match_S(",", true)){
        const_Declare();
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

void grammar_analyze(){
    program();
    return;
}

void grammar_initialize(vector<pair<string, TYPE> > arg){
    g_vec_grammarTokens = arg;
    g_iter_grammarTokens = g_vec_grammarTokens.begin();
}