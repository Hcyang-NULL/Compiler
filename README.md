# 编译原理试点班项目

## 文件说明

项目|说明
:-:|:-:
Final-Vsersion|最终编译器的所有源代码及相关材料文件
First-Mission|第一次小作业的源码与报告
mips.txt|编译后生成的汇编指令文件
report.md|最终报告

## 运行说明

* 主函数位于./Final-Version/compiler.cpp中
* 默认读取进行编译的程序为：./Final-Version/test.c，也可在命令行中给出路径
* 我的配置为./.vscode，若使用我的配置，请更改其中g++的路径即可
* 若您手动进行编译运行，那么请注意，需要进行编译的cpp有两个：compiler.cpp与optimizer.cpp，否则会报依赖错误
* 运行本编译器得到的mips.txt用MARS进行仿真即可
* 若结果出现问题或报错，90%可能是您给出的源程序中的文法与我定义的文法不匹配（测试过很多程序均正常）
