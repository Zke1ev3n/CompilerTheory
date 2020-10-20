### 一个运算表达式实现
这里我参考了python的语法：
[https://github.com/antlr/grammars-v4/blob/790d9882e90f189821ff6e302768678a272c4a8d/python3/Python3.g4#L217](https://github.com/antlr/grammars-v4/blob/790d9882e90f189821ff6e302768678a272c4a8d/python3/Python3.g4#L217)
```
<expr> ::= <expr> + <term>
         | <expr> - <term>
         | <term>

<term> ::= <term> * <factor>
         | <term> / <factor>
         | <factor>

<factor> ::= ( <expr> )
           | num
           | -<factor>
```
当然上面的范式存在左递归，在实现的时候我们需要消除左递归。
```
<expr> ::= <term> <expr_tail>
<expr_tail> ::= + <term> <expr_tail>
              | - <term> <expr_tail>
              | <empty>

<term> ::= <factor> <term_tail>
<term_tail> ::= * <factor> <term_tail>
              | / <factor> <term_tail>
              | <empty>

<factor> ::= ( <expr> )
           | Num
           | -<factor>
```

### TODO

1. 支持更多运算符，支持变量
```
expression
    : term
    | expression '+' term
    | expression '-' term
    ;

term
    : factor
    | term '*' factor
    | term '/' factor
    | term '%' factor
    ;

factor
    : primary
    | '-' factor
    | '+' factor
    ;

primary
    : IDENTIFIER
    | INTEGER
    | FLOATING_POINT_LITERAL
    | '(' expression ')'
    ;
```

2. 解析成AST。

3. 优化运算符语法，使用优先级

### 参考资料
[http://homepage.divms.uiowa.edu/~jones/compiler/spring13/notes/10.shtml](http://homepage.divms.uiowa.edu/~jones/compiler/spring13/notes/10.shtml)