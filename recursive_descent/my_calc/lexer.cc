#include "lexer.h"
#include <sstream>

bool tokenize(const string & expression, list<Token> & tokens)
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
			token.tokenType = TokenType::ADD;
			break;
		case '-':
			token.tokenType = TokenType::SUB;
			break;
		case '*':
			token.tokenType = TokenType::MUL;
			break;
		case '/':
			token.tokenType = TokenType::DIV;
			break;
		case '(':
			token.tokenType = TokenType::LPAREN;
			break;
		case ')':
			token.tokenType = TokenType::RPAREN;
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
			token.tokenType = TokenType::NUM;
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
	return true;
}