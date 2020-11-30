### ll1_calc
一个用ll1分析实现的计算器，词法和语法分析耦合，无法扩展

### ll1_mycalc
在ll1_calc上改进，加入了词法分析以及自定义语法
注意为了解析起来方便，语法格式在标准bnf格式上做了一些改造：
* 将｜关系改成每一行
* 终结符为<>括起来的符号
* 各个符号之间用空格间隔
* 第一个非终结符为开始符号

### tial
用python实现的词法分析，ll1分析并能生产ir


### TODO
1. 改写成bison的语法
```
%{
#include <stdio.h>
void yyerror(const char* msg) {}
int yylex();
#define YYSTYPE char *
%}

%token T_WORD
%token T_PUNC

%%

SENT   : SENT M_WORD T_PUNC '\n'            {printf("This is a sentence.\n");}
       |        
       ;

M_WORD : T_WORD
       | M_WORD T_WORD             
       ;
%%

int main() {
    return yyparse();
}
```

2. 优化性能，lex和语法分析器token映射

3. lexer和parser解耦