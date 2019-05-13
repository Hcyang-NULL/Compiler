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

vector<_constType> g_constDef;
int g_stringCount = 0;
bool g_init = true;

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
    cout << ".globl main" << endl ;
    cout << "\t\tj\tmain" << endl ;
    _getMidcode();
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


void mips_start(){
}


void translate_init(vector<midcode> arg){
    g_mipsMidecodes = arg;
    g_midcodeEnd.s_operation = "endgame";
    g_mipsMidecodes.push_back(g_midcodeEnd);
    g_mipsMidecodes_iter = g_mipsMidecodes.begin();
}


void translate(vector<midcode> arg){
    freopen("./mips.txt", "w", stdout);
    translate_init(arg);
    g_init = true;
    mips_init();
    g_init = false;
    mips_start();
    freopen("CON", "w", stdout);
}