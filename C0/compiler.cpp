#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include "config.h"
#include "Word_Analysis.cpp"
#include "sample_grammar.cpp"
#include "sample_translate.cpp"

using namespace std;

string file = "";

int main(int argc,char *argv[]){
    
    if(argc > 1){
        file = argv[1];
    }
    else{
        file = "test.c";
    }
    cout << "词法分析结果：" << endl;
    vector<pair<string, TYPE> > vec_tempFuncarg_1;
    word_initialize(file);
    vec_tempFuncarg_1 = word_analyze();
    cout << endl << "语法分析结果：" << endl;
    vector<struct inter_code> vec_tempFuncarg_2;
    grammar_initialize(vec_tempFuncarg_1);
    vec_tempFuncarg_2 = grammar_analyze();
    // system("pause");
    cout << endl << "语法制导翻译结果：" << endl;
    translate_initialize(vec_tempFuncarg_2);
    translate_process();
    // system("pause");
    return 0;
}
