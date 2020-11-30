calc: y.tab.c lex.yy.c
	gcc -o $@ $^

y.tab.h y.tab.c y.output: calc.y
	bison -vdty $<

lex.yy.c: calc.l
	flex $<