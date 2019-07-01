#ifndef GENERATEINTERCODE_H
#define GENERATEINTERCODE_H

#include <string>
#include "config.h"

using namespace std;

vector<midcode> getVecMidcodes();

class genMidcode
{
    public:
    static midcode optMidcode(string operation, string alphaVar, string betaVar, string result);
};


#endif