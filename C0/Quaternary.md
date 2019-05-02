# 四元式设计

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
非主函数带参返回|return (expr)|&lt;ret, , , exist_Var&gt;
非主函数无参返回|return;|&lt;ret, , , &gt;
主函数返回|return|&lt;exit, , , &gt;
有返回函数定义|int handy()|&lt;func, int, , handy&gt;
有返回函数定义|char handy()|&lt;func, char, , handy&gt;
无返回函数定义|void handy()|&lt;func, void, , handy&gt;
函数结束|void handy(){}|&lt;end, , , handy&gt;
if语句|if(condition){sentence}|&lt;较多略&gt;
while语句|while(condition){sentence}|&lt;较多略&gt;

## 符号说明：

1. Var表示生成的变量（临时）
2. exist_Var表示这个中间变量已经生成了

## 语法支持

暂不支持：

1. switch语句
2. 循环语句只支持while
3. int，char之外的类型
4. &，|，^，%，!操作
5. 函数提前声明

特性：

1. 程序结构顺序：常量定义$\rightarrow$变量定义$\rightarrow$非主函数$\rightarrow$主函数