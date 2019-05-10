#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<midcode> g_mipsMidecodes;
vector<midcode>::iterator g_mipsMidecodes_iter;
midcode g_curMidcode;

class _constType{
    public:
    string name;
    string value;
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
            // generate mips order
            cout << g_curMidcode.s_result << ":\t.word\t" << g_curMidcode.s_betaVar << endl;
            // add to global const defination vector
            _constType tempArg;
            tempArg.name = g_curMidcode.s_result;
            tempArg.value = atoi(g_curMidcode.s_betaVar.c_str());
            g_constDef.push_back(tempArg);
            // get next midcode
            _getMidcode();
        }
        else{
            break;
        }
    }
}


void mips_start(){
    cout << ".data" << endl;
    globalConst();
    
}


void mips_init(vector<midcode> arg){
    g_mipsMidecodes = arg;
    g_mipsMidecodes_iter = g_mipsMidecodes.begin();
    freopen("./mips.txt", "w", stdout);
}