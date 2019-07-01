# 文法

## 中文定义

非终结符|推导式
:-:|:-:
程序 | <常量说明> <变量说明> <辅助1> <主函数>
常量说明 | const <常量定义> ; <常量说明> &#124; ε
常量定义 | int <辅助16> &#124; char <辅助16>
辅助16 | <标识符> = <整数> <辅助17> &#124; <标识符> = <字符> <辅助17>
辅助17 | , <辅助16> &#124; ε
变量说明 | <声明头部> <辅助2> <辅助3> ; <变量说明> &#124; ε
辅助2 | [ <无符号整数> ] &#124; ε
辅助3 | , TAG <辅助2> <辅助3> &#124; ε
辅助1 | <有返回值函数定义> <辅助1> &#124; <无返回值函数定义> <辅助1> &#124; ε
参数表 | <声明头部> <辅助18> &#124; ε
辅助18 | , <参数表> &#124; ε
复合语句 | <常量说明> <变量说明> <语句列>
声明头部 | <类型标识符> <标识符>
语句列 | <语句> <语句列> &#124; ε
语句 | <条件语句> &#124; <循环语句> &#124; { <语句列> } &#124; <标识符> <辅助13> ; &#124; scanf( <标识符 <辅助4> ); &#124; printf( <辅助5> ); &#124; ; &#124; <情况语句> &#124; return <辅助6> ;
条件语句 | if( <条件> ) <语句> <辅助7>
循环语句 | while ( <条件> ) <语句>
情况语句 | switch( <表达式> ){ <情况表> }
主函数 | void main(){ <复合语句> }
有返回值函数定义 | <声明头部> ( <参数表> ){ <复合语句> } &#124; ε
无返回值函数定义 | void <标识符> ( <参数表> ){ <复合语句> }
类型标识符 | int &#124; char
辅助13 | <函数调用语句> &#124; <赋值语句>
函数调用语句 | ( <值参数表> )
赋值语句 | = <表达式> &#124; [ <表达式> ]= <表达式>
辅助4 | , <标识符> <辅助4> &#124; ε
辅助5 | <字符串> <辅助9> &#124; <表达式>
辅助9 | , <表达式> &#124; ε
辅助6 | ( <表达式> ) &#124; ε
表达式 | + <项> <辅助10> &#124; - <项> <辅助10> &#124; <项> <辅助10>
项 | <因子> <辅助11>
辅助10 | <加法运算符> <项> <辅助10> &#124; ε
因子 | <标识符> <辅助14> &#124; <整数> &#124; <字符> &#124; ( <表达式> )
辅助11 | <乘法运算符> <因子> <辅助11> &#124; ε
辅助14 | [ <表达式> ] &#124; <函数调用语句> &#124;  ε
值参数表 | <表达式> <辅助12> &#124; ε
辅助12 | , <表达式> <辅助12> &#124; ε
情况表 | <情况子语句> <情况表> &#124; ε
情况子语句 | case <整数> : <语句> &#124; case <字符> : <语句>
条件 | <表达式> <辅助15>
辅助15 | <关系运算符> <表达式> &#124; ε
辅助7 | else <语句> &#124; ε
辅助8 | + &#124; -
关系运算符 | < &#124; <= &#124; > &#124; >= &#124; != &#124; ==
加法运算符 | + &#124; -
常量 | <整数> &#124; <字符>
乘法运算符 | * &#124; /
整数 | 0 &#124; + <无符号整数> &#124; - <无符号整数> &#124; <无符号整数>

## 实际代码对应符号

非终结符|推导式
:-:|:-:
&lt;program> | <const_Declare> <varible_Declare> <assist_1> <main_Func>
<const_Declare> | const <const_Define> ; <const_Declare> &#124; ε
<const_Define> | int <assist_16> &#124; char <assist_16>
<assist_16> | TAG  = NUM <assist_17> &#124; TAG  = CHAR <assist_17>
<assist_17> | , <assist_16> &#124; ε
<varible_Declare> | <head_State> <assist_2> <assist_3> ; <varible_Declare> &#124; ε
<assist_2> | [ NUM ] &#124; ε
<assist_3> | , TAG <assist_2> <assist_3> &#124; ε
<assist_1> | <return_func_Declare> <assist_1> &#124; <void_func_Declare> <assist_1> &#124; ε
<arg_List> | <head_State> <assist_18> &#124; ε
<assist_18> | , <arg_List> &#124; ε
<compound_Sentence> | <const_Declare> <varible_Declare> <sentence_List>
<head_State> | <type_Iden> &lt;identity>
<sentence_List> | &lt;sentence> <sentence_List> &#124; ε
&lt;sentence> | <if_Sentence> &#124; <for_Sentence> &#124; { <sentence_List> } &#124; TAG <assist_13> ; &#124; sanf( TAG <assist_4> ); &#124; printf( <assist_5> ); &#124; ; &#124; <switch_Sentence> &#124; return <assist_6> ;
<if_Sentence> | if( &lt;condition> ) &lt;sentence> <assist_7>
<for_Sentence> | while ( <条件> ) <语句>
<switch_Sentence> | switch( &lt;expression> ){ <condition_List> }
<main_Func> | void main(){ <compound_Sentence> }
<return_func_Declare> | <head_State> ( <arg_List> ){ <compound_Sentence> } &#124; ε
<void_func_Declare> | void TAG ( <arg_List> ){ <compound_Sentence> }
<type_Iden> | int &#124; char
<assist_13> | <func_Call> &#124; <assign_Sentence>
<func_Call> | ( <value_argList> )
<assign_Sentence> | = &lt;expression> &#124; [ &lt;expression> ]= &lt;expression>
<assist_4> | , TAG <assist_4> &#124; ε
<assist_5> | STR <assist_9> &#124; &lt;expression>
<assist_9> | , &lt;expression> &#124; ε
<assist_6> | ( &lt;expression> ) &#124; ε
&lt;expression> | + &lt;item> <assist_10> &#124; - &lt;item> <assist_10> &#124; &lt;item> <assist_10>
&lt;item> | &lt;factor> <assist_11>
<assist_10> | + &lt;item> <assist_10> &#124; - &lt;item> <assist_10> &#124; ε
&lt;factor> | TAG <assist_14> &#124; NUM &#124; CHAR &#124; ( &lt;expression> )
<assist_11> | * &lt;factor> <assist_11> &#124; / &lt;factor> <assist_11> &#124; ε
<assist_14> | [ &lt;expression> ] &#124; <func_Call> &#124;  ε
<value_argList> | &lt;expression> <assist_12> &#124; ε
<assist_12> | , &lt;expression> <assist_12> &#124; ε
<condition_List> | <condition_kid> <condition_List> &#124; ε
<condition_kid> | case NUM : &lt;sentence> &#124; case CHAR : &lt;sentence>
&lt;condition> | &lt;expression> <assist_15>
<assist_15> | 关系运算符 &lt;expression> &#124; ε
<assist_7> | else &lt;sentence> &#124; ε
<assist_8> | + &#124; -

