#ifndef CONFIG_H
#define CONFIG_H


enum TYPE{
    NONE,         //未知类型
    ERR,          //错误
    OK,           //单个识别完成
    STR,          //字符串
    NUM,          //数字
    BOUND,        //界符
    PRE_HEX,      //临时
    HEX,          //十六进制数字
    DEC,          //十进制数字
    OCT,          //八进制数字
    PRE_BIN,      //临时
    BIN,          //二进制数字
    CHAR,         //字符
    TAG,          //标识符
    KW_AUTO,      //关键字
    KW_SHORT,     //关键字            
    KW_INT,       //关键字           
    KW_LONG,      //关键字              
    KW_FLOAT,     //关键字              
    KW_DOUBLE,    //关键字              
    KW_CHAR,      //关键字              
    KW_STRUCT,    //关键字              
    KW_UNION,     //关键字              
    KW_ENUM,      //关键字              
    KW_TYDEDEF,   //关键字              
    KW_CONST,     //关键字              
    KW_UNSIGNED,  //关键字              
    KW_SIGNED,    //关键字              
    KW_EXTERN,    //关键字              
    KW_REGISTER,  //关键字              
    KW_STATIC,    //关键字              
    KW_VOLATILE,  //关键字              
    KW_VOID,      //关键字              
    KW_ELSE,      //关键字              
    KW_SWITCH,    //关键字              
    KW_CASE,      //关键字              
    KW_FOR,       //关键字              
    KW_DO,        //关键字              
    KW_WHILE,     //关键字           
    KW_GOTO,      //关键字              
    KW_CONTINUE,  //关键字              
    KW_BREAK,     //关键字              
    KW_DEFAULT,   //关键字              
    KW_SIZEOF,    //关键字              
    KW_RETURN,    //关键字              
    KW_IF,        //关键字          
    END           //关键字         
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

#endif