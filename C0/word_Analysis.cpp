#include<iostream>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include "config.h"

using namespace std;

TYPE s, iden;

#define BUFFER_SIZE 100  //��������С

FILE *fp;  //�ļ�ָ��
string now = "";  //�����ļ������뵱ǰtoken
char buffer[BUFFER_SIZE];  //������
char c;  //��ǰ��ȡ�ַ�
vector<pair<string, TYPE> > w_tokens;  //�зֵ�token���
int row;  //��ǰ����

/**
 * �ӻ�������ȡһ���ַ���c��������������������´��ļ���ȡ��������
 */
void scan(){
    static int p = BUFFER_SIZE;
    static int top = BUFFER_SIZE;
    if(p == top){
        if(top != BUFFER_SIZE){
            c = 0;
            return;
        }
        top = fread(buffer, 1, BUFFER_SIZE, fp);
        p = 0;
    }
    if(buffer[p] == '\0'){
        c = 0;
    }
    else{
        c = buffer[p++];
    }
}

/**
 * ����DFA
 */
void NUM_DFA(){
    if(c == '0'){
        iden = s;
        s = OCT;
        now += c;
        scan();
        {
            if(c >= '0' && c <= '7'){  //ʶ��˽�����
                now += c;
                scan();
            }
            else if(c == 'x'){  //ʶ��ʮ��������
                iden = s;
                s = PRE_HEX;
                now += c;
                scan();
            }
            else if(c == 'b'){  //ʶ���������
                iden = s;
                s = PRE_BIN;
                now += c;
                scan();
            }
        }
        while(s != OK && s != ERR){
            switch (s) {
            case PRE_HEX:
                {
                    if(c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'){
                        iden = s;
                        s = HEX;
                        now += c;
                        scan();
                    }
                    else{
                        s = ERR;
                    }
                } 
                break;
            case HEX:
                {
                    if(c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'){
                        now += c;
                        scan();
                    }
                    else{
                        s = OK;
                    }
                }
                break;
            case PRE_BIN:
                {
                    if(c == '0' || c == '1'){
                        iden = s;
                        s = BIN;
                        now += c;
                        scan();
                    }
                    else{
                        s = ERR;
                    }
                }
                break;
            case BIN:
                {
                    if(c == '0' || c == '1'){
                        iden = s;
                        s = BIN;
                        now += c;
                        scan();
                    }
                    else{
                        s = OK;
                    }
                }
                break;
            case OCT:
                {
                    if(c >= '0' && c <= '7'){
                        now += c;
                        scan();
                    }
                    else{
                        iden = s;
                        s = OK;
                    }
                }
                break;
            }
        }
        return;
    }
    else{  //ʶ��ʮ������
        iden = s;
        s = DEC;
        now += c;
        scan();
        while(s != ERR && s != OK){
            if(c >= '0' && c <= '9'){
                now += c;
                scan();
            }
            else{
                iden = s;
                s = OK;
            }
        }
        return;
    }
}

/**
 * �ַ�DFA
 */
void CHAR_DFA(){
    scan();
    while(s != ERR && s != OK){
        if(c == '\''){
            iden = s;
            s = ERR;
        }
        else if(c == '\\'){
            scan();
            if(c == '\n' || c == 0){
                iden = s;
                s = ERR;
            }
            else{
                now += c;
                scan();
                if(c == '\''){
                    iden = s;
                    s = OK;
                    scan();
                }
                else{
                    iden = s;
                    s = ERR;
                }
            }
        }
        else{
            now += c;
            scan();
            if(c == '\''){
                iden = s;
                s = OK;
                scan();
            }
            else{
                iden = s;
                s = ERR;
            }
        }
    }
}

/**
 * �ַ���DFA
 */
void STR_DFA(){
    scan();
    while(s != ERR && s != OK){
        if(c == 0 || c == '\n'){
            iden = s;
            s = ERR;
        }
        else if(c == '\\'){
            scan();
            if(c == 0){
                iden = s;
                s = ERR;
            }
            else{
                now += c;
            }
        }
        else if(c == '"'){
            iden = s;
            s = OK;
            scan();
        }
        else{
            now += c;
            scan();
        }
    }
}

/**
 * ��ʶ��DFA
 */
void TAG_DFA(){
    now += c;
    while(s != ERR && s != OK){
        scan();
        if(c == '_' || c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
            now += c;
        }
        else{
            iden = s;
            s = OK;
        }
    }
}

/**
 * ���DFA
 */
void BOUND_DFA(){
    now += c;
    iden = s;
    s = OK;
    if(c == '+'){
        scan();
        if(c == '+'){
            now += c;
            scan();
        }
    }
    else if(c == '-'){
        scan();
        if(c == '-'){
            now += c;
            scan();
        }
    }
    else if(c == '&'){
        scan();
        if(c == '&'){
            now += c;
            scan();
        }
    }
    else if(c == '|'){
        scan();
        if(c == '|'){
            now += c;
            scan();
        }
    }
    else if(c == '>'){
        scan();
        if(c == '='){
            now += c;
            scan();
        }
    }
    else if(c == '<'){
        scan();
        if(c == '='){
            now += c;
            scan();
        }
    }
    else if(c == '='){
        scan();
        if(c == '='){
            now += c;
            scan();
        }
    }
    else if(c == '!'){
        scan();
        if(c == '='){
            now += c;
            scan();
        }
    }else{
        scan();
    }
}

void add_token(){
    if(s == OK){
        cout << now << " ";
        switch(iden){
            case HEX:cout << "ʮ��������" << endl;break;
            case DEC:cout << "ʮ������" << endl;break;
            case OCT:cout << "�˽�����" << endl;break;
            case BIN:cout << "��������" << endl;break;
            case BOUND:cout << "���" << endl;break;
            case STR:cout << "�ַ�������" << endl;break;
            case CHAR:cout << "�ַ�����" << endl;break;
            case TAG:{
                if(now == "auto"){
                    iden = KW_AUTO;
                }
                else if(now == "short"){
                    iden = KW_SHORT;
                }
                else if(now == "int"){
                    iden = KW_INT;
                }    
                else if(now == "long"){
                    iden = KW_LONG;
                }     
                else if(now == "float"){
                    iden = KW_FLOAT;
                }     
                else if(now == "double"){
                    iden = KW_DOUBLE;
                }
                else if(now == "char"){
                    iden = KW_CHAR;
                }  
                else if(now == "struct"){
                    iden = KW_STRUCT;
                }  
                else if(now == "union"){
                    iden = KW_UNION;
                }  
                else if(now == "enum"){
                    iden = KW_ENUM;
                } 
                else if(now == "typedef"){
                    iden = KW_TYDEDEF;
                }  
                else if(now == "const"){
                    iden = KW_CONST;
                } 
                else if(now == "unsigned"){
                    iden = KW_UNSIGNED;
                }
                else if(now == "signed"){
                    iden = KW_SIGNED;
                }  
                else if(now == "extern"){
                    iden = KW_EXTERN;
                }   
                else if(now == "register"){
                    iden = KW_REGISTER;
                } 
                else if(now == "static"){
                    iden = KW_STATIC;
                }    
                else if(now == "volatile"){
                    iden = KW_VOLATILE;
                } 
                else if(now == "void"){
                    iden = KW_VOID;
                }   
                else if(now == "else"){
                    iden = KW_ELSE;
                }    
                else if(now == "switch"){
                    iden = KW_SWITCH;
                }     
                else if(now == "case"){
                    iden = KW_CASE;
                }   
                else if(now == "for"){
                    iden = KW_FOR;
                }   
                else if(now == "do"){
                    iden = KW_DO;
                }   
                else if(now == "while"){
                    iden = KW_WHILE;
                }    
                else if(now == "goto"){
                    iden = KW_GOTO;
                }  
                else if(now == "continue"){
                    iden = KW_CONTINUE;
                }
                else if(now == "break"){
                    iden = KW_BREAK;
                } 
                else if(now == "default"){
                    iden = KW_DEFAULT;
                }   
                else if(now == "sizeof"){
                    iden = KW_SIZEOF;
                } 
                else if(now == "return"){
                    iden = KW_RETURN;
                }    
                else if(now == "if"){
                    iden = KW_IF;
                }
                else if(now == "printf"){
                    iden = KW_PRINTF;
                }
                else if(now == "scanf"){
                    iden = KW_SCANF;
                }
                else{
                    cout << "��ʶ��" << endl;
                    break;
                }
                cout << "�ؼ���" << endl;
                break;
            }
            default:cout << "����" << endl;break;
        };
        if(iden == HEX || iden == DEC || iden == OCT || iden == BIN){
            iden = NUM;
        }
        pair<string, TYPE> token(now, iden);
        w_tokens.push_back(token);
        now = "";
    }
    else{
        c = 0;
        cout << "Erro: row " << row+1 << endl;
    }
    s = NONE;
}

vector<pair<string,TYPE> > word_analyze(){
    s = NONE;
    scan();
    do{
        if(c >= '0' && c <= '9'){
            s = NUM;
            NUM_DFA();
        }
        else if(c == '"'){
            s = STR;
            STR_DFA();
        }
        else if(c == '\''){
            s = CHAR;
            CHAR_DFA();
        }
        else if(c == ' ' || c == '\t' || c == '\n'){
            if(c == '\n'){
                row++;
            }
            scan();
            continue;
        }
        else if(c == '_' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
            s = TAG;
            TAG_DFA();
        }
        else{
            s = BOUND;
            BOUND_DFA();
        }
        add_token();
    }while(c != 0);
    w_tokens.push_back(pair<string,TYPE>("",END));
    return w_tokens;
}

void word_initialize(string file){
    fp = fopen(file.c_str(),"r");
}

// int main(int argc, char *argv[]){
//     string file = "";
//     if(argc > 1){
//         file = argv[1];
//     }
//     else{
//         file = "C:\\Users\\HCY\\Desktop\\Compiler\\C0\\test.c";
//     }
//     cout << "�ʷ����������" << endl;
//     vector<pair<string, TYPE> > vec_tempFuncarg_1;
//     word_initialize(file);
//     vec_tempFuncarg_1 = word_analyze();
// }
