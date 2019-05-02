#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include "config.h"
#include "Word_Analysis.cpp"
#include "c0_grammar.cpp"

using namespace std;

string file = "";

int main(int argc,char *argv[]){
    
    if(argc > 1){
        file = argv[1];
    }
    else{
        file = "C:\\Users\\HCY\\Desktop\\Compiler\\C0\\test.c";
    }
    cout << "Lex analysis result£º" << endl;
    vector<pair<string, TYPE> > vec_tempFuncarg_1;
    cout << file << endl;
    word_initialize(file);
    vec_tempFuncarg_1 = word_analyze();
    cout << endl << "Grammar analysis result£º" << endl;
    // vector<struct inter_code> vec_tempFuncarg_2;
    int g_tempError = 0;
    grammar_initialize(vec_tempFuncarg_1);
    g_tempError = grammar_analyze();
    cout << "Error number:" << g_tempError << endl;
    if(g_tempError == 0){
        cout << endl;
        g_test();
    }
    system("pause");
    return 0;
}
