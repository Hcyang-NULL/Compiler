#include <iostream>
#include <vector>
#include <string>
#include "config.h"

using namespace std;



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

vector<midcode> getVecMidcodes(){
    return g_vec_midcodes;
}