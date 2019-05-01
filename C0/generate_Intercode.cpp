#include <iostream>
#include <vector>
#include <string>

using namespace std;

class midcode{
    public:
        string s_operation;
        string s_alphaVar;
        string s_betaVar;
        string s_result;
};

vector<midcode> g_vec_midcodes;

void genMidcode(string operation, string alphaVar, string betaVar, string result){
    midcode mc_temp;
    mc_temp.s_operation = operation;
    mc_temp.s_alphaVar = alphaVar;
    mc_temp.s_betaVar = betaVar;
    mc_temp.s_result = result;
    g_vec_midcodes.push_back(mc_temp);
}

void mid_out(){
    for(int i = 0; i < g_vec_midcodes.size(); i++){
        midcode now = g_vec_midcodes[i];
        cout << "<" << now.s_operation << ", " << now.s_alphaVar << ", "
        << now.s_betaVar << ", " << now.s_result << ">" << endl;
    }
}