# 编译原理报告

姓名：胡川洋

学号：2016302362

## 1、总体设计

* 本编译器使用C++开发，主要工作分为六个部分：词法分析、语法分析、四元式生成、语法制导翻译、优化、主函数。

模块|概述|代码行数
:-:|:-:|:-:
词法分析|根据有限状态自动机来完成对单词的识别|485
语法分析|使用C0文法使用递归下降子程序|1218
四元式生成|根据递归下降子程序生成对应的四元式|45
语法制导翻译|根据四元式与符号表完成翻译|1035
优化|基于数据流分析框架进行优化|944
主函数|对各个子模块进行调用完成编译|197
总计||3924

注：四元式生成模块的部分代码在语法分析中，所有统计的代码行不包含空行，使用python程序统计得到

* 开发环境

|  配置项  |   环境    |
| :------: | :-------: |
|   IDE    |  VSCode   |
|    OS    | Windows10 |
| Compiler |    G++    |
|  Debug   |    GDB    |

## 2、详细设计

### 词法分析

* 使用DFA来进行对单词的识别，主要有数字常量识别DFA，字符常量识别DFA，字符串常量识别DFA，标识符识别DFA以及界符识别DFA
  * 数字常量识别DFA（支持十六进制、十进制、八进制、二进制）

  ![1561862180224](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561862180224.png)

  * 字符常量识别DFA

  ![1561862192008](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561862192008.png)

  * 字符串常量识别DFA

  ![1561862211937](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561862211937.png)

  * 标识符识别DFA

  ![1561862224888](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561862224888.png)

  * 界符识别DFA

  ![1561862330534](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561862330534.png)

* 词法分析结果

  * 若存在DFA无法识别的单词，直接给出错误信息
  * 若程序正确，得到的结果类型：**vector<pair<string, TYPE>>**
  * 其中每一对pair里的string为识别的单词，TYPE为其所对应的类别（在config.h中定义）
  * 例子：**[("void", KW_VOID), ("main", TAG), ("(", BOUND), (")", BOUND)]**

### 语法分析

采用C0文法，使用递归下降子程序实现语法分析

* 文法（原始定义）


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

* 实际代码对应符号

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

* 文法说明
  * 变量只支持int与char类型
  * 常量定义必须在程序最前
  * 循环只支持while
  * 主函数需放在最后
  
* 递归下降子程序

  * 根据文法进行终结符匹配，非终结符递归下降
  * 遇到不匹配的情况则直接输出错误以及对应信息

* LL2

  * 文法中有一个推导式中紧挨的两个非终结符具有相同前缀，这里单独做了处理：<变量申明>与<有返回值函数定义>以及<无返回值函数定义>存在相同前缀int或char，但一个是定义变量，一个是定义函数，因此单独判断后两个单词若为TAG与"("则为函数定义，否则为变量申明

  * 判断函数

    ```c++
    bool LL2_func_variable(){
        if(g_vec_grammarTokens.size() >= 2){
            if(g_vec_grammarTokens[0].second == TAG){
                if(g_vec_grammarTokens[1].first == "("){
                    return true;
                }
            }
        }
        return false;
    }
    ```

* 语法分析结果

  * 若存在语法错误，则直接给出错误信息（如缺少匹配的有括号等）
  * 若程序无语法错误，则语法分析正常通过，无变量生成

### 四元式生成

* 四元式定义

说明|原始语句|四元式
:-:|:-:|:-:
整型常量定义|const int a = -5|&lt;const, int, -5, a&gt;
字符常量定义|const char c = 'a'|&lt;const, char, 97, c&gt;
整型数组定义|int a[10]|&lt;inta, , 10, a&gt;
字符数组定义|char c[20]|&lt;chara, , 20, c&gt;
整型变量定义|int a|&lt;int, , , a&gt;
字符变量定义|char c|&lt;char, , , c&gt;
表达式负运算|-10|&lt;-, 0, 10, Var&gt;
表达式加法运算|1 + 2|&lt;+, 1, 2, Var&gt;
表达式减法运算|9 - 8|&lt;-, 9, 8, Var&gt;
表达式乘法运算|3 * 4|&lt;*, 3, 4, Var&gt;
表达式除法运算|7 / 6|&lt;/, 7, 6, Var&gt;
右值函数调用|a = handy()|&lt;call, handy, , Var&gt;
右值数组调用|a = b[4]|&lt;aAss, b, 4, Var&gt;
赋值语句|a = 1|&lt;=, a, , 1&gt;
左值数组调用赋值|a[3] = 5|&lt;[]=, 5, 3, a&gt;
左值函数调用|handy();|&lt;call, handy, , &gt;
关系运算|a < b|&lt;<, a, b, &gt;
特殊关系运算|a|&lt;!=, a, 0, &gt;
函数调用参数|handy(a);|&lt;cpara, , , a&gt;
函数定义参数|void handy(int a)|&lt;para, int, , a&gt;
函数定义参数|void handy(char c)|&lt;para, char, , c&gt;
读语句|scanf(a)|&lt;scf, , int, a&gt;
写语句|printf("a",b)|&lt;prtf, a, b, char&gt;
写语句|printf("a")|&lt;prtf, a, , char&gt;
写语句|printf(b)|&lt;prtf, , b, char&gt;
非主函数带参返回|return (expr)|&lt;ret, , , exist_Var&gt;
非主函数无参返回|return;|&lt;ret, , , &gt;
主函数返回|return|&lt;exit, , , &gt;
有返回函数定义|int handy()|&lt;func, int, , handy&gt;
有返回函数定义|char handy()|&lt;func, char, , handy&gt;
无返回函数定义|void handy()|&lt;func, void, , handy&gt;
函数结束|void handy(){}|&lt;end, , , handy&gt;
if语句|if(condition){sentence}|&lt;较多略&gt;
while语句|while(condition){sentence}|&lt;较多略&gt;

* 说明

  1. Var表示生成的变量（临时）
  2. exist_Var表示这个中间变量已经生成了
* 四元式生成结果

  * 四元式生成夹在语法分析中，在递归下降的过程中不断生成四元式

  * 若语法分析通过，得到结果类型：**vector< midcode >**

  * midcode定义：

    ```c++
    class midcode{
        public:
            std::string s_operation;
            std::string s_alphaVar;
            std::string s_betaVar;
            std::string s_result;
    };
    ```

### 语法制导翻译

* 根据生成的四元式依次按顺序进行翻译，同时使用符号表查找偏移与变量

* 样例：

  * 四元式：**<+, 1, a, $hcy_1>**

  * 翻译过程：

    1. 识别到operation为"+"，跳转至对应处理函数
    2. 识别第一个操作数为立即数，直接翻译：" li $t0 1 "
    3. 识别第二个操作数为变量，先在局部变量中寻找a
    4. 在局部变量表中找到叫a的变量，则获取其偏移f，翻译：" lw $t1 f($fp) "，进入第8步
    5. 没有在局部变量表中找到变量a，则在全局变量中找a
    6. 在全局变量表中找到叫a的变量，首先获取其相对地址：" la $t1 a "
    7. 根据相对地址提取值：" lw $t1 ($t1) "
    8. 翻译加法：" add $t0 $t0 $t1 "
    9. 识别结果操作数不为空，即要将运算的结果赋给一个变量
    10. 若变量为用户定义的变量（不以$开头），进入第13步，否则为翻译所生成的中间变量
    11. 将中间变量储存在符号表中同时sp指针移动：" subi $sp $sp 4 "
    12. 再获取到该中间变量的偏移f，进行赋值：" sw $t0 f($fp) "，进入第18步
    13. 在局部变量表中查找变量a
    14. 在局部变量表中找到叫a的变量，则获取其偏移f，赋值：" sw $t0 f($fp) "，进入第18步
    15. 没有在局部变量表中找到变量a，则在全局变量中找a
    16. 在全局变量表中找到叫a的变量，首先获取其相对地址：" la $t1 a "
    17. 赋值：" sw $t0 ($t1) "
    18. 结束

  * 翻译加法的源代码

    ```c++
    void mips_add(){
        // generate mips order of alpha operator
        if(_isNumber(g_curMidcode.s_alphaVar)){
            cout << "\t\tli\t$t0\t" << g_curMidcode.s_alphaVar << endl;
        }
        else{
            int tempAddr_alpha = -1*_locateAddr(g_curMidcode.s_alphaVar);
            // not find in local variable vector
            if(tempAddr_alpha == 1){
                cout << "\t\tla\t$t0\t" << g_curMidcode.s_alphaVar << endl;
                cout << "\t\tlw\t$t0\t($t0)" << endl;
            }
            // find
            else{
                cout << "\t\tlw\t$t0\t" << tempAddr_alpha << "($fp)" << endl;
            }
        }
        // generate mips order of beta operator
        if(_isNumber(g_curMidcode.s_betaVar)){
            cout << "\t\tli\t$t1\t" << g_curMidcode.s_betaVar << endl;
        }
        else{
            int tempAddr_beta = -1*_locateAddr(g_curMidcode.s_betaVar);
            // not find in local variable vector
            if(tempAddr_beta == 1){
                cout << "\t\tla\t$t1\t" << g_curMidcode.s_betaVar << endl;
                cout << "\t\tlw\t$t1\t($t0)" << endl;
            }
            // find
            else{
                cout << "\t\tlw\t$t1\t" << tempAddr_beta << "($fp)" << endl;
            }
        }
        // generate mips order of add
        cout << "\t\tadd\t$t0\t$t0\t$t1" << endl;
        // generate mips order of store of variable of result
        if(_isHcyVariable(g_curMidcode.s_result)){
            _storeLocal(g_curMidcode.s_result);
            int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
            cout << "\t\tsw\t$t0\t" << tempAddr_gama << "($fp)" << endl;
        }
        else{
            int tempAddr_gama = -1*_locateAddr(g_curMidcode.s_result);
            // not find
            if(tempAddr_gama == 1){
                cout << "\t\tla\t$t1\t" << g_curMidcode.s_result << endl;
                cout << "\t\tsw\t$t0\t($t1)" << endl;
            }
            // find
            else{
                cout << "\t\tsw\t$t0\t" << g_curMidcode.s_result << "($fp)" << endl;
            }
        }
    }
    ```

  * 翻译结果（实际运行得到）

    ```assembly
    li	$t0	1
    lw	$t1	-8($fp)
    add	$t0	$t0	$t1
    subi	$sp	$sp	4
    sw	$t0	-20($fp)
    ```
  
* 语法制导翻译结果

  * 得到的汇编语句结果输出到 mips.txt 中

### 优化

* 使用了数据流分析框架（DFG），而后实现了常量传播优化

* 数据流分析框架
  * 针对每一个函数内部进行
  * 基本要素
    * 数据流方向：前向或是后向（与代码执行方向比较）
    * 值集：初值与数据流分析中使用的值
    * 交汇函数：对于前向数据流，用于合并基本块的多个前驱块的数据流作为基本块的in值，对于逆向数据流，则用于合并基本块的多个后继块的数据流作为基本块的out值
    * 传递函数：对于前向数据流，用于将基本块的in值转化为out值，对于逆向数据流，则用于将基本块的out值转换为in值
  * 首指令：用于切分基本块
    * 第一条指令
    * 跳转指令的目标指令
    * 跳转指令之后的指令
  * 分析流程
    1. 标定首指令
    2. 根据首指令切分块（block）
    3. 根据指令类别确定块间联系
    4. 初始化值集
    5. 根据传递函数与交汇函数进行迭代更新直至各个块的数据流不再变化
  
* 常量传播
  * 将能够计算出结果的变量直接替换为常量
  
  * 半格定义
    * 最大值（表示未定义）：UNDEF
    * 最小值（表示无确定常量值）：NAC
    * 常量值
    
  * 常量传播数据流分析框架
    * 数据流方向：前向
    * 值集：全局变量、局部变量、参数变量
    * 交汇函数：半格间最大下界运算⊕
      * 任意值a：UNDEF &oplus; v = v，NAC &oplus; V =NAC
      * 常量值c：c &oplus; c = c
      * 常量值c1，c2（c1 &ne; c2）：c1 &oplus; c2 = NAC
    * 传递函数
      * 块间传递函数fb（基本块B）：B.out = fb(B.in)
      * 指令传递函数fs（变量x，指令s）：
        * 常量赋值语句 x = c：s.out[x] = c
        * 运算赋值语句 x = a &bigodot; b：
          * 情况1：a，b为确定值，则 s.out[x] = a &bigodot; b
          * 情况2：a或b为NAC，则 s.out[x] = NAC
          * 情况3：其他所有情形下 s.out[x] = UNDEF
        * 调用函数语句 func() ：对于任意全局变量g有 s.out[g] = NAC
        * 调用函数并赋值语句 x = func()：对于任意全局变量g有 s.out[g] = NAC且 s.out[x] = NAC
        * 其他所有语句：s.out[x] = s.in[x]
    
  * 优化
  
    * 在使用常量传播数据流分析框架对所有块进行分析后，即下一次迭代的数据流不会再变化了，此时若变量在传递时的值为确定值（即不为UNDEF或NAC）则可将此语句直接优化为赋值语句
  
    * 例子
  
      ```c++
      a = 1;
      b = 2;
      c = a+b;
      ```
  
      则 c = a+b; 将会被优化为 c = 3;
  
    *  实际运行例子
  
      * 源程序
  
        ```c++
        const int g = 5;
        
        int fun()
        {
            int a, b, c;
            b = 7+g;
            a = 5;
            if(b)
            {
                b = b+1;
            }
            b = b+1;
            c = a+g;
            return (b);
        }
        
        void main()
        {
            fun();
        }
        ```
  
      * 未优化的四元式集合
  
        ```
        < const, int, 5, g>
        < func, int, , fun>
        < int, , , a>
        < int, , , b>
        < int, , , c>
        < +, 7, g, $hcy_1>
        < =, $hcy_1, , b>
        < =, 5, , a>
        < !=, b, 0, >
        < jne, , , _LABLE_1>
        < +, b, 1, $hcy_2>
        < =, $hcy_2, , b>
        < jmp, , , _LABLE_2>
        < lab, , , _LABLE_1>
        < lab, , , _LABLE_2>
        < +, b, 1, $hcy_3>
        < =, $hcy_3, , b>
        < +, a, g, $hcy_4>
        < =, $hcy_4, , c>
        < ret, , , b>
        < end, , , fun>
        < func, , , main>
        < call, fun, , >
        < end, , , main>
        ```
  
      * 优化后的四元式集合
  
        ```
        < const, int, 5, g>
        < func, int, , fun>
        < int, , , a>
        < int, , , b>
        < int, , , c>
        < =, 12, , $hcy_1>
        < =, 12, , b>
        < =, 5, , a>
        < !=, b, 0, >
        < jne, , , _LABLE_1>
        < =, 13, , $hcy_2>
        < =, 13, , b>
        < jmp, , , _LABLE_2>
        < lab, , , _LABLE_1>
        < lab, , , _LABLE_2>
        < +, b, 1, $hcy_3>
        < =, $hcy_3, , b>
        < =, 10, , $hcy_4>
        < =, 10, , c>
        < ret, , , b>
        < end, , , fun>
        < func, , , main>
        < call, fun, , >
        < end, , , main>
        ```
  
      * 说明
  
        * 可以看到 b = 7+g; 由于g是全局常量，因此直接被优化为 b = 12;
        * if语句在翻译时由于其存在判断因此肯定会存在首指令，那么指令 a = 5; 与指令 c = a+g; 肯定不在一个基本块里，经过常量分析数据流传播，最终指令 c = a+g; 也被优化为了 c = 10; 因此只要两个指令间没有对其中涉及到变量进行一些操作（导致变量变为NAC或UNDEF），常量传播都可进行优化，与距离无关
  
* 优化结果

  * 优化代码在将四元式集合送入翻译之前对四元式集合进行优化，因此结果直接体现在四元式集合上

### 符号表设计

* 符号表类

  ```c++
  class symbolTable{
      public:
          int i_topIndex;
          int i_totalProgram;
          vector<int> vec_programIndex;
      	vector<symbol> vec_symbols;
  };
  ```

  **说明**

  |              变量              |             说明             |
  | :----------------------------: | :--------------------------: |
  |           i_topIndex           |    符号表中的所有变量个数    |
  |         i_totalProgram         |    符号表中的所有函数数量    |
  | vector< int > vec_programIndex | 每一个函数对应的变量开始位置 |
  |  vector< symbol > vec_symbols  |       所有变量具体信息       |

* 符号类

  ```c++
  class symbol{
      public:
          string s_name;
          int i_type;
          int i_value;
          int i_address;
          int i_para;
  };
  ```

  **说明：**

  s_name：符号的名字

  i_type：

  | 取值 |  类别  |
  | :--: | :----: |
  |  0   | 常量名 |
  |  1   | 变量名 |
  |  2   | 函数名 |
  |  3   | 参数名 |

  i_value：

  | i_type取值 | i_value取值 |    类别    |
  | :--------: | :---------: | :--------: |
  |     0      |     any     |  常量的值  |
  |     1      |      0      | int型变量  |
  |     1      |      1      | char型变量 |
  |     2      |      0      | void型函数 |
  |     2      |      1      | int型函数  |

  i_address：符号对应的偏移

  i_para：符号对应的数组大小（如果是数组的话，否则为0）

### 中间变量与表达式栈

* 由于存在1+2+3+4这样的表达式，但按照语法分析不能一次分析完，先是1+(...)，而后再依次如此分析因此每一步需要一个中间量来存储，为了不和用户定义变量冲突，我定义中间变量为字符串**"$hcy_"**加上一个数字，这个数字用全局变量计数控制，保证不会有两个地方用到相同名字的中间变量

* 在处理长运算表达式时，我经过仔细的分析，发现符合栈的特性，以1+2+3+4为例，首先1压栈，读取运算符+，而后2压栈，这时由于没有高优先级运算符，处理+号，为取得操作数，弹栈两次，生成加法四元式，结果存入$hcy_1中，再将$hcy_1压栈，再读取+号，读取3压栈，此时处理刚读取的+号，弹栈两次即$hcy_1+3，结果存入$hcy_2，再压栈，如此进行

* 若存在括号，高级运算符时，仍然满足这个特性，如(1+2) * (3+4)，先是处理为($hcy_1) * (3+4)，而后由于有括号，语法分析会先进入括号中，处理为($hcy_1) * ($hcy_2)，最后再处理为$hcy_3，结束

* 因此，在处理表达式时，设计了表达式栈，用于辅助生成对应的四元式

* 例子：

  * 表达式：a = 1 * ( 2 + 3 / 4 ) - 5;

  * 四元式结果：

    ![1561969022120](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561969022120.png)

## 3、测试

### 基础测试

* 基本上覆盖绝大部分测试点

* 测试程序：

  ```c++
  const int g_i1 = 10, g_i2 = 5;
  const char g_c1 = 'a', g_c2 = 'A';
  int g_i3, g_i4;
  char g_c3, g_c4;
  
  void func1()
  {
      int g_i2;
      char g_c1;
      g_i2 = 6;
      g_c1 = 'C';
      printf("\n>> func1 test");
      printf("\ng_i1 = ", g_i1);
      printf("\ng_c2 = ", g_c2);
      g_i4 = 100;
      g_c3 = 'H';
      printf("\ng_i4 = ", g_i4);
      printf("\ng_c3 = ", g_c3);
      printf("\ng_i2 = ", g_i2);
      printf("\ng_c1 = ", g_c1);
      return;
  }
  
  int func2()
  {
      int a;
      char b;
      printf("\n>> func2 test");
      printf("\ninput integer a:");
      scanf(a);
      printf("confirm a: ", a);
      printf("\ninput character b:");
      scanf(b);
      printf("confirm b: ", b);
      return (0);
  }
  
  char func3()
  {
      int a,b,c;
      printf("\n>> func3 test");
      a = 1+(2-3)*4;
      b = g_i1/g_i2;
      c = -(a+b);
      printf("\nc = ", c);
      return ('K');
  }
  
  void func4(int p1)
  {
      printf("\n>> func4 test");
      while(p1 < 4)
      {
          if(p1 == 0)
          {
              printf("\np1 = 0");
          }
          else if(p1 == 1)
          {
              printf("\np1 = 1");
          }
          else if(p1 == 2)
          {
              printf("\np1 = 2");
          }
          else
          {
              printf("\np1 = ", p1);
          }
          p1 = p1+1;
      }
      return;
  }
  
  int func5(char p1)
  {
      char c[3];
      printf("\n>> func5 test");
      c[0] = 'a';
      c[1] = 'g';
      c[2] = 'z';
      g_c4 = c[1];
      printf("\nc[1] = ", g_c4);
      return (0);
  }
  
  char func6(int p1, char p4)
  {
      int a;
      char c[26], now;
      printf("\n>> func6 test");
      now = 'a';
      a = 0;
      while(a < 26)
      {
          c[a] = now;
          now = now+1;
          a = a+1;
      }
      a = 0;
      printf("\n");
      while(a < 26)
      {
          now = c[a];
          printf(now);
          a = a+1;
      }
      return ('Z');
  }
  
  void main()
  {
      int res_i;
      char res_c;
      printf("Begin");
      func1();
      printf("\n<< func1 return");
      res_i = func2();
      printf("\n<< func2 return: ", res_i);
      res_c = func3();
      printf("\n<< func3 return: ", res_c);
      func4(res_i);
      printf("\n<< func4");
      res_i = func5(res_c);
      func5(99);
      printf("\n<< func5 return: ", res_i);
      res_c = func6(52,97);
      printf("\n<< func6 return: ", res_c);
      printf("\nEnd\n");
  }
  ```

* 测试点：

  * 全局常量定义与全局变量定义
  * func1：全局常量输出、全局变量赋值输出、局部变量覆盖全局变量输出、无返回值函数测试
  * func2：输入输出与有返回值函数测试
  * func3：运算与有返回值函数测试
  * func4：while与if-else语句以及带参数无返回值函数测试
  * func5：数组赋值与输出测试
  * func6：循环对数组赋值与循环输出测试

* 测试结果：使用MARS仿真，结果与预期一致

  ```
  Begin
  >> func1 test
  g_i1 = 10
  g_c2 = 65
  g_i4 = 100
  g_c3 = H
  g_i2 = 6
  g_c1 = C
  << func1 return
  >> func2 test
  input integer a:730
  confirm a: 730
  input character b:97
  confirm b: a
  << func2 return: 0
  >> func3 testBegin
  >> func1 test
  g_i1 = 10
  g_c2 = 65
  g_i4 = 100
  g_c3 = H
  g_i2 = 6
  g_c1 = C
  << func1 return
  >> func2 test
  input integer a:730
  confirm a: 730
  input character b:97
  confirm b: a
  << func2 return: 0
  >> func3 test
  c = 1
  << func3 return: K
  >> func4 test
  p1 = 0
  p1 = 1
  p1 = 2
  p1 = 3
  << func4
  >> func5 test
  c[1] = g
  >> func5 test
  c[1] = g
  << func5 return: 0
  >> func6 test
  abcdefghijklmnopqrstuvwxyz
  << func6 return: Z
  End
  
  -- program is finished running (dropped off bottom) --
  ```

  注：由于MARS输出框无法最大化，截图有点困难，因此是将I/O输出粘贴至此，绝对真实且可以验证

### 集成测试

* 汉诺塔程序：

  * 源代码：

    ```c++
    void move(int n,char x,char y,char z) {
        if (n==1) {
            printf(x);
            printf("--->",z);
            printf("\n");
        }else {
            move(n-1,x,z,y);
            printf(x);
            printf("--->",z);
            printf("\n");
            move(n-1,y,x,z);
        }
    }
    
    void main()
    {
        int n;
        printf("Input number : ");
        scanf(n);
        move(n,'X','Y','Z');
        return (0);
    }
    ```

  * 编译结果：

    ```assembly
    .data
    $string0:	.asciiz	"--->"
    $string1:	.asciiz	"\n"
    $string2:	.asciiz	"--->"
    $string3:	.asciiz	"\n"
    $string4:	.asciiz	"Input number : "
    .text
    .globl main
    		j	main
    move:
    		subi	$sp	$sp	4
    		move	$t0	$a0
    		sw	$t0	-8($fp)
    		subi	$sp	$sp	4
    		move	$t0	$a1
    		sw	$t0	-12($fp)
    		subi	$sp	$sp	4
    		move	$t0	$a2
    		sw	$t0	-16($fp)
    		subi	$sp	$sp	4
    		move	$t0	$a3
    		sw	$t0	-20($fp)
    		subi	$sp	$sp	4
    		lw	$t0	-8($fp)
    		li	$t1	1
    		bne	$t0	$t1	_LABLE_1
    		lw	$a0	-12($fp)
    		li	$v0	11
    		syscall
    		la	$t0	$string0
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		lw	$a0	-20($fp)
    		li	$v0	11
    		syscall
    		la	$t0	$string1
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		j	_LABLE_2
    _LABLE_1:
    		lw	$t0	-8($fp)
    		li	$t1	1
    		sub	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-24($fp)
    		lw	$a0	-24($fp)
    		lw	$a1	-12($fp)
    		lw	$a2	-20($fp)
    		lw	$a3	-16($fp)
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	move
    		nop
    		lw	$a0	-12($fp)
    		li	$v0	11
    		syscall
    		la	$t0	$string2
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		lw	$a0	-20($fp)
    		li	$v0	11
    		syscall
    		la	$t0	$string3
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		lw	$t0	-8($fp)
    		li	$t1	1
    		sub	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-28($fp)
    		lw	$a0	-28($fp)
    		lw	$a1	-16($fp)
    		lw	$a2	-12($fp)
    		lw	$a3	-20($fp)
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	move
    		nop
    _LABLE_2:
    		move	$t0	$ra
    		lw	$ra	-4($fp)
    		add	$sp	$fp	$zero
    		lw	$fp	($fp)
    		jr	$t0
    main:
    		subi	$sp	$sp	4
    		subi	$sp	$sp	16
    		subi	$sp	$sp	4
    		la	$t0	$string4
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		li	$v0	5
    		syscall
    		sw	$v0	-24($fp)
    		lw	$a0	-24($fp)
    		li	$a1	88
    		li	$a2	89
    		li	$a3	90
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	move
    		nop
    		li	$v0	10
    		syscall
    ```

  * 运行结果（MARS）：

    ```
    Input number : 4
    X--->Y
    X--->Z
    Y--->Z
    X--->Y
    Z--->X
    Z--->Y
    X--->Y
    X--->Z
    Y--->Z
    Y--->X
    Z--->X
    Y--->Z
    X--->Y
    X--->Z
    Y--->Z
    
    -- program is finished running --
    ```

* 快速排序

  * 源程序

    ```c++
    int a[5];
    
    void quicksort(int l,int r) 
    { 
        int i,j;
        int tmp,tt;
        i = l;
        j = r;
        tmp = a[(l+r)/2];
        while(i <= j)
        {  
            while(a[j] > tmp)
            {
                j = j-1;
            }
            while(a[i] < tmp)
            {
                i = i+1;
            }
            if(i<=j)
            {
                tt=a[i];
                a[i]=a[j];
                a[j]=tt;
                i = i+1;
                j = j-1;
            }
        }
        if(i<r)
        {
            quicksort(i,r);
        }
        if(j>l)
        {
            quicksort(l,j);
        }
    }
    
    void main()
    {
    	int i,j;
        a[0] = 8;
        a[1] = 9;
        a[2] = 5;
        a[3] = 7;
        a[4] = 6;
        i = 0;
        printf("Before sort: ");
    	while(i<5)
        {
            j = a[i];
    		printf(" ",j);
            i = i+1;
        }
        printf("\nAfter  sort: ");
    	quicksort(0,4);
        i = 0;
    	while(i<5)
        {
            j = a[i];
    		printf(" ",j);
            i = i+1;
        }
    	printf("\n");
    	return (0);
    }
    ```

  * 编译结果

    ```assembly
    .data
    a:	.space	20
    $string0:	.asciiz	"Before sort: "
    $string1:	.asciiz	" "
    $string2:	.asciiz	"\nAfter  sort: "
    $string3:	.asciiz	" "
    $string4:	.asciiz	"\n"
    .text
    .globl main
    		j	main
    quicksort:
    		subi	$sp	$sp	4
    		move	$t0	$a0
    		sw	$t0	-8($fp)
    		subi	$sp	$sp	4
    		move	$t0	$a1
    		sw	$t0	-12($fp)
    		subi	$sp	$sp	4
    		subi	$sp	$sp	4
    		subi	$sp	$sp	4
    		subi	$sp	$sp	4
    		subi	$sp	$sp	4
    		lw	$t0	-8($fp)
    		sw	$t0	-16($fp)
    		lw	$t0	-12($fp)
    		sw	$t0	-20($fp)
    		lw	$t0	-8($fp)
    		lw	$t1	-12($fp)
    		add	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-32($fp)
    		lw	$t0	-32($fp)
    		li	$t1	2
    		div	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-36($fp)
    		la	$t0	a
    		lw	$t1	-36($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-40($fp)
    		lw	$t0	-40($fp)
    		sw	$t0	-24($fp)
    _LABLE_1:
    		lw	$t0	-16($fp)
    		lw	$t1	-20($fp)
    		bgt	$t0	$t1	_LABLE_2
    _LABLE_3:
    		la	$t0	a
    		lw	$t1	-20($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-44($fp)
    		lw	$t0	-44($fp)
    		lw	$t1	-24($fp)
    		ble	$t0	$t1	_LABLE_4
    		lw	$t0	-20($fp)
    		li	$t1	1
    		sub	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-48($fp)
    		lw	$t0	-48($fp)
    		sw	$t0	-20($fp)
    		j	_LABLE_3
    _LABLE_4:
    _LABLE_5:
    		la	$t0	a
    		lw	$t1	-16($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-52($fp)
    		lw	$t0	-52($fp)
    		lw	$t1	-24($fp)
    		bge	$t0	$t1	_LABLE_6
    		lw	$t0	-16($fp)
    		li	$t1	1
    		add	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-56($fp)
    		lw	$t0	-56($fp)
    		sw	$t0	-16($fp)
    		j	_LABLE_5
    _LABLE_6:
    		lw	$t0	-16($fp)
    		lw	$t1	-20($fp)
    		bgt	$t0	$t1	_LABLE_7
    		la	$t0	a
    		lw	$t1	-16($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-60($fp)
    		lw	$t0	-60($fp)
    		sw	$t0	-28($fp)
    		la	$t0	a
    		lw	$t1	-20($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-64($fp)
    		lw	$t0	-64($fp)
    		lw	$t1	-16($fp)
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		lw	$t0	-28($fp)
    		lw	$t1	-20($fp)
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		lw	$t0	-16($fp)
    		li	$t1	1
    		add	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-68($fp)
    		lw	$t0	-68($fp)
    		sw	$t0	-16($fp)
    		lw	$t0	-20($fp)
    		li	$t1	1
    		sub	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-72($fp)
    		lw	$t0	-72($fp)
    		sw	$t0	-20($fp)
    		j	_LABLE_8
    _LABLE_7:
    _LABLE_8:
    		j	_LABLE_1
    _LABLE_2:
    		lw	$t0	-16($fp)
    		lw	$t1	-12($fp)
    		bge	$t0	$t1	_LABLE_9
    		lw	$a0	-16($fp)
    		lw	$a1	-12($fp)
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	quicksort
    		nop
    		j	_LABLE_10
    _LABLE_9:
    _LABLE_10:
    		lw	$t0	-20($fp)
    		lw	$t1	-8($fp)
    		ble	$t0	$t1	_LABLE_11
    		lw	$a0	-8($fp)
    		lw	$a1	-20($fp)
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	quicksort
    		nop
    		j	_LABLE_12
    _LABLE_11:
    _LABLE_12:
    		move	$t0	$ra
    		lw	$ra	-4($fp)
    		add	$sp	$fp	$zero
    		lw	$fp	($fp)
    		jr	$t0
    main:
    		subi	$sp	$sp	4
    		subi	$sp	$sp	8
    		subi	$sp	$sp	4
    		subi	$sp	$sp	4
    		li	$t0	8
    		li	$t1	0
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		li	$t0	9
    		li	$t1	1
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		li	$t0	5
    		li	$t1	2
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		li	$t0	7
    		li	$t1	3
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		li	$t0	6
    		li	$t1	4
    		mul	$t1	$t1	-4
    		la	$t2	a
    		add	$t1	$t1	$t2
    		sw	$t0	($t1)
    		li	$t0	0
    		sw	$t0	-16($fp)
    		la	$t0	$string0
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    _LABLE_13:
    		lw	$t0	-16($fp)
    		li	$t1	5
    		bge	$t0	$t1	_LABLE_14
    		la	$t0	a
    		lw	$t1	-16($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-24($fp)
    		lw	$t0	-24($fp)
    		sw	$t0	-20($fp)
    		la	$t0	$string1
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		lw	$a0	-20($fp)
    		li	$v0	1
    		syscall
    		lw	$t0	-16($fp)
    		li	$t1	1
    		add	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-28($fp)
    		lw	$t0	-28($fp)
    		sw	$t0	-16($fp)
    		j	_LABLE_13
    _LABLE_14:
    		la	$t0	$string2
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		li	$a0	0
    		li	$a1	4
    		sw	$fp	($sp)
    		add	$fp	$sp	$0
    		subi	$sp	$sp	4
    		sw	$ra	($sp)
    		jal	quicksort
    		nop
    		li	$t0	0
    		sw	$t0	-16($fp)
    _LABLE_15:
    		lw	$t0	-16($fp)
    		li	$t1	5
    		bge	$t0	$t1	_LABLE_16
    		la	$t0	a
    		lw	$t1	-16($fp)
    		mul	$t1	$t1	-4
    		add	$t0	$t0	$t1
    		lw	$t0	($t0)
    		subi	$sp	$sp	4
    		sw	$t0	-32($fp)
    		lw	$t0	-32($fp)
    		sw	$t0	-20($fp)
    		la	$t0	$string3
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		lw	$a0	-20($fp)
    		li	$v0	1
    		syscall
    		lw	$t0	-16($fp)
    		li	$t1	1
    		add	$t0	$t0	$t1
    		subi	$sp	$sp	4
    		sw	$t0	-36($fp)
    		lw	$t0	-36($fp)
    		sw	$t0	-16($fp)
    		j	_LABLE_15
    _LABLE_16:
    		la	$t0	$string4
    		move	$a0	$t0
    		li	$v0	4
    		syscall
    		li	$v0	10
    		syscall
    ```

  * 运行结果（MARS）：

    ```
    
    Before sort:  8 9 5 7 6
    After  sort:  5 6 7 8 9
    
    -- program is finished running --
    ```

## 遇到的问题与解决方案

* 编译缺失依赖文件（环境配置问题）

  * 问题描述

    由于使用的是轻量级开发环境VSCode，在配置好g++与gdb的路径后，启动调试，显示编译缺少依赖文件，无法进行编译

  * 原因分析

    配置文件中的launch.json主要用于启动GDB调试，启动调试之前需先编译，因此在launch.json中有一个preLaunchTask的配置项，对应在task.json中的一个任务，随后转去task.json，发现问题，该任务原始配置如下：

    ```json
    "args": [
            "-g",
            "${file}",
            "-o",
            "${fileDirname}\\${fileBasenameNoExtension}.exe",
          ],
    ```

    但由于本程序include了其他的.cpp文件和.h文件，本来最好是只include头文件以免发生多重定义，但这里各个cpp之间基本没有交互，有交互的cpp之间也是include对方的头文件的，但main.cpp中最初是直接include源文件（cpp文件），因此出现这个问题，由于使用了未include的文件中的函数而编译时并没有包含该cpp文件，所以报错

  * 解决方案

    在配置项里增加缺失的cpp文件即可，这里只提示缺失了optimizer.cpp，因此修改如下：

    ```json
    "args": [
            "-g",
            "${file}",
            "-o",
            "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "${fileDirname}\\optimizer.cpp",
          ],
    ```

    随后编译成功，调试运行正常

* 值传递与引用传递（BUG调试）[生成四元式部分]

  * 问题描述

    在函数中对全局常量进行输出，在语法分析阶段报错，信息为：找不到变量的定义

  * 原因分析

    报错代码段为：

    ```c++
    int tempType = g_symbolTab.searchSymbol(opArg_printfBeta, 1, g);
    cout << "variable " << opArg_printfBeta << " type: " << tempType;
    if(tempType == 0)
    {
        genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "int");
    }
    else if(tempType == 1)
    {
        genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "char");
    }
    else{
        if(g.g_const != 1){
            error("Not find declaration of avarible array", g);
        }
        else{
            genMidcode("prtf", opArg_printfAplha, opArg_printfBeta, "int");
        }
    }
    ```

    该代码段为语法分析中对printf的处理，其中的opArg_printfBeta为我们要输出的变量名字，该问题下为全局常量的名字，而后在符号表中进行查询，返回的是该变量的value，根据我的符号表的定义，全局常量的value为其常量值，若不是常量，则value为0代表int型变量，为1代表char型变量，经过调试发现得到的tempType确实为常量的值，因此进入else语句，再对全局控制变量g中的g_const进行判断，这个g_const为1时代表当前所查询的符号为常量，否则为变量，而g_const本身是属于全局控制变量g的，g在每次调用searchSymbol的时候会传入，查询时会根据符号是否是常量来改变g_const的值，也就是说这个g_const的值的改变是在调用searchSymbol的时候即时改变而生效，正常来说运行到这里g_const应该为1，但是却报错了，说明是g_const出了问题，因此跟进searchSymbol函数中：

    ```c++
    int symbolTable::searchSymbol(string name, int type, global_Var gn){
        if(type == 2){
            //search a name of function
            for(int i = 0; i < i_totalProgram; i++){
                if(vec_symbols[vec_programIndex[i]].s_name == name){
                    if(vec_symbols[vec_programIndex[i]].i_para == gn.g_paranum){
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
            }
            return -1;
        }
        else{
            //search a name of variable
            for(int i = vec_programIndex[i_totalProgram-1]; i < i_topIndex; i++){
                if(vec_symbols[i].s_name == name){
                    return vec_symbols[i].i_value;
                }
            }
            for(int i = 0; i < vec_programIndex[0]; i++){
                if(vec_symbols[i].s_name == name){
                    if(vec_symbols[i].i_type == 0){
                        gn.g_const = 1;
                    }
                    else{
                        gn.g_const = 0;
                    }
                    return vec_symbols[i].i_value;
                }
            }
            return -2;
        }
    }
    ```

    由于不是查找函数名，因此直接进入else分支，经过调试发现gn.g_const = 1;这句代码居然被执行了，此时一下明白了是传参的问题，果然一看参数是值传递，因此这里的gn只是对全局控制变量g的一个拷贝，改变这个gn的值对g没有任何影响，所以返回后的g_const并不是1

  * 解决方案

    改值传递为引用传递即可

    ```c++
    int symbolTable::searchSymbol(string name, int type, global_Var& gn)
    ```

* 语句顺序（BUG调试）[语法分析部分]

  * 问题描述

    定义了两个分别带一个char类型的参数的函数，发现如果给的参数的名字一样的话在语法分析时报出“多重定义”的错误

  * 原因分析

    由变量的作用域可知，这两个同名的函数参数是没有道理触发多重定义的，那么最开始我以为是符号表部分变量的插入出了问题，调试了半天，最后发现这部分的逻辑代码是没有问题的，代码如下：

    ```c++
    void symbolTable::insert_symbol(string name, int type, int value, int address, int para, global_Var& gn){
        if(type == 2){
            //insert a name of function
            for(int i = 0; i < i_totalProgram; i++){
                if(vec_symbols[vec_programIndex[i]].s_name == name){
                    error("Multiply defination of function", gn);
                    return;
                }
            }
            vec_programIndex.push_back(i_topIndex);
            i_totalProgram++;
            gn.g_paranum = 0;
        }
        else{
            //insert a name of variable
            if(i_totalProgram != 0){
                for(int i = vec_programIndex[i_totalProgram-1]; i < i_topIndex; i++){
                    if(vec_symbols[i].s_name == name){
                        error("Multiply defination of variable", gn);
                        return;
                    }
                }
            }
        }
    }
    // insert operation
    ```

    如上述代码可见，报出的错误位置是在第二个error函数处，但是根据我的符号表设计，首先语法分析到这是一个函数定义，那么就会先生成函数定义对应的四元式，这时候肯定会插入函数名，即调用这个函数其中type=2，因此当函数定义四元式生成后，i_totalprogram已经++了，这时候就已经出现了一个新的空间，即刚插入的那个函数的空间，此时这个空间是空的，没有任何变量存在，那么接下来语法分析就到了参数部分，在插入函数参数到符号表时，也会调用本函数进行插入，那么就说明这个变量在查找时对应的查找空间找到上一个函数的空间去了，因此我又仔细调试了一波，发现确实找的时候居然是上一个函数的空间，随后又赶紧检查了一下符号表的设计，却没有发现任何疑点，那么就只剩下一个可能了，在函数参数插入到符号表时，其对应的函数名压根就没插入符号表，导致新的函数空间没有生成，再查的时候就查到了上一个函数的空间里，导致了重名，一番调试后发现还真是函数名没有插入成功，最后一路回溯，到递归下降子程序对应位置，终于找到了原因：

    ```c++
    genMidcode("func", "char", "", opArg_funcName);
    arg_List();
    g_symbolTab.insert_symbol(opArg_funcName, 2, 2, g.g_address, g.g_paranum, g);
    ```

    和这个递归下降子函数中的int类型返回值分支一比较，发现arg_List()的调用是在将函数名插入符号表生成其对应的函数空间之后的，可能是自己手误按到了Alt+down导致两句话顺序换了一下，结果就导致函数名在参数插入后再插入符号表中，但上面的int类型返回值函数的部分的顺序却是对的，调试了好久一直以为是符号表的问题，最后发现是一个小失误

  * 解决方案

    将顺序改为正确的顺序即可

    ```c++
    genMidcode("func", "char", "", opArg_funcName);
    g_symbolTab.insert_symbol(opArg_funcName, 2, 2, g.g_address, g.g_paranum, g);
    arg_List();
    ```

## 4、附录

### 程序执行过程

* 读入源程序 &rightarrow; 词法分析&rightarrow; 语法分析 &rightarrow; 生成四元式 &rightarrow; 常量传播优化四元式 &rightarrow; 翻译 &rightarrow; 汇编语句

### 运行示例（带过程）

* 源程序（为了覆盖所有点但又不至于太复杂）

  ```c++
  const int g = 5;
  void main()
  {
      int a;
      a = 1*(2+8/4)-g;
      if(a < 0)
      {
          printf(a);
      }
      return (0);
  }
  ```

* 我的编译器：启动！

  * Step1：词法分析

    结果如下：

    ![1561970090898](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970090898.png)

  * Step2：语法分析

    结果如下：

    ![1561970105021](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970105021.png)

    符号表输出：

    ![1561969924856](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561969924856.png)

  * Step3：生成四元式

    结果如下：

    ![1561970136707](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970136707.png)

  * Step4：常量传播优化

    结果如下：

    ![1561970148819](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970148819.png)

    数据流块：

    ![1561970195489](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970195489.png)

  * Step5：汇编

    结果如下：

    ![1561970267588](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970267588.png)

  * Step6：MARS仿真

    结果如下：

    ![1561970356529](C:\Users\HCY\AppData\Roaming\Typora\typora-user-images\1561970356529.png)

## 5、结束语

通过本次编译原理试点班，我对编译器的认识从只知道在IDE里写好代码后按F5就完事儿到现在对编译器的整个工作流程有了一个清晰的认识，也掌握并实现了很多的方法，如递归下降子程序、数据流分析优化、符号表等，在开发整个项目的过程中，遇到了很多的问题、Bug，也有过苦恼、难受，但依然还是没有放弃，一步一步从词法分析开始到最后的优化，从一开始很困惑递归下降子程序到底最后的目的是要干什么到完成最初的那个表达式翻译的小任务，很多时候在一瞬间想明白了某一步甚至某一个模块要怎么运作，怎么配合其他模块，一下就豁然开朗了，但这并不代表这我就完全掌握了编译原理的所有内容，我曾经思考过一个问题，现在我写的这个编译器是使用的C++语言，要运行我这个编译器就得先用G++进行编译，但是G++本身也是一个编译器，经过查找资料，发现G++、GCC这些编译器也是用C语言写的，那么这些编译器的运行又是谁来负责编译呢，这仿佛陷入了一个死循环，最后经过一番google，原来第一个编译器是用汇编写出伪代码后再对照表转换为二进制指令来实现的，可见我们的工作都是站在巨人的肩膀上的，除此之外，G++比我的编译器复杂了不知道多少个数量级，还有很多地方值得我学习，本次项目的整个开发过程都使用了Github管理，目前已经开源

Github地址：<https://github.com/Hcyang-NULL/Compiler>

