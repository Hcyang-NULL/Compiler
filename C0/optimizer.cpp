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


double _TranNumber(string s) {
    return atof(s.c_str());
}


void midcode_print2(vector<optMidcode> a){
    for(int i = 0; i < a.size(); i++){
        optMidcode now = a[i];
        cout << "<" << now.s_operation << ", " << now.s_alphaVar << ", "
        << now.s_betaVar << ", " << now.s_result << ">   f:" << now.first << endl;
    }
    cout << endl;
}


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
        if(blocks[i].blockCodes[0].s_operation == "lab" && blocks[i].blockCodes[0].s_result == target)
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
            blocks[tarBlock_index].preBlocks.push_back(i);
            blocks[i].sucBlocks.push_back(tarBlock_index);
        }
        if(last.s_operation != "jmp" && i != blocks.size()-1)
        {
            blocks[i].sucBlocks.push_back(i+1);
            blocks[i+1].preBlocks.push_back(i);
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
            midcode_print2(tempList);
            tempList.clear();
        }
        tempList.push_back(midcodes[i]);
    }
    Block last_block;
    last_block.blockCodes = tempList;
    midcode_print2(tempList);
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
    int len = midcodes.size();
    for(int i = 0; i < len; i++)
    {
        if(midcodes.at(i).s_operation == "func")
        {
            DFG temp_dfg;

            temp_dfg.funcName = midcodes[i].s_result;
            temp_dfg.midcodes.push_back(midcodes[i]);
            ++i;

            while(true)
            {
                if(midcodes[i].s_operation == "para")
                {
                    temp_dfg.paras.push_back(midcodes[i++]);
                }
                else
                {
                    break;
                }
            }

            while(true)
            {
                if(midcodes[i].s_operation == "end")
                {
                    temp_dfg.midcodes.push_back(midcodes[i++]);
                    break;
                }
                temp_dfg.midcodes.push_back(midcodes[i]);
                ++i;
            }
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


vector<midcode> _getGlbMicos(vector<midcode> midcodes) {
    vector<midcode> result;
    for(int i = 0; i < midcodes.size(); i++)
    {
        midcode now = midcodes[i];
        if(now.s_operation == "func")
        {
            return result;
        }
        result.push_back(now);
    }
    assert(0);
}


void inVal_test(vector<var> vars, Block b, int bi) {
    cout << "Block" << bi << ": inVals" << endl;
    for(int i = 0; i < vars.size(); i++)
    {
        cout << vars[i].index << ": " << vars[i].name;
        if(b.inVals[i] == NAC)
        {
            cout << "  NAC";
        }
        else if(b.inVals[i] == UNDEF)
        {
            cout << "  UNDEF";
        }
        else
        {
            cout << "  " << b.inVals[i];
        }
        cout << endl;
    }
    cout << endl;
}


void outVal_test(vector<var> vars, Block b, int bi) {
    cout << "Block" << bi << ": outVals" << endl;
    for(int i = 0; i < vars.size(); i++)
    {
        cout << vars[i].index << ": " << vars[i].name;
        if(b.outVals[i] == NAC)
        {
            cout << "  NAC";
        }
        else if(b.outVals[i] == UNDEF)
        {
            cout << "  UNDEF";
        }
        else
        {
            cout << "  " << b.outVals[i];
        }
        cout << endl;
    }
    cout << endl;
}


void ConstProp::analyze() {
    vector<double> inInit;
    for(int i = 0; i < vars.size(); i++)
    {
        inInit.push_back(UNDEF);
    }
    dfg.blocks[0].outVals = boundVals;
    for(int i = 1; i < dfg.blocks.size(); i++)
    {
        dfg.blocks[i].outVals = initVals;
        dfg.blocks[i].inVals = inInit;
    }
    
    bool outChange = true;
    while(outChange)
    {
        outChange = false;
        for(int i = 1; i < dfg.blocks.size(); i++)
        {
            cout << dfg.blocks[dfg.blocks[1].preBlocks[0]].outVals[0] << endl;
            join(dfg.blocks[i]);
            inVal_test(vars, dfg.blocks[i], i);
            if(translate(dfg.blocks[i]))
            {
                outChange = true;
            }
            outVal_test(vars, dfg.blocks[i], i);
        }
    }
}





void midcode_print1(vector<midcode> a){
    for(int i = 0; i < a.size(); i++){
        midcode now = a[i];
        cout << "<" << now.s_operation << ", " << now.s_alphaVar << ", "
        << now.s_betaVar << ", " << now.s_result << ">" << endl;
    }
    cout << endl;
}


void outOptmidcode(optMidcode now) {
    cout << "<" << now.s_operation << ", " << now.s_alphaVar << ", " << now.s_betaVar << ", " << now.s_result << ">   f:" << now.first << endl;
}

void link_test(DFG dfg) {
    for(int i = 0; i < dfg.blocks.size(); i++)
    {
        Block b = dfg.blocks[i];
        cout << "now block > ";
        outOptmidcode(b.blockCodes[0]);
        cout << "pre: " << endl;
        for(int j = 0; j < b.preBlocks.size(); j++)
        {
            outOptmidcode(dfg.blocks[b.preBlocks[j]].blockCodes[0]);
        }
        cout << "next: " << endl;
        for(int j = 0; j < b.sucBlocks.size(); j++)
        {
            outOptmidcode(dfg.blocks[b.sucBlocks[j]].blockCodes[0]);
        }
        cout << endl;
    }
}

void opt::_DFG_Analysis(std::vector<midcode> org_midcodes) {
    std::vector<midcode> glbmicos = _getGlbMicos(org_midcodes);
    std::vector<optMidcode> midcodes = copy_midcode(org_midcodes);
    std::vector<DFG> dfgs = _splitDFGS(midcodes);
    for(int i = 0; i < dfgs.size(); i++)
    {
        DFG now_dfg = dfgs[i];
        now_dfg.markFirst();
        now_dfg.splitBlocks();
        now_dfg.linkBlocks();
        ConstProp cp;
        cp.init(now_dfg, glbmicos);
        cp.analyze();
    }
}


var makeVar(string name, int index) {
    var result;
    result.name = name;
    result.index = index;
    return result;
}


bool _isBase_mico(symbol s) {
    if(s.i_type == 0 || s.i_type == 3)
    {
        return true;
    }
    if(s.i_type == 1)
    {
        return s.i_para==0 ? true : false;
    }
    return false;
}


bool _isBase_optmico(optMidcode m) {
    if(m.s_operation == "int" || m.s_operation == "char")
    {
        return true;
    }
    if(m.s_result[0] == '$')
    {
        return true;
    }
    return false;
}


bool _isInit(symbol s) {
    if(s.i_type == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool _isDeclare(optMidcode m) {
    if(m.s_operation == "int" || m.s_operation == "char" || m.s_operation == "inta" || m.s_operation == "chara"){
        return true;
    }
    else if(m.s_operation == "+" || m.s_operation == "-" || m.s_operation == "*" || m.s_operation == "/")
    {
        if(m.s_result[0] == '$')
        {
            return true;
        }
    }
    return false;
}


void ConstProp::init(DFG dfg_now, vector<midcode> glbmicos) {
    dfg = dfg_now;
    int index = 0;
    //处理全局变量
    for(int i = 0; i < glbmicos.size(); i++)
    {
        midcode micoTemp = glbmicos[i];
        if(micoTemp.s_operation == "func")
        {
            glb_num = index;
            break;
        }

        var varTemp = makeVar(micoTemp.s_result, index++);
        vars.push_back(varTemp);

        double val = 0;
        if(micoTemp.s_operation == "const")
        {
            val = atoi(micoTemp.s_betaVar.c_str());
        }
        else if(micoTemp.s_operation == "inta" || micoTemp.s_operation == "chara")
        {
            val = NAC;
        }
        boundVals.push_back(val);
    }
    //处理函数参数
    for(int i = 0; i < dfg.paras.size(); i++)
    {
        var varTemp = makeVar(dfg.paras[i].s_result, index++);
        vars.push_back(varTemp);
        boundVals.push_back(NAC);
    }
    //处理函数内局部变量
    for(int i = 0; i < dfg.midcodes.size(); i++)
    {
        optMidcode temp = dfg.midcodes[i];
        if(_isDeclare(temp))
        {
            var varTemp = makeVar(temp.s_result, index++);
            vars.push_back(varTemp);
            double val = UNDEF;
            if(!_isBase_optmico(temp))
            {
                val = NAC;
            }
            boundVals.push_back(val);
        }
    }
    while(index--)
    {
        initVals.push_back(UNDEF);
    }

    // for(int i = 0; i < vars.size(); i++)
    // {
    //     cout << vars[i].index << ": " << vars[i].name;
    //     if(boundVals[i] == NAC)
    //     {
    //         cout << "  NAC";
    //     }
    //     else if(boundVals[i] == UNDEF)
    //     {
    //         cout << "  UNDEF";
    //     }
    //     else
    //     {
    //         cout << "  " << boundVals[i];
    //     }
    //     if(initVals[i] == NAC)
    //     {
    //         cout << "  NAC";
    //     }
    //     else if(initVals[i] == UNDEF)
    //     {
    //         cout << "  UNDEF";
    //     }
    //     else
    //     {
    //         cout << "  " << initVals[i];
    //     }
    //     cout << endl;
    // }
}


double ConstProp::join(double left, double right) {
    if(left == NAC || right == NAC)
    {
        return NAC;
    }
    else if(left == UNDEF)
    {
        return right;
    }
    else if(right == UNDEF)
    {
        return left;
    }
    else
    {
        return NAC;
    }
}


void ConstProp::join(Block& b) {
    // cout << dfg.blocks[b.preBlocks[0]].outVals[0] << endl;
    for(int i = 0; i < b.inVals.size(); i++)
    {
        double val = UNDEF;
        // cout << b.preBlocks.size() << endl;
        // outOptmidcode(b.preBlocks[0].blockCodes[0]);
        // cout << b.preBlocks[0].outVals[0] << endl;
        for(int j = 0; j < b.preBlocks.size(); j++)
        {
            Block& preb = dfg.blocks[b.preBlocks[j]];
            val = join(val, preb.outVals[i]);
        }
        b.inVals[i] = val;
    }
}


int ConstProp::searchVar(string s) {
    for(int i = glb_num; i < vars.size(); i++)
    {
        if(vars[i].name == s)
        {
            return vars[i].index;
        }
    }
    for(int i = 0; i < glb_num; i++)
    {
        if(vars[i].name == s)
        {
            return vars[i].index;
        }
    }
    return -1;
}


double calculate(string op, double a, double b) {
    if(op == "+" || op == "-" || op == "*" || op == "/")
    {
        if(a == NAC || b == NAC)
        {
            return NAC;
        }
        else if(a == UNDEF || b == UNDEF)
        {
            return UNDEF;
        }
        

        if(op == "+")
        {
            return a+b;
        }
        else if(op == "-")
        {
            return a-b;
        }
        else if(op == "*")
        {
            return a*b;
        }
        else if(op == "/")
        {
            return a/b;
        }
    }
}


void ConstProp::translate(optMidcode m, vector<double>& in, vector<double>& out) {
    out = in;
    string op = m.s_operation;
    string arg1 = m.s_alphaVar;
    string arg2 = m.s_betaVar;
    string result = m.s_result;

    if(op == "=")
    {
        double temp;
        if(_isNumber(arg1))
        {
            temp = _TranNumber(arg1);
        }
        else
        {
            int pos = searchVar(arg1);
            if(pos == -1)
            {
                assert(0);
            }
            temp = in[pos];
        }
        int pos = searchVar(result);
        if(pos == -1)
        {
            assert(0);
        }
        out[pos] = temp;
    }
    else if(op == "+" || op == "-" || op == "*" || op == "/")
    {
        double temp;
        double a, b;
        if(_isNumber(arg1))
        {
            a = _TranNumber(arg1);
        }
        else
        {
            int pos = searchVar(arg1);
            if(pos == -1)
            {
                assert(0);
            }
            a = in[pos];
        }
        if(_isNumber(arg2))
        {
            b = _TranNumber(arg2);
        }
        else
        {
            int pos = searchVar(arg2);
            if(pos == -1)
            {
                assert(0);
            }
            b = in[pos];
        }
        temp = calculate(op, a, b);
        int pos = searchVar(result);
        if(pos == -1)
        {
            assert(0);
        }
        out[pos] = temp;
    }
    else if(op == "call")
    {
        for(int i = 0; i < glb_num; i++)
        {
            int pos = vars[i].index;
            out[pos] = NAC;
        }
        //format: x=func()
        if(result != "")
        {
            int pos = searchVar(result);
            if(pos == -1)
            {
                assert(0);
            }
            out[pos] = NAC;
        }
    }
}


bool ConstProp::translate(Block& b) {
    vector<double> in = b.inVals;
    vector<double> out = in;
    for(int i = 0; i < b.blockCodes.size(); i++)
    {
        optMidcode nowcode = b.blockCodes[i];
        translate(nowcode, in, out);
        in = out;
    }
    bool flag = false; 
    for(int i = 0; i < out.size(); i++)
    {
        if(b.outVals[i] != out[i])
        {
            flag = true;
            break;
        }
    }
    b.outVals = out;
    return flag;
}