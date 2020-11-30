#include "lexer.h"
#include <sstream>

bool tokenize(const string & expression, vector<Token> & tokens)
{
	const char * current = expression.c_str();
	const char * const end = current + expression.length();
	while (current != end)
	{
		Token token;
		bool tryNumber = false;
		switch (*current)
		{
		case '+':
			token.tokenType = token_symbols[1];
			break;
		case '-':
			token.tokenType = token_symbols[2];
			break;
		case '*':
			token.tokenType = token_symbols[3];
			break;
		case '/':
			token.tokenType = token_symbols[4];
			break;
		case '(':
			token.tokenType = token_symbols[5];
			break;
		case ')':
			token.tokenType = token_symbols[6];
			break;
		default:
			tryNumber = true;
		}
		if (!tryNumber)
		{
			token.value.symbol = *current;
		    current++;
		}
		else
		{
			std::stringstream converter;
			token.tokenType = token_symbols[0];
			converter << current;
			converter >> token.value.num;
			if (converter.fail())
			{
				return false;
			}
			string left;
			converter >> left;
			current += end - current - left.length();
		}
		
		tokens.push_back(token);
	}
	Token final_token;
	final_token.tokenType = "$";
	tokens.push_back(final_token);
	return true;
}