#include "optimizer.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include "config.h"
#include "Word_Analysis.cpp"
#include "c0_grammar.cpp"
#include "mips.cpp"

using namespace std;

string file = "";

midcode transform_one(optMidcode now) {
    midcode p;
    p.s_operation = now.s_operation;
    p.s_alphaVar = now.s_alphaVar;
    p.s_betaVar = now.s_betaVar;
    p.s_result = now.s_result;
    return p;
}

int main(int argc,char *argv[]){
    
    if(argc > 1){
        file = argv[1];
    }
    else{
        file = ".\\Final-Version\\test.c";
    }
    // lex analysis
    cout << "Lex analysis result: " << endl;
    vector<pair<string, TYPE> > vec_tempFuncarg_1;
    cout << file << endl;
    word_initialize(file);
    vec_tempFuncarg_1 = word_analyze();

    // grammar analysis + quaternary production
    cout << endl << "Grammar analysis result: " << endl;
    int g_tempError = 0;
    grammar_initialize(vec_tempFuncarg_1);
    g_tempError = grammar_analyze();
    cout << "Error number:" << g_tempError << endl;
    if(g_tempError == 0){
        cout << endl;
        g_test();
    }

    // optimize
    opt optimizer;
    vector<midcode> vec_tempFuncarg_2 = getVecMidcodes();
    optimizer._DFG_Analysis(vec_tempFuncarg_2);
    vector<optMidcode> vec_tempFuncarg_3 = optimizer.getFinalOptMidcodes();

    // transfer format
    vector<midcode> vec_tempFuncarg_4;
    bool start = false;
    int gap_index = 0;
    bool fill = false;
    for(int i = 0; i < vec_tempFuncarg_2.size(); i++)
    {
        midcode now = vec_tempFuncarg_2[i];
        if(false == start)
        {
            if(now.s_operation != "func")
            {
                vec_tempFuncarg_4.push_back(now);
                continue;
            }
            else
            {
                start = true;
                i--;
                continue;
            }
        }
        else
        {
            if(now.s_operation == "func" || now.s_operation == "para")
            {
                vec_tempFuncarg_4.push_back(now);
                fill = false;
                continue;
            }
            else if(now.s_operation != "func" && now.s_operation != "end")
            {
                if(false == fill)
                {
                    while(true)
                    {
                        optMidcode optnow = vec_tempFuncarg_3[gap_index];
                        if(optnow.s_operation != "GAP")
                        {
                            vec_tempFuncarg_4.push_back(transform_one(optnow));
                            gap_index++;
                            continue;
                        }
                        else
                        {
                            gap_index++;
                            break;
                        }
                    }
                    fill = true;
                }
                continue;
            }
            else if(now.s_operation == "end")
            {
                vec_tempFuncarg_4.push_back(now);
                continue;
            }
        }
        
    }
    // output the result of optmize operation
    cout << "----------------------------" << endl;
    for(int i = 0; i < vec_tempFuncarg_4.size(); i++)
    {
        midcode now = vec_tempFuncarg_4[i];
        std::cout << "< " << now.s_operation << ", " << now.s_alphaVar << ", " << now.s_betaVar << ", " << now.s_result << ">" << endl;
    }
    cout << "----------------------------" << endl;

    // translate
    translate(vec_tempFuncarg_4);

    // end
    system("pause");
    return 0;
}
