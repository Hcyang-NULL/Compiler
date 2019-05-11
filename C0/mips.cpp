#include <iostream>
#include <vector>
#include <string>
#include "config.h"

using namespace std;

vector<midcode> g_mipsMidecodes;
vector<midcode>::iterator g_mipsMidecodes_iter;
midcode g_curMidcode;

class _constType{
    public:
    string name;
    int value;
};

vector<_constType> g_constDef;


void _getMidcode(){
    if(!g_mipsMidecodes.empty()){
        g_curMidcode = g_mipsMidecodes[0];
        g_mipsMidecodes.erase(g_mipsMidecodes_iter);
    }
    else{
        cout << "no midcode any more!" << endl;
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


void mips_init(){
    cout << ".data" << endl;
    globalConst();
    globalVar();
}


void mips_start(){
    mips_init();

    freopen("CON", "w", stdout);
}


void mips_init(vector<midcode> arg){
    g_mipsMidecodes = arg;
    g_mipsMidecodes_iter = g_mipsMidecodes.begin();
    freopen("./mips.txt", "w", stdout);
}