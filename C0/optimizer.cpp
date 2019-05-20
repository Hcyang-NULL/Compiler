#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include "config.h"
#include "generate_intercode.h"
#include "mips.h"
#include "optimizer.h"

using namespace std;

bool g_experOpt_succ;

midcode optExper_Assist(std::string opera, std::string alpha, std::string beta, std::string result){
    g_experOpt_succ = true;
    int a = atoi(alpha.c_str());
    int b = atoi(beta.c_str());
    int c;
    if(opera == "+")
    {
        c = a+b;
    }
    else if(opera == "-")
    {
        c = a-b;
    }
    else if(opera == "*")
    {
        c = a*b;
    }
    else if(opera == "/")
    {
        c = a/b;
    }
    std::stringstream ss;
    ss << c;
    return genMidcode::optMidcode("=", ss.str(), "", result);
}

// 计算表达式优化
std::vector<midcode> opt::_exprOptimizer(std::vector<midcode> orig_midcodes)
{
    std::vector<midcode>::iterator orig_iter = orig_midcodes.begin();
    std::vector<midcode> opt_result;
    midcode current_midcode;
    midcode new_midcode;

    while(!orig_midcodes.empty())
    {
        current_midcode = orig_midcodes[0];
        orig_midcodes.erase(orig_iter);
        g_experOpt_succ = false;

        if(current_midcode.s_operation == "+")
        {
            if(_isNumber(current_midcode.s_alphaVar) && _isNumber(current_midcode.s_betaVar))
            {
                opt_result.push_back(optExper_Assist("+", current_midcode.s_alphaVar, current_midcode.s_betaVar, current_midcode.s_result));
            }
        }
        else if(current_midcode.s_operation == "-")
        {
            if(_isNumber(current_midcode.s_alphaVar) && _isNumber(current_midcode.s_betaVar))
            {
                opt_result.push_back(optExper_Assist("-", current_midcode.s_alphaVar, current_midcode.s_betaVar, current_midcode.s_result));
            }
        }
        else if(current_midcode.s_operation == "*")
        {
            if(_isNumber(current_midcode.s_alphaVar) && _isNumber(current_midcode.s_betaVar))
            {
                opt_result.push_back(optExper_Assist("*", current_midcode.s_alphaVar, current_midcode.s_betaVar, current_midcode.s_result));
            }
        }
        else if(current_midcode.s_operation == "/")
        {
            if(_isNumber(current_midcode.s_alphaVar) && _isNumber(current_midcode.s_betaVar))
            {
                opt_result.push_back(optExper_Assist("/", current_midcode.s_alphaVar, current_midcode.s_betaVar, current_midcode.s_result));
            }
        }
        if(!g_experOpt_succ)
        {
            opt_result.push_back(current_midcode);
        }
    }

    cout << "------ Simple expression optimizer ------" << endl;;
    for(int i = 0; i < opt_result.size(); i++){
        midcode now = opt_result[i];
        cout << "<" << now.s_operation << ", " << now.s_alphaVar << ", "
        << now.s_betaVar << ", " << now.s_result << ">" << endl;
    }
    cout << "-------------- Completed ----------------" << endl;

    return opt_result;
}