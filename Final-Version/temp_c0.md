# 文法

1. <程序> ::= <常量说明> <变量说明> <辅助1> <主函数>
&lt;program> ::= <const_Declare> <varible_Declare> <assist_1> <main_Func>

2. <常量说明> ::= const <常量定义> ; <常量说明> | ε
<const_Declare> ::= const <const_Define> ; <const_Declare> | ε

3. <常量定义> ::= int <辅助16> | char <辅助16>
<const_Define> ::= int <assist_16> | char <assist_16>

4. <辅助16> ::= <标识符> = <整数> <辅助17> | <标识符> = <字符> <辅助17>
<assist_16> ::= TAG  = NUM <assist_17> | TAG  = CHAR <assist_17>

5. <辅助17> ::= , <辅助16> | ε
<assist_17> ::= , <assist_16> | ε

6. <变量说明> ::= <声明头部> <辅助2> <辅助3> ; <变量说明> | ε
<varible_Declare> ::= <head_State> <assist_2> <assist_3> ; <varible_Declare> | ε

7. <辅助2> ::= [ <无符号整数> ] | ε
<assist_2> ::= [ NUM ] | ε

8. <辅助3> ::= , TAG <辅助2> <辅助3> | ε
<assist_3> ::= , TAG <assist_2> <assist_3> | ε

9. <辅助1> ::= <有返回值函数定义> <辅助1> | <无返回值函数定义> <辅助1> | ε
<assist_1> ::= <return_func_Declare> <assist_1> | <void_func_Declare> <assist_1> | ε

10. <参数表> ::= <声明头部> <辅助18> | ε
<arg_List> ::= <head_State> <assist_18> | ε

11. <辅助18> ::= , <参数表> | ε
<assist_18> ::= , <arg_List> | ε

12. <复合语句> ::= <常量说明> <变量说明> <语句列>
<compound_Sentence> ::= <const_Declare> <varible_Declare> <sentence_List>

13. <声明头部> ::= <类型标识符> <标识符>
<head_State> ::= <type_Iden> &lt;identity>

14. <语句列> ::= <语句> <语句列> | ε
<sentence_List> ::= &lt;sentence> <sentence_List> | ε

15. <语句> ::= <条件语句> | <循环语句> | { <语句列> } | <标识符> <辅助13> ; | scanf( <标识符> <辅助4> ); | printf( <辅助5> ); | ; | <情况语句> | return <辅助6> ;
&lt;sentence> ::= <if_Sentence> | <for_Sentence> | { <sentence_List> } | TAG <assist_13> ; | sanf( TAG <assist_4> ); | printf( <assist_5> ); | ; | <switch_Sentence> | return <assist_6> ;

16. <条件语句> ::= if( <条件> ) <语句> <辅助7>
<if_Sentence> ::= if( &lt;condition> ) &lt;sentence> <assist_7>

17. <循环语句> ::= while ( <条件> ) <语句>
<for_Sentence> ::= while ( <条件> ) <语句>

18. <情况语句> ::= switch( <表达式> ){ <情况表> }
<switch_Sentence> ::= switch( &lt;expression> ){ <condition_List> }

19. <主函数> ::= void main(){ <复合语句> }
<main_Func> ::= void main(){ <compound_Sentence> }

20. <有返回值函数定义> ::= <声明头部> ( <参数表> ){ <复合语句> } | ε
<return_func_Declare> ::= <head_State> ( <arg_List> ){ <compound_Sentence> } | ε

21. <无返回值函数定义> ::= void <标识符> ( <参数表> ){ <复合语句> }
<void_func_Declare> ::= void TAG ( <arg_List> ){ <compound_Sentence> }

22. <类型标识符> ::= int | char
<type_Iden> ::= int | char

23. <辅助13> ::= <函数调用语句> | <赋值语句>
<assist_13> ::= <func_Call> | <assign_Sentence>

24. <函数调用语句> ::= ( <值参数表> )
<func_Call> ::= ( <value_argList> )

25. <赋值语句> ::= = <表达式> | [ <表达式> ]= <表达式>
<assign_Sentence> ::= = &lt;expression> | [ &lt;expression> ]= &lt;expression>

26. <辅助4> ::= , <标识符> <辅助4> | ε
<assist_4> ::= , TAG <assist_4> | ε

27. <辅助5> ::= <字符串> <辅助9> | <表达式>
<assist_5> ::= STR <assist_9> | &lt;expression>

28. <辅助9> ::= , <表达式> | ε
<assist_9> ::= , &lt;expression> | ε

29. <辅助6> ::= ( <表达式> ) | ε
<assist_6> ::= ( &lt;expression> ) | ε

30. <表达式> ::= + <项> <辅助10> | - <项> <辅助10> | <项> <辅助10>
&lt;expression> ::= + &lt;item> <assist_10> | - &lt;item> <assist_10> | &lt;item> <assist_10>

31. <项> ::= <因子> <辅助11>
&lt;item> ::= &lt;factor> <assist_11>

32. <辅助10> ::= <加法运算符> <项> <辅助10> | ε
<assist_10> ::= + &lt;item> <assist_10> | - &lt;item> <assist_10> | ε

33. <因子> ::= <标识符> <辅助14> | <整数> | <字符> | ( <表达式> )
&lt;factor> ::= TAG <assist_14> | NUM | CHAR | ( &lt;expression> )

34. <辅助11> ::= <乘法运算符> <因子> <辅助11> | ε
<assist_11> ::= * &lt;factor> <assist_11> | / &lt;factor> <assist_11> | ε

35. <辅助14> ::= [ <表达式> ] | <函数调用语句> |  ε
<assist_14> ::= [ &lt;expression> ] | <func_Call> |  ε

36. <值参数表> ::= <表达式> <辅助12> | ε
<value_argList> ::= &lt;expression> <assist_12> | ε

37. <辅助12> ::= , <表达式> <辅助12> | ε
<assist_12> ::= , &lt;expression> <assist_12> | ε

38. <情况表> ::= <情况子语句> <情况表> | ε
<condition_List> ::= <condition_kid> <condition_List> | ε

39. <情况子语句> ::= case <整数> : <语句> | case <字符> : <语句>
<condition_kid> ::= case NUM : &lt;sentence> | case CHAR : &lt;sentence>

40. <条件> ::= <表达式> <辅助15>
&lt;condition> ::= &lt;expression> <assist_15>

41. <辅助15> ::= <关系运算符> <表达式> | ε
<assist_15> ::= 关系运算符 &lt;expression> | ε

42. <辅助7> ::= else <语句> | ε
<assist_7> ::= else &lt;sentence> | ε

43. <辅助8> ::= + | -
<assist_8> ::= + | -

44. <关系运算符> ::= < | <= | > | >= | != | ==

45. <加法运算符> ::= + | -

46. <常量> ::= <整数> | <字符>

47. <乘法运算符> ::= * | /

48. <整数> ::= 0 | + <无符号整数> | - <无符号整数> | <无符号整数>
