#include <iostream>
#include <stdlib.h>
#include "config.h"
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>

using namespace std;

string global_name = "hcy_t";
int increase = 0;
vector<struct inter_code> g_intercodes;

string genADD(string opt1, string opt2){
    stringstream ss;
    struct inter_code now;
    now.opt = OPT_ADD;
    now.opt1 = opt1;
    now.opt2 = opt2;
    ss << increase;
    now.result = global_name+ss.str();
    increase++;
    g_intercodes.push_back(now);
    return now.result;
}

string genMUL(string opt1, string opt2){
    stringstream ss;
    struct inter_code now;
    now.opt = OPT_MUL;
    now.opt1 = opt1;
    now.opt2 = opt2;
    ss << increase;
    now.result = global_name+ss.str();
    increase++;
    g_intercodes.push_back(now);
    return now.result;
}

vector<struct inter_code> finish(){
    cout << "四元式个数:" << g_intercodes.size() << endl;
    for(int i = 0; i < g_intercodes.size(); i++){
        cout << "< ";
        if(g_intercodes[i].opt == OPT_ADD){
            cout << "+, ";
        }
        else if(g_intercodes[i].opt == OPT_MUL){
            cout << "*, ";
        }
        cout << g_intercodes[i].result << ", ";
        cout << g_intercodes[i].opt1 << ", ";
        cout << g_intercodes[i].opt2 << ">" << endl;;
    }
    return g_intercodes;
}