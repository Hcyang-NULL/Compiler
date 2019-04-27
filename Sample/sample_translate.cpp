#include <iostream>
#include <stdlib.h>
#include "config.h"
#include <vector>
#include <stdio.h>

using namespace std;

vector<struct inter_code> t_intercodes;
vector<string> sentences;
// FILE *fp;

void traslate_one(struct inter_code now){
    string cmd;
    if(now.opt1[0] >= '0' && now.opt1[0] <= '9'){
        cmd = "li $t0, "+now.opt1+"\n";
        cout << cmd;
    }
    else{
        cmd = "subi $sp, $sp, 4\n";
        cout << cmd;
        cmd = "lw $t0, ($sp)\n";
        cout << cmd;
    }
    if(now.opt2[0] >= '0' && now.opt2[0] <= '9'){
        cmd = "li $t1, "+now.opt2+"\n";
        cout << cmd;
    }
    else{
        cmd = "subi $sp, $sp, 4\n";
        cout << cmd;
        cmd = "lw $t1, ($sp)\n";
        cout << cmd;
    }
    if(now.opt == OPT_ADD){
        cmd = "add $t0, $t0, $t1\n";
        cout << cmd;
    }
    else if(now.opt == OPT_MUL){
        cmd = "mul $t0, $t0, $t1\n";
        cout << cmd;
    }
    cmd = "sw $t0, ($sp)\n";
    cout << cmd;
    cmd = "addi $sp, $sp, 4\n";
    cout << cmd;
}

void translate_process(){
    // fp = fopen("mips_out.txt", "w");
    freopen("mips_out.asm", "w", stdout);
    for(int i = 0; i < t_intercodes.size(); i++){
        struct inter_code now = t_intercodes[i];
        traslate_one(now);
    }
    // fclose(fp);
}

void translate_initialize(vector<struct inter_code> g_intercodes){
    t_intercodes = g_intercodes;
}