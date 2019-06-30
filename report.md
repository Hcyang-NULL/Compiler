# 编译原理报告

## 要求

|               项目               | 完成 |
| :------------------------------: | :--: |
|             文法定义             |  OK  |
|           典型语句案例           |      |
|             文法说明             |  OK  |
|           系统总体设计           |  OK  |
|             详细设计             |  OK  |
| 测试用例及测试结果（截图、说明） |      |
|    实验过程中解决的问题和说明    |      |
|             其他附录             |      |



## 总体设计

* 本编译器使用C++开发，主要工作分为六个部分：词法分析、语法分析、四元式生成、语法制导翻译、优化、主函数。

模块|概述|代码行数
:-:|:-:|:-:
词法分析|根据有限状态自动机来完成对单词的识别|485
语法分析|使用C0文法使用递归下降子程序|1212
四元式生成|根据递归下降子程序生成对应的四元式|45
语法制导翻译|根据四元式与符号表完成翻译|1031
优化|基于数据流分析框架进行优化|929
主函数|对各个子模块进行调用完成编译|117
总计||3819

注：四元式生成模块的部分代码在语法分析中，所有统计的代码行不包含空行

* 开发环境

|  配置项  |   环境    |
| :------: | :-------: |
|   IDE    |  VSCode   |
|    OS    | Windows10 |
| Compiler |    G++    |
|  Debug   |    GDB    |

## 详细设计

### 1、词法分析

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

### 2、语法分析

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

### 3、四元式生成

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

### 4、语法制导翻译

* 根据生成的四元式依次按顺序进行翻译，同时使用符号表查找偏移与变量

* 样例：

  * 四元式：<+, 1, a, $hcy_1>

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

### 5、优化

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

## 测试

### 单元测试

