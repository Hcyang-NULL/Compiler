#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include "config.h"
#include "generate_intercode.h"
#include "mips.h"
#include "optimizer.h"
#include <assert.h>

using namespace std;

bool g_experOpt_succ;

midcode optExper_Assist(std::string opera, std::string alpha, std::string beta, std::string result) {
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
std::vector<midcode> opt::_exprOptimizer(std::vector<midcode> org_midcodes) {
    std::vector<midcode>::iterator orig_iter = org_midcodes.begin();
    std::vector<midcode> opt_result;
    midcode current_midcode;
    midcode new_midcode;

    while(!org_midcodes.empty())
    {
        current_midcode = org_midcodes[0];
        org_midcodes.erase(orig_iter);
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


int _getTargetBlock(std::vector<Block> blocks, std::string target) {
    for(int i = 0; i < blocks.size(); i++)
    {
        if(blocks[i].blockCodes[0].s_operation == "lab" || blocks[i].blockCodes[0].s_result == target)
        {
            return i;
        }
    }
    assert(0);
}


void DFG::linkBlocks() {
    for(int i = 0; i < blocks.size(); i++)
    {
        optMidcode last = blocks[i].blockCodes[blocks[i].blockCodes.size()-1];
        if(last.s_operation == "jmp" || last.s_operation == "jne")
        {
            int tarBlock_index = _getTargetBlock(blocks, last.s_result);
            blocks[tarBlock_index].preBlocks.push_back(blocks[i]);
            blocks[i].sucBlocks.push_back(blocks[tarBlock_index]);
        }
        if(last.s_operation != "jmp" && i != blocks.size()-1)
        {
            blocks[i].sucBlocks.push_back(blocks[i+1]);
            blocks[i+1].preBlocks.push_back(blocks[i]);
        }
    }
}


void DFG::splitBlocks() {
    std::vector<optMidcode> tempList;
    tempList.push_back(midcodes[0]);
    for(int i = 1; i < midcodes.size(); i++)
    {
        if(midcodes[i].first)
        {
            Block temp_block;
            temp_block.blockCodes = tempList;
            blocks.push_back(temp_block);
            tempList.clear();
        }
        tempList.push_back(midcodes[i]);
    }
    Block last_block;
    last_block.blockCodes = tempList;
    blocks.push_back(last_block);
}


int _getTargetCode(std::vector<optMidcode> midcodes, std::string target) {
    int len = midcodes.size();
    for(int i = 0; i < len; i++)
    {
        if(midcodes[i].s_operation == "lab" && midcodes[i].s_result == target)
        {
            return i;
        }
    }
    assert(0);
}


void DFG::markFirst() {
    int len = midcodes.size();
    midcodes[0].first = true;
    midcodes[len-1].first = true;
    midcodes[1].first = true;
    for(int i = 1; i < len-1; i++)
    {
        if(midcodes[i].s_operation == "jmp" || midcodes[i].s_operation == "jne")
        {
            midcodes[_getTargetCode(midcodes, midcodes[i].s_result)].first = true;
            midcodes[i+1].first = true;
        }
    }
}


std::vector<DFG> _splitDFGS(std::vector<optMidcode> midcodes) {
    std::vector<DFG> dfgs;
    for(int i = 0; i < midcodes.size(); i++)
    {
        if(midcodes[i].s_operation == "func")
        {
            DFG temp_dfg;
            std::vector<optMidcode> temp_codes;
            while(true)
            {
                temp_codes.push_back(midcodes[i]);
                ++i;
                if(midcodes[i].s_operation == "end")
                {
                    temp_codes.push_back(midcodes[i]);
                    ++i;
                    break;
                }
            }
            temp_dfg.midcodes = temp_codes;
            dfgs.push_back(temp_dfg);
        }
    }
    return dfgs;
}


// copy midcode to optmidcode
std::vector<optMidcode> copy_midcode(std::vector<midcode> org_midcodes) {
    std::vector<optMidcode> result;
    for(int i = 0; i < org_midcodes.size(); i++)
    {
        optMidcode temp;
        temp.first = false;
        temp.s_alphaVar = org_midcodes[i].s_alphaVar;
        temp.s_betaVar = org_midcodes[i].s_betaVar;
        temp.s_operation = org_midcodes[i].s_operation;
        temp.s_result = org_midcodes[i].s_result;
        result.push_back(temp);
    }
    return result;
}


void opt::_DFG_Analysis(std::vector<midcode> org_midcodes) {
    std::vector<optMidcode> midcodes = copy_midcode(org_midcodes);
    std::vector<DFG> dfgs = _splitDFGS(midcodes);
    for(int i = 0; i < dfgs.size(); i++)
    {
        DFG now_dfg = dfgs[i];
        now_dfg.markFirst();
        now_dfg.splitBlocks();
        now_dfg.linkBlocks();
    }
}