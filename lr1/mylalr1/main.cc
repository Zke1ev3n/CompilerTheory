#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "lalr1.h"
#include "parser.h"

using namespace std;

//对比：http://jsmachines.sourceforge.net/machines/lalr1.html
//对比：https://mikedevice.github.io/first-follow/
int main(int argc, char* argv[])
{
    ifstream input;
	float result;
    LALR1* lalr1 = new LALR1();
    string expression;
    vector<Token> tokens;
	
    if(argc < 2) {
        cout << "please input grammer file" << endl;
        return -1;
    }
    string filename = argv[1];
    lalr1->Init(filename);
    lalr1->FindFirstSet();
    lalr1->FindStates();
    lalr1->MakeLALR1Table();
    lalr1->MakeOutputTable();
    //lalr1->PrintTest();

    Parser* parser = new Parser(lalr1);

    //开始校验
	cout << "(Input empty string to exit.)\n";
	while (true)
	{
        tokens.clear();
		getline(cin, expression);
		if (expression.empty())
		{
			return 0;
		}else if (tokenize(expression, tokens)){
            parser->Parse(tokens);
        }
	}

}