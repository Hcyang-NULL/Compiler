#ifndef CONFIG_H
#define CONFIG_H


enum TYPE{
    NONE,
    ERR,
    OK,
    STR,
    NUM,
    BOUND,
    PRE_HEX,
    HEX,
    DEC,
    OCT,
    PRE_BIN,
    BIN,
    CHAR,
    TAG,
    KW_AUTO,    
    KW_SHORT,   
    KW_INT,  
    KW_LONG,    
    KW_FLOAT,   
    KW_DOUBLE,  
    KW_CHAR,    
    KW_STRUCT,  
    KW_UNION,   
    KW_ENUM,    
    KW_TYDEDEF, 
    KW_CONST,   
    KW_UNSIGNED,
    KW_SIGNED,  
    KW_EXTERN,  
    KW_REGISTER,
    KW_STATIC,  
    KW_VOLATILE,
    KW_VOID,    
    KW_ELSE,    
    KW_SWITCH,  
    KW_CASE,    
    KW_FOR,     
    KW_DO,      
    KW_WHILE,
    KW_GOTO,    
    KW_CONTINUE,
    KW_BREAK,   
    KW_DEFAULT, 
    KW_SIZEOF,  
    KW_RETURN,  
    KW_IF,
    END
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