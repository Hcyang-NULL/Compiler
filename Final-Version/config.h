#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <math.h>

enum TYPE{
    NONE,         //δ֪����
    ERR,          //����
    OK,           //����ʶ�����
    STR,          //�ַ���
    NUM,          //����
    BOUND,        //���
    PRE_HEX,      //��ʱ
    HEX,          //ʮ����������
    DEC,          //ʮ��������
    OCT,          //�˽�������
    PRE_BIN,      //��ʱ
    BIN,          //����������
    CHAR,         //�ַ�
    TAG,          //��ʶ��
    KW_AUTO,      //�ؼ���
    KW_SHORT,     //�ؼ���            
    KW_INT,       //�ؼ���           
    KW_LONG,      //�ؼ���              
    KW_FLOAT,     //�ؼ���              
    KW_DOUBLE,    //�ؼ���              
    KW_CHAR,      //�ؼ���              
    KW_STRUCT,    //�ؼ���              
    KW_UNION,     //�ؼ���              
    KW_ENUM,      //�ؼ���              
    KW_TYDEDEF,   //�ؼ���              
    KW_CONST,     //�ؼ���              
    KW_UNSIGNED,  //�ؼ���              
    KW_SIGNED,    //�ؼ���              
    KW_EXTERN,    //�ؼ���              
    KW_REGISTER,  //�ؼ���              
    KW_STATIC,    //�ؼ���              
    KW_VOLATILE,  //�ؼ���              
    KW_VOID,      //�ؼ���              
    KW_ELSE,      //�ؼ���              
    KW_SWITCH,    //�ؼ���              
    KW_CASE,      //�ؼ���              
    KW_FOR,       //�ؼ���              
    KW_DO,        //�ؼ���              
    KW_WHILE,     //�ؼ���           
    KW_GOTO,      //�ؼ���              
    KW_CONTINUE,  //�ؼ���              
    KW_BREAK,     //�ؼ���              
    KW_DEFAULT,   //�ؼ���              
    KW_SIZEOF,    //�ؼ���              
    KW_RETURN,    //�ؼ���              
    KW_IF,        //�ؼ���    
    KW_PRINTF,    //�ؼ���
    KW_SCANF,     //�ؼ���      
    END           //�ؼ���         
};

enum OPT{
    OPT_ADD,
    OPT_MUL
};

struct inter_code{
    OPT opt;
    std::string opt1;
    std::string opt2;
    std::string result;
};

class midcode{
    public:
        std::string s_operation;
        std::string s_alphaVar;
        std::string s_betaVar;
        std::string s_result;
};

#endif