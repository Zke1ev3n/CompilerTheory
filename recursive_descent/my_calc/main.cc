#include <iostream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

bool calclute(const string & exp, float & result)
{
	list<Token> tokens;
	if (tokenize(exp, tokens))
	{
		if(parse_exp(tokens, result) && tokens.empty())
		{
			return true;
		}
		else
		{
			cout << "Parse error.\n";
			return false;
		}
	}
	else
	{
		cout << "Tokenization error.\n";
		return false;
	}
}

int main()
{
	string expression;
	float result;
	cout << "(Input empty string to exit.)\n";
	while (true)
	{
		getline(cin, expression);
		if (expression.empty())
		{
			return 0;
		}
		else if(calclute(expression, result))
		{
			cout << result << endl;
		}
	}
}