#include <iostream>
#include <vector>
#include <string>
#include "config.h"

using namespace std;

vector<midcode> g_mipsMidecodes;
vector<midcode>::iterator g_mipsMidecodes_iter;
midcode g_curMidcode;
midcode g_midcodeEnd;

class _constType{
    public:
    string name;
    int value;
};

class _localVari{
    public:
    string name;
    int addr;
};

vector<_constType> g_constDef;
vector<_localVari> g_localVari;
int g_stringCount = 0;
int g_funcParaNum = 0;
int g_callParaNum = 0;
int g_offSet = 0;
bool g_init = true;

bool _isNumber(string s){
    if(s[0] == '-'){
        return s[1]>='0'&s[1]<='9'?true:false;
    }
    else{
        return s[0]>='0'&s[0]<='9'?true:false;
    }
}


bool _isHcyVariable(string name){
    return name[0]=='$'?true:false;
}


int _locateAddr(string name){
    for(int i = 0; i < g_localVari.size(); i++){
        if(g_localVari[i].name == name){
            return g_localVari[i].addr;
        }
    }
    return -1;
}


void _storeLocal(string name){
    _localVari tempArg;
    tempArg.name = name;
    tempArg.addr = g_offSet;
    g_localVari.push_back(tempArg);
    cout << "\t\tsubi\t$sp\t$sp\t4" << endl;
    g_offSet += 4;
}


void _getMidcode(){
    if(g_init){
        g_mipsMidecodes.push_back(g_curMidcode);
        g_curMidcode = g_mipsMidecodes[0];
        g_mipsMidecodes.erase(g_mipsMidecodes_iter);
    }
    else{
        if(!g_mipsMidecodes.empty()){
            g_curMidcode = g_mipsMidecodes[0];
            g_mipsMidecodes.erase(g_mipsMidecodes_iter);
        }
        else{
            cout << "no midcode any more!" << endl;
        }
    }
}


void globalConst(){
    _getMidcode();
    while(true){
        if(g_curMidcode.s_operation == "const"){
            // add to global const defination vector
            _constType tempArg;
            tempArg.name = g_curMidcode.s_result;
            tempArg.value = atoi(g_curMidcode.s_betaVar.c_str());
            g_constDef.push_back(tempArg);
            // generate mips order
            cout << g_curMidcode.s_result << ":\t.word\t" << g_curMidcode.s_betaVar << endl;
            // get next midcode
            _getMidcode();
        }
        else{
            break;
        }
    }
}


void globalVar(){
    while(true){
        if(g_curMidcode.s_operation == "int" || g_curMidcode.s_operation == "char"){
            // add to global variable defination
            _constType tempArg;
            tempArg.name = g_curMidcode.s_result;
            tempArg.value = atoi(g_curMidcode.s_betaVar.c_str());
            g_constDef.push_back(tempArg);
            //generate mips order
            cout << g_curMidcode.s_result << ":\t.space\t" << "4" << endl;
            _getMidcode();
        }
        else if(g_curMidcode.s_operation == "inta" || g_curMidcode.s_operation == "chara"){
            // add to global variable defination
            _constType tempArg;
            tempArg.name = g_curMidcode.s_result;
            tempArg.value = 0;
            g_constDef.push_back(tempArg);
            //generate mips order
            int size = atoi(g_curMidcode.s_betaVar.c_str());
            cout << g_curMidcode.s_result << ":\t.space\t" << size*4 << endl;
            _getMidcode();
        }
        else{
            break;
        }
    }
}


void globalString(){
    while(g_curMidcode.s_operation != "endgame"){
        if(g_curMidcode.s_operation == "prtf"){
            if(g_curMidcode.s_alphaVar != ""){
                // generate string name as variable
                string temp_stringName = "$string";
                stringstream ss;
                ss << g_stringCount++;
                temp_stringName += ss.str();
                // add to queue
                _constType tempArg;
                tempArg.name = temp_stringName;
                tempArg.value = atoi(g_curMidcode.s_alphaVar.c_str());
                g_constDef.push_back(tempArg);
                // generate mips order
                cout << temp_stringName << ":\t.asciiz\t" << "\"" << g_curMidcode.s_alphaVar << "\"" << endl;
                g_curMidcode.s_alphaVar = temp_stringName;
            }
        }
        _getMidcode();
    }
    cout << ".text" << endl;
    cout << ".globl main" << endl;
    cout << "\t\tj\tmain" << endl;
    _getMidcode();
}


void mips_func(){
    // generate order
    cout << g_curMidcode.s_result << ":" << endl;
    cout << "\t\tsubi\t$sp\t$sp\t4" << endl;
    cout << "\t\tsubi\t$sp\t$sp\t" << g_funcParaNum*4 << endl;
    // update global offset
    g_offSet = 8+g_funcParaNum*4;
}


void mips_int(){
    // add to local variables vector;
    _localVari tempArg;
    tempArg.name = g_curMidcode.s_result;
    tempArg.addr = g_offSet;
    g_localVari.push_back(tempArg);
    // generate order;
    cout << "\t\tsubi\t$sp\t$sp\t4" << endl;
    // update global offset
    g_offSet += 4;
}


void mips_char(){
    // add to local variables vector;
    _localVari tempArg;
    tempArg.name = g_curMidcode.s_result;
    tempArg.addr = g_offSet;
    g_localVari.push_back(tempArg);
    // generate order;
    cout << "\t\tsubi\t$sp\t$sp\t4" << endl;
    // update global offset
    g_offSet += 4;
}


void mips_inta(){
    // add to local variable vector
    _localVari tempArg;
    tempArg.name = g_curMidcode.s_result;
    tempArg.addr = g_offSet;
    g_localVari.push_back(tempArg);
    // generate mips order
    int size = atoi(g_curMidcode.s_betaVar.c_str());
    cout << "\t\tsubi\t$sp\t$sp\t" << size*4 << endl;
    // update global offset
    g_offSet += size*4;
}


void mips_chara(){
    // add to local variable vector
    _localVari tempArg;
    tempArg.name = g_curMidcode.s_result;
    tempArg.addr = g_offSet;
    g_localVari.push_back(tempArg);
    // generate mips order
    int size = atoi(g_curMidcode.s_betaVar.c_str());
    cout << "\t\tsubi\t$sp\t$sp\t" << size*4 << endl;
    // update global offset
    g_offSet += size*4;
}


void mips_add(){
    // generate mips order of alpha operator
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        // not find in local variable vector
        if(tempAddr_alpha == 1){
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    // generate mips order of beta operator
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
        // not find in local variable vector
        if(tempAddr_beta == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    // generate mips order of add
    cout << "\t\tadd\t$t0\t$t0\t$t1" << endl;
    // generate mips order of store of variable of result
    if(_isHcyVariable(g_curMidcode.s_result)){
        _storeLocal(g_curMidcode.s_result);
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
    }
    else{
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        // not find
        if(tempAddr_gama == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
            cout << "\t\tsw\t$t0\t($t1)" << endl;
        }
        // find
        else{
            cout << "\t\tsw\t$t0\t" << g_curMidcode.s_result << "($fp)" << endl;
        }
    }
}


void mips_sub(){
    // generate mips order of alpha operator
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        // not find in local variable vector
        if(tempAddr_alpha == 1){
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    // generate mips order of beta operator
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
        // not find in local variable vector
        if(tempAddr_beta == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    // generate mips order of sub
    cout << "\t\tsub\t$t0\t$t0\t$t1" << endl;
    // generate mips order of store of variable of result
    if(_isHcyVariable(g_curMidcode.s_result)){
        _storeLocal(g_curMidcode.s_result);
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
    }
    else{
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        // not find
        if(tempAddr_gama == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
            cout << "\t\tsw\t$t0\t($t1)" << endl;
        }
        // find
        else{
            cout << "\t\tsw\t$t0\t" << g_curMidcode.s_result << "($fp)" << endl;
        }
    }
}


void mips_mul(){
    // generate mips order of alpha operator
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        // not find in local variable vector
        if(tempAddr_alpha == 1){
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    // generate mips order of beta operator
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
        // not find in local variable vector
        if(tempAddr_beta == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    // generate mips order of mul
    cout << "\t\tmul\t$t0\t$t0\t$t1" << endl;
    // generate mips order of store of variable of result
    if(_isHcyVariable(g_curMidcode.s_result)){
        _storeLocal(g_curMidcode.s_result);
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
    }
    else{
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        // not find
        if(tempAddr_gama == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
            cout << "\t\tsw\t$t0\t($t1)" << endl;
        }
        // find
        else{
            cout << "\t\tsw\t$t0\t" << g_curMidcode.s_result << "($fp)" << endl;
        }
    }
}


void mips_div(){
    // generate mips order of alpha operator
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        // not find in local variable vector
        if(tempAddr_alpha == 1){
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    // generate mips order of beta operator
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
        // not find in local variable vector
        if(tempAddr_beta == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t0)" << endl;
        }
        // find
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    // generate mips order of div
    cout << "\t\tdiv\t$t0\t$t0\t$t1" << endl;
    // generate mips order of store of variable of result
    if(_isHcyVariable(g_curMidcode.s_result)){
        _storeLocal(g_curMidcode.s_result);
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
    }
    else{
        int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
        // not find
        if(tempAddr_gama == 1){
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
            cout << "\t\tsw\t$t0\t($t1)" << endl;
        }
        // find
        else{
            cout << "\t\tsw\t$t0\t" << g_curMidcode.s_result << "($fp)" << endl;
        }
    }
}


void mips_printf(){
    if(g_curMidcode.s_alphaVar != ""){
        cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
        cout << "\t\tmove\t$a0\t$t0" << endl;
        cout << "\t\tli\t$v0\t4" << endl;
        cout << "\t\tsyscall" << endl;
        cout << "\t\tli\t$a0\t\'\\n\'" << endl;
        cout << "\t\tli\t$v0\t11" << endl;
        cout << "\t\tsyscall" << endl;
    }
    if(g_curMidcode.s_betaVar == ""){
        return;
    }
    int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_betaVar);
    // global variable
    if(tempAddr_alpha == 1){
        if(_isNumber(g_curMidcode.s_betaVar)){
            cout << "\t\tli\t$a0\t" << g_curMidcode.s_betaVar << endl;
        }
        else{
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$a0\t($t0)" << endl;
        }
    }
    // local variable
    else{
        cout << "\t\tlw\t$a0\t" << tempAddr_alpha << "($fp)" << endl;
    }
    // generate mips order of syscall
    if(g_curMidcode.s_result == "int"){
        cout << "\t\tli\t$v0\t1" << endl;
    }
    else if(g_curMidcode.s_result == "char"){
        cout << "\t\tli\t$v0\t11" << endl;
    }
    cout << "\t\tsyscall" << endl;
    cout << "\t\tli\t$a0\t\'\\n\'" << endl;
    cout << "\t\tli\t$v0\t11" << endl;
    cout << "\t\tsyscall" << endl;
}


void mips_scanf(){
    int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_result);
    if(g_curMidcode.s_betaVar == "int"){
        cout << "\t\tli\t$v0\t5" << endl;
    }
    else if(g_curMidcode.s_betaVar == "char"){
        cout << "\t\tli\t$v0\t12" << endl;
    }
    cout << "\t\tsyscall" << endl;
    // global variable
    if(tempAddr_alpha == 1){
        cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
        cout << "\t\tsw\t$v0\t($t0)" << endl;
    }
    else{
        cout << "\t\tsw\t$v0\t" << tempAddr_alpha << "($fp)" << endl;
    }
}


void mips_return(){
    if(g_curMidcode.s_result != ""){
        if(_isNumber(g_curMidcode.s_result)){
            cout << "\t\tli\t$v1\t" << g_curMidcode.s_result << endl;
        }
        else{
            int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_result);
            // global variable
            if(tempAddr_alpha == 1){
                cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                cout << "\t\tlw\t$v1\t($t0)" << endl;
            }
            else{
                cout << "\t\tlw\t$v1\t" << tempAddr_alpha << "($fp)" << endl;
            }
        }
    }
    cout << "\t\tmove\t$t0\t$ra" << endl;
    cout << "\t\tlw\t$ra\t-4($fp)" << endl;
    cout << "\t\tadd\t$sp\t$fp\t$zero" << endl;
    cout << "\t\tlw\t$fp\t($fp)" << endl;
    cout << "\t\tjr\t$t0" << endl;
}


void mips_label(){
    cout << g_curMidcode.s_result << ":" << endl;
}


void mips_para(){
    if(g_funcParaNum < 4){
        switch(g_funcParaNum){
            case 0 :
                cout << "\t\tmove\t$t0\t$a0" << endl;
                cout << "\t\tsw\t$t0\t" << -8 << "($fp)" << endl;
                break;
            case 1 :
                cout << "\t\tmove\t$t0\t$a1" << endl;
                cout << "\t\tsw\t$t0\t" << -12 << "($fp)" << endl;
                break;
            case 2 :
                cout << "\t\tmove\t$t0\t$a2" << endl;
                cout << "\t\tsw\t$t0\t" << -16 << "($fp)" << endl;
                break;
            case 3 :
                cout << "\t\tmove\t$t0\t$a3" << endl;
                cout << "\t\tsw\t$t0\t" << -20 << "($fp)" << endl;
                break;
        }
    }
    _storeLocal(g_curMidcode.s_result);
    g_funcParaNum++;
}


void mips_callPara(){
    int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_result);
    if(g_callParaNum < 4){
        switch(g_callParaNum){
            case 0:{
                // global variable
                if(tempAddr_alpha == 1){
                    if(_isNumber(g_curMidcode.s_result)){
                        cout << "\t\tli\t$a0\t" << g_curMidcode.s_result << endl;
                    }
                    else{
                        cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                        cout << "\t\tlw\t$a0\t($t0)" << endl;
                    }
                }
                else{
                    cout << "\t\tlw\t$a0\t" << tempAddr_alpha << "($fp)" << endl;
                }
                break;
            }
            case 1:{
                if(tempAddr_alpha == 1){
                    if(_isNumber(g_curMidcode.s_result)){
                        cout << "\t\tli\t$a1\t" << g_curMidcode.s_result << endl;
                    }
                    else{
                        cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                        cout << "\t\tlw\t$a1\t($t0)" << endl;
                    }
                }
                else{
                    cout << "\t\tlw\t$a1\t" << tempAddr_alpha << "($fp)" << endl;
                }
                break;
            }
            case 2:{
                if(tempAddr_alpha == 1){
                    if(_isNumber(g_curMidcode.s_result)){
                        cout << "\t\tli\t$a2\t" << g_curMidcode.s_result << endl;
                    }
                    else{
                        cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                        cout << "\t\tlw\t$a2\t($t0)" << endl;
                    }
                }
                else{
                    cout << "\t\tlw\t$a2\t" << tempAddr_alpha << "($fp)" << endl;
                }
                break;
            }
            case 3:{
                if(tempAddr_alpha == 1){
                    if(_isNumber(g_curMidcode.s_result)){
                        cout << "\t\tli\t$a3\t" << g_curMidcode.s_result << endl;
                    }
                    else{
                        cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                        cout << "\t\tlw\t$a3\t($t0)" << endl;
                    }
                }
                else{
                    cout << "\t\tlw\t$a3\t" << tempAddr_alpha << "($fp)" << endl;
                }
                break;
            }
        }
    }
    // the number of parameters if more than four
    else{
        if(tempAddr_alpha == 1){
            if(_isNumber(g_curMidcode.s_result)){
                cout << "\t\tli\t$t8\t" << g_curMidcode.s_result << endl;
            }
            else{
                cout << "\t\tla\t$t0\t" << g_curMidcode.s_result << endl;
                cout << "\t\tlw\t$t8\t($t0)" << endl;
            }
        }
        else{
            cout << "\t\tlw\t$t8\t" << tempAddr_alpha << "($fp)" << endl;
        }
        cout << "\t\tsw\t$t8\t" << -1*((g_callParaNum-4)*4+24) << "($sp)" << endl;
    }
    g_callParaNum++;
}


void mips_callFunc(){
    // clear global counter
    g_callParaNum = 0;
    cout << "\t\tsw\t$fp\t($sp)" << endl;
    cout << "\t\tadd\t$fp\t$sp\t$0" << endl;
    cout << "\t\tsubi\t$sp\t$sp\t4" << endl;
    cout << "\t\tsw\t$ra\t($sp)" << endl;
    cout << "\t\tjal\t" << g_curMidcode.s_alphaVar << endl;
    cout << "\t\tnop\n";
    // generate mips order
    if(g_curMidcode.s_result != ""){
        _storeLocal(g_curMidcode.s_result);
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_result);
        cout << "\t\tsw\t$v1\t" << tempAddr_alpha << "($fp)" << endl;
    }
}


void mips_jne(){
    cout << g_curMidcode.s_result << endl;
}


void mips_jump(){
    cout << "\t\tj\t" << g_curMidcode.s_result << endl;
}


void mips_assign(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        // global variable
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_result);
    if(tempAddr_beta == 1){//not find, is global
        cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
        cout << "\t\tsw\t$t0\t($t1)" << endl;
    }
    else{
        cout << "\t\tsw\t$t0\t" << tempAddr_beta << "($fp)" << endl;
    }
}


void mips_bigger(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tble\t$t0\t$t1\t";
}


void mips_smaller(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tbge\t$t0\t$t1\t";
}


void mips_equal(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tbne\t$t0\t$t1\t";
}


void mips_notEqual(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tbeq\t$t0\t$t1\t";
}


void mips_notBigger(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tbgt\t$t0\t$t1\t";
}


void mips_notSmaller(){
    if(_isNumber(g_curMidcode.s_alphaVar)){
        cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_alpha == 1){//not find, is global
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
        else{
            cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
        }
    }
    if(_isNumber(g_curMidcode.s_betaVar)){
        cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
    }
    else{
        int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_alphaVar);
        if(tempAddr_beta == 1){//not find, is global
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        else{
            cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        }
    }
    cout << "\t\tblt\t$t0\t$t1\t";
}


void mips_assArray(){
    int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
    int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
    int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
    if(tempAddr_alpha == 1){
        if(_isNumber(g_curMidcode.s_alphaVar)){
            cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
        }
        else{
            cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
            cout << "\t\tlw\t$t0\t($t0)" << endl;
        }
    }
    else{
        cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
    }
    if(tempAddr_beta == 1){
        if(_isNumber(g_curMidcode.s_betaVar)){
            cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
        }
        else{
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }
        cout << "\t\tmul\t$t1\t$t1\t-4" << endl;

    }
    else{
        cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        cout << "\t\tmul\t$t1\t$t1\t-4" << endl;
    }
    if(tempAddr_gama == 1){
        cout << "\t\tla\t$t2\t" << g_curMidcode.s_result << endl;
        cout << "\t\tadd\t$t1\t$t1\t$t2" << endl;
        cout << "\t\tsw\t$t0\t($t1)" << endl;
    }
    else{
        cout << "\t\tli\t$t2\t" << tempAddr_gama << endl;
        cout << "\t\tadd\t$t1\t$t2\t$t1" << endl;
        cout << "\t\tadd\t$t1\t$t1\t$fp" << endl;
        cout << "\t\tsw\t$t0\t($t1)" << endl;
    }
}


void mips_arrayAss(){
    int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
    int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
    if(tempAddr_alpha == 1){
        cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
    }
    else{
        cout << "\t\tli\t$t0\t" << tempAddr_alpha << endl;
        cout << "\t\tadd\t$t0\t$t0\t$fp" << endl;
    }
    if(tempAddr_beta == 1){
        if(_isNumber(g_curMidcode.s_betaVar)){
            cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
        }
        else{
            cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
            cout << "\t\tlw\t$t1\t($t1)" << endl;
        }

        cout << "\t\tmul\t$t1\t$t1\t-4" << endl;
    }
    else{
        cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
        cout << "\t\tmul\t$t1\t$t1\t-4" << endl;
    }
    cout << "\t\tadd\t$t0\t$t0\t$t1" << endl;
    cout << "\t\tlw\t$t0\t($t0)" << endl;
    _storeLocal(g_curMidcode.s_result);
    int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
    cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
}


void mips_exit(){
    cout << "\t\tli\t$v0\t10" << endl;
    cout << "\t\tsyscall" << endl;
}


void mips_end(){
    g_localVari.clear();
}


void mips_start(){
    while(g_curMidcode.s_operation != "endgame"){
        if(g_curMidcode.s_operation == "func"){
            mips_func();
        }
        else if(g_curMidcode.s_operation == "int"){
            mips_int();
        }
        else if(g_curMidcode.s_operation == "char"){
            mips_char();
        }
        else if(g_curMidcode.s_operation == "inta"){
            mips_inta();
        }
        else if(g_curMidcode.s_operation == "chara"){
            mips_chara();
        }
        else if(g_curMidcode.s_operation == "add"){
            mips_add();
        }
        else if(g_curMidcode.s_operation == "sub"){
            mips_sub();
        }
        else if(g_curMidcode.s_operation == "*"){
            mips_mul();
        }
        else if(g_curMidcode.s_operation == "/"){
            mips_div();
        }
        else if(g_curMidcode.s_operation == "prtf"){
            mips_printf();
        }
        else if(g_curMidcode.s_operation == "scf"){
            mips_scanf();
        }
        else if(g_curMidcode.s_operation == "ret"){
            mips_return();
        }
        else if(g_curMidcode.s_operation == "lab"){
            mips_label();
        }
        else if(g_curMidcode.s_operation == "para"){
            mips_para();
        }
        else if(g_curMidcode.s_operation == "cpara"){
            mips_callPara();
        }
        else if(g_curMidcode.s_operation == "call"){
            mips_callFunc();
        }
        else if(g_curMidcode.s_operation == "jne"){
            mips_jne();
        }
        else if(g_curMidcode.s_operation == "jmp"){
            mips_jump();
        }
        else if(g_curMidcode.s_operation == "="){
            mips_assign();
        }
        else if(g_curMidcode.s_operation == ">"){
            mips_bigger();
        }
        else if(g_curMidcode.s_operation == "<"){
            mips_smaller();
        }
        else if(g_curMidcode.s_operation == "=="){
            mips_equal();
        }
        else if(g_curMidcode.s_operation == "!="){
            mips_notEqual();
        }
        else if(g_curMidcode.s_operation == "<="){
            mips_notBigger();
        }
        else if(g_curMidcode.s_operation == ">="){
            mips_notSmaller();
        }
        else if(g_curMidcode.s_operation == "[]="){
            mips_assArray();
        }
        else if(g_curMidcode.s_operation == "aAss"){
            mips_arrayAss();
        }
        else if(g_curMidcode.s_operation == "exit"){
            mips_exit();
        }
        else if(g_curMidcode.s_operation == "end"){
            mips_end();
        }

        _getMidcode();
    }
}


void mips_init(){
    cout << ".data" << endl;
    // handle the defination of consts
    g_init = false;
    globalConst();
    // handle the defination of variables
    globalVar();
    // handle the data of string
    g_init = true;
    globalString();
}


void translate_init(vector<midcode> arg){
    g_mipsMidecodes = arg;
    g_midcodeEnd.s_operation = "endgame";
}


void add_endGame(){
    g_mipsMidecodes.push_back(g_midcodeEnd);
    g_mipsMidecodes_iter = g_mipsMidecodes.begin();
}


void translate(vector<midcode> arg){
    freopen("./mips.txt", "w", stdout);
    translate_init(arg);
    add_endGame();
    g_init = true;
    mips_init();
    g_init = false;
    add_endGame();
    mips_start();
    freopen("CON", "w", stdout);
    cout << endl << ">> translate completed !" << endl;
}