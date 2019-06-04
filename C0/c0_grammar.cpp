#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "generate_Intercode.cpp"
#include "c0_grammar.h"

using namespace std;

vector<pair<string, TYPE> > g_vec_grammarTokens;
vector<pair<string, TYPE> >::iterator g_iter_grammarTokens;
pair<string, TYPE> g_currentToken;

global_Var g;
symbolTable g_symbolTab;
signal g_signal;

int i_genVarCount = 1;
int i_genLableCount = 1;

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

void error(string erro_type, global_Var g){
    cout << "Erro: " << erro_type << endl;
    g.g_errorNum++;
    system("pause");
    exit(1);
}

void symbolTable::symbol_test(){
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

void symbolTable::insert_symbol(string name, int type, int value, int address, int para, global_Var gn){
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


int symbolTable::searchSymbol(string name, int type, global_Var gn){
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


void global_Var::init() {
    g_address = 0;
    g_paranum = 0;
    g_const = 0;
    g_errorNum = 0;
}

string genVarName(){
    stringstream ss;
    ss << i_genVarCount;
    i_genVarCount++;
    string name = "$hcy_"+ss.str();
    g_signal.stk_opArg.push(name);
    return name;
}

string genLableName(){
    stringstream ss;
    ss << i_genLableCount;
    i_genLableCount++;
    string name = "_LABLE_"+ss.str();
    return name;
}

void look(){
    if(!g_vec_grammarTokens.empty()){
        g_currentToken = g_vec_grammarTokens[0];
        g_vec_grammarTokens.erase(g_iter_grammarTokens);
        cout << "Current Token: " << g_currentToken.first << " TYPE: " << g_currentToken.second << endl;
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
            else if(p == NUM || p == CHAR || p == STR){
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
                    g_signal.b_isNeg = false;
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
        error("Get: stack is empty", g);
    }
}

string requireSTK_Top(){
    if(!g_signal.stk_opArg.empty()){
        return g_signal.stk_opArg.top();
    }
    else{
        error("Require: stack is empty", g);
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
            string s_ifLable = genLableName();
            genMidcode("jne", "", "", s_ifLable);
            if(match_S(")", true)){
                sentence();
                string s_elseLable = genLableName();
                genMidcode("jmp", "", "", s_elseLable);
                genMidcode("lab", "", "", s_ifLable);
                assist_7();
                genMidcode("lab", "", "", s_elseLable);
                return;
            }
        }
    }
    error("if_Sentence", g);
}

void assist_15(){
    if(match_S("<", true)){
        expression();
        genMidcode("<", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else if(match_S("<=", true)){
        expression();
        genMidcode("<=", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else if(match_S(">", true)){
        expression();
        genMidcode(">", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else if(match_S(">=", true)){
        expression();
        genMidcode(">=", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else if(match_S("!=", true)){
        expression();
        genMidcode("!=", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else if(match_S("==", true)){
        expression();
        genMidcode("==", getSTK_Top(), getSTK_Top(), "");
        return;
    }
    else{
        genMidcode("!=", getSTK_Top(), "0", "");
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
            string opArg_loopLableName = genLableName();
            string opArg_outLableName = genLableName();
            genMidcode("lab", "", "", opArg_loopLableName);
            condition();
            if(match_S(")", true)){
                genMidcode("jne", "", "", opArg_outLableName);
                sentence();
                genMidcode("jmp", "", "", opArg_loopLableName);
                genMidcode("lab", "", "", opArg_outLableName);
                return;
            }
            error("Missing right paren nearing while", g);
        }
        error("Missing loop-condition", g);
    }
    error("uncanny mistake in for_Sentence", g);
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
        error("condition_kid branch 1", g);
    }
    else{
        error("condition_kid none", g);
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
    error("switch_Sentence", g);
}

void assist_12(){
    if(match_S(",", true)){
        expression();
        genMidcode("cpara", "", "", getSTK_Top());
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
        genMidcode("cpara", "", "", getSTK_Top());
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
        error("Missing right paren nearing the calling of function", g);
    }
    error("uncanny mistake in func_Call", g);
}

void assist_14(){
    if(match_S("[", true)){
        expression();
        if(match_S("]", true)){
            string opArg_arrayIndex = getSTK_Top();
            string opArg_arrayName = getSTK_Top();
            genMidcode("aAss", opArg_arrayName, opArg_arrayIndex, genVarName());
            return;
        }
        error("assist_14", g);
    }
    if(match_S("(", false)){
        func_Call();
        string opArg_funcName = getSTK_Top();
        genMidcode("call", opArg_funcName, "", genVarName());
        return;
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
        error("factor branch 1", g);
    }
    else{
        error("factor none", g);
    }
}

void assist_11(){
    if(match_S("*", true)){
        factor();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("*", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_11();
        return;
    }
    else if(match_S("/", true)){
        factor();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("/", opArg_tempAlpha, opArg_tempBeta, genVarName());
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
        genMidcode("+", opArg_tempAlpha, opArg_tempBeta, genVarName());
        assist_10();
        return;
    }
    else if(match_S("-", true)){
        item();
        string opArg_tempBeta = getSTK_Top();
        string opArg_tempAlpha = getSTK_Top();
        genMidcode("-", opArg_tempAlpha, opArg_tempBeta, genVarName());
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
        genMidcode("-", "0", opArg_temp, genVarName());
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
        string opArg_varBeta = getSTK_Top();
        string opArg_varAlpha = getSTK_Top();
        genMidcode("=", opArg_varBeta, "", opArg_varAlpha);
        return;
    }
    else if(match_S("[", true)){
        expression();
        if(match_S("]", true)){
            if(match_S("=", true)){
                expression();
                string opArg_VarGama = getSTK_Top();
                string opArg_VarBeta = getSTK_Top();
                string opArg_VarAlpha = getSTK_Top();
                genMidcode("[]=", opArg_VarGama, opArg_VarBeta, opArg_VarAlpha);
                return;
            }
        }
        error("assign_Sentence branch 1", g);
    }
    else{
        error("assign_Sentence none", g);
    }
}

void assist_13(){
    if(match_S("(", false)){
        func_Call();
        genMidcode("call", getSTK_Top(), "", "");
        return;
    }
    else if(match_S("=", false) || match_S("[", false)){
        assign_Sentence();
        return;
    }
    else{
        error("assist_13", g);
    }
}

void assist_4(){
    genMidcode("scf", "", "int", getSTK_Top());
    if(match_S(",", true)){
        if(match_T(TAG, true)){
            assist_4();
            return;
        }
        error("assist_4", g);
    }
    else{
        return;
    }
}

string assist_9(){
    if(match_S(",", true)){
        expression();
        return getSTK_Top();
    }
    else{
        return "";
    }
}

void assist_5(){
    if(match_T(STR, true)){
        string opArg_printfAplha = getSTK_Top();
        string opArg_printfBeta = assist_9();
        if(opArg_printfBeta != "")
        {
            int tempType = g_symbolTab.searchSymbol(opArg_printfBeta, 1, g);
            cout << "variable " << opArg_printfBeta << " type: " << tempType;
            if(tempType == 0)
            {
                genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "int");
            }
            else if(tempType == 1)
            {
                genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "char");
            }
            else{
                if(g.g_const != 1){
                    error("Not find declaration of avarible array", g);
                }
                else{
                    genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "int");
                }
            }
        }
        else
        {
            genMidcode("prtf", opArg_printfAplha, "", "char");
        }
        
        return;
    }
    else{
        expression();
        string opArg_printfBeta = getSTK_Top();
        int tempType = g_symbolTab.searchSymbol(opArg_printfBeta, 1, g);
        cout << "variable " << opArg_printfBeta << " type: " << tempType;
        if(tempType == 0)
        {
            genMidcode("prtf", "", opArg_printfBeta, "int");
        }
        else if(tempType == 1)
        {
            genMidcode("prtf", "", opArg_printfBeta, "char");
        }
        else{
            if(g.g_const != 1){
                error("Not find declaration of avarible array", g);
            }
            else{
                genMidcode("prtf", "", opArg_printfBeta, "int");
            }
        }
        return;
    }
}

void assist_6(){
    if(match_S("(", true)){
        expression();
        if(match_S(")", true)){
            if(g_signal.b_isMain){
                genMidcode("exit", "", "", "");
            }
            else{
                genMidcode("ret", "", "", getSTK_Top());
            }
            return;
        }
        error("assist_6", g);
    }
    else{
        if(g_signal.b_isMain){
            genMidcode("exit", "", "", "");
        }
        else{
            genMidcode("ret", "", "", "");
        }
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
        error("sentence branch 1", g);
    }
    else if(match_T(TAG, true)){
        assist_13();
        if(match_S(";", true)){
            return;
        }
        error("sentence branch 2", g);
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
        error("sentence branch 3", g);
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
        error("sentence branch 4", g);
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
        error("sentence branch 5", g);
    }
    else{
        error("sentence none", g);
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
    if(match_T(KW_INT, false)){
        head_State();
        g.g_paranum++;
        string opArg_name = getSTK_Top();
        g.g_address++;
        g_symbolTab.insert_symbol(opArg_name, 3, 0, g.g_address, 0, g);
        genMidcode("para", "int", "", opArg_name);
        assist_18();
        return;
    }
    else if(match_T(KW_CHAR, false)){
        head_State();
        g.g_paranum++;
        string opArg_name = getSTK_Top();
        g.g_address++;
        g_symbolTab.insert_symbol(opArg_name, 3, 1, g.g_address, 0, g);
        genMidcode("para", "char", "", opArg_name);
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
                string opArg_funcName = getSTK_Top();
                g.g_address = 0;
                g.g_paranum = 0;
                genMidcode("func", "void", "", opArg_funcName);
                g_symbolTab.insert_symbol(opArg_funcName, 2, 0, g.g_address, g.g_paranum, g);
                arg_List();
                if(match_S(")", true)){
                    if(match_S("{", true)){
                        compound_Sentence();
                        if(match_S("}", true)){
                            genMidcode("ret", "", "", "");
                            genMidcode("end", "", "", opArg_funcName);
                            return;
                        }
                    }
                }
            }
        }
        error("void_func_Declare", g);
    }
}

void return_func_Declare(){
    if(match_T(KW_INT, false)){
        head_State();
        if(match_S("(", true)){
            string opArg_funcName = getSTK_Top();
            g.g_address = 0;
            g.g_paranum = 0;
            genMidcode("func", "int", "", opArg_funcName);
            g_symbolTab.insert_symbol(opArg_funcName, 2, 1, g.g_address, g.g_paranum, g);
            arg_List();
            if(match_S(")", true)){
                if(match_S("{", true)){
                    compound_Sentence();
                    if(match_S("}", true)){
                        genMidcode("end", "", "", opArg_funcName);
                        return;
                    }
                }

            }
            error("Missing right paren nearing the declaration of function", g);
        }
        error("uncanny mistake in declaration of int function", g);
    }
    else if(match_T(KW_CHAR, false)){
        head_State();
        if(match_S("(", true)){
            string opArg_funcName = getSTK_Top();
            g.g_address = 0;
            g.g_paranum = 0;
            genMidcode("func", "char", "", opArg_funcName);
            arg_List();
            g_symbolTab.insert_symbol(opArg_funcName, 2, 2, g.g_address, g.g_paranum, g);
            if(match_S(")", true)){
                if(match_S("{", true)){
                    compound_Sentence();
                    if(match_S("}", true)){
                        genMidcode("end", "", "", opArg_funcName);
                        return;
                    }
                }

            }
            error("Missing right paren nearing the declaration of function", g);
        }
        error("uncanny mistake in declaration of char function", g);
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
        error("Missing name of variable", g);
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
                    string opArg_arrayName = getSTK_Top();
                    //insert into symboltab
                    int tempArray_size = atoi(opArg_arrayValue.c_str());
                    g.g_address += tempArray_size;
                    g.g_paranum = tempArray_size;
                    g_symbolTab.insert_symbol(opArg_arrayName, 1, 0, g.g_address, g.g_paranum, g);
                    genMidcode("inta", "", opArg_arrayValue, opArg_arrayName);
                }
                else if(g_signal.ty_kwType == KW_CHAR){
                    string opArg_arrayValue = getSTK_Top();
                    string opArg_arrayName = getSTK_Top();
                    //insert into symboltab
                    int tempArray_size = atoi(opArg_arrayValue.c_str());
                    g.g_address += tempArray_size;
                    g.g_paranum = tempArray_size;
                    g_symbolTab.insert_symbol(opArg_arrayName, 1, 1, g.g_address, g.g_paranum, g);
                    genMidcode("chara", "", opArg_arrayValue, opArg_arrayName);
                }
                else{
                    error("the type of declaration is not supported", g);
                }
                return;
            }
        }
        error("assist_2", g);
    }
    else{
        //this is a declaration of int or char
        if(g_signal.ty_kwType == KW_INT){
            string opArg_name = getSTK_Top();
            g.g_address++;
            g.g_paranum = 0;
            g_symbolTab.insert_symbol(opArg_name, 1, 0, g.g_address, g.g_paranum, g);
            genMidcode("int", "", "", opArg_name);
        }
        else if(g_signal.ty_kwType == KW_CHAR){
            string opArg_name = getSTK_Top();
            g.g_address++;
            g.g_paranum = 0;
            g_symbolTab.insert_symbol(opArg_name, 1, 1, g.g_address, g.g_paranum, g);
            genMidcode("char", "", "", opArg_name);
        }
        else{
            error("the type of declaration is not supported", g);
        }
        return;
    }
}

void type_Iden(){
    if(match_T(KW_INT, true) || match_T(KW_CHAR, true)){
        return;
    }
    else{
        error("type_iden", g);
    }
}

void head_State(){
    if(match_T(KW_INT, false) || match_T(KW_CHAR, false)){
        type_Iden();
        if(match_T(TAG, true)){
            return;
        }
        error("head_State", g);
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
            error("varible_Declare", g);
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
    error("match number", g);
}

void assist_16(){
    if(match_T(TAG, true)){
        if(match_S("=", true)){
            if(match_S("+", false) || match_S("-", false) || match_T(NUM, false)){
                number();
                if(g_signal.b_isConst){
                    if(g_signal.ty_kwType == KW_INT){
                        string opArg_value = getSTK_Top();
                        string opArg_name = getSTK_Top();
                        //insert symbol
                        g.g_address++;
                        g.g_paranum = 0;
                        g_symbolTab.insert_symbol(opArg_name, 0, atoi(opArg_value.c_str()), g.g_address, g.g_paranum, g);
                        //generate quaternary
                        genMidcode("const","int", opArg_value, opArg_name);
                    }
                    else{
                        error("Not support assign a int value to char type", g);
                    }
                }
                assist_17();
                return;
            }
            else if(match_T(CHAR, true)){
                if(g_signal.b_isConst){
                    if(g_signal.ty_kwType == KW_CHAR){
                        string opArg_value = getSTK_Top();
                        string opArg_name = getSTK_Top();
                        //insert symbol
                        g.g_address++;
                        g.g_paranum = 0;
                        g_symbolTab.insert_symbol(opArg_name, 0, atoi(opArg_value.c_str()), g.g_address, g.g_paranum, g);
                        genMidcode("const","char", opArg_value, opArg_name);
                    }
                    else{
                        error("Not support assign a char value to int type", g);
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
        error("const_Define none", g);
    }
}

void const_Declare(){
    if(match_T(KW_CONST, true)){
        const_Define();
        if(match_S(";", true)){
            const_Declare();
        }
        else{
            error("const_Declare", g);
        }
    }
    else{
        return;
    }
}

void main_Func(){
    if(match_T(KW_VOID, true)){
        if(match_S("main", true)){
            g.g_address = 0;
            g.g_paranum = 0;
            g_symbolTab.insert_symbol("main", 2, -1, g.g_address, g.g_paranum, g);
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
    error("main_Func", g);
}

void program(){
    g.init();
    look();
    const_Declare();
    varible_Declare();
    assist_1();
    g_signal.b_isMain = true;
    main_Func();
    return;
}

void g_test(){
    mid_out(); 
    int size = g_signal.stk_opArg.size();
    cout << endl;
    if(size == 0){
        cout << ">> Analysis Successed" << endl;
    }
    else{
        cout << ">> Analysis Failed..." << endl;
    }
    for(int i = 0; i < size; i++){
        string temp = getSTK_Top();
        cout << temp << endl;
    }
    cout << endl;
    g_symbolTab.symbol_test();
}

int grammar_analyze(){
    program();
    return g.g_errorNum;
}

// symbolTable getSymbolTab() {
//     return g_symbolTab;
// }

void grammar_initialize(vector<pair<string, TYPE> > arg){
    g_vec_grammarTokens = arg;
    g_iter_grammarTokens = g_vec_grammarTokens.begin();
    g_signal.b_isConst = false;
    g_signal.b_isNeg = false;
    g_signal.b_isMain = false;
    while(!g_signal.stk_opArg.empty()){
        g_signal.stk_opArg.pop();
    }
}