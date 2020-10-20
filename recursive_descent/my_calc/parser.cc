#include "parser.h"

/*
exp = term exp'
exp' = + term exp' | - term exp' | null
term = factor term'
term' = * factor term' | null
factor = (exp) | num | -factor
*/


/*
<factor> ::= ( <expr> )
           | Num
           | -<factor>
*/
bool parse_factor(list<Token> & exp, float & result)
{
	if (exp.empty())
	{
		return false;
	}
	else if (exp.front().tokenType == TokenType::LPAREN)
	{
		exp.pop_front();
		if (!parse_exp(exp, result))
		{
			return false;
		}
		else
		{
			if (!exp.empty() && exp.front().tokenType == TokenType::RPAREN)
			{
				exp.pop_front();
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else if (exp.front().tokenType == TokenType::NUM)
	{
		result = exp.front().value.num;
		exp.pop_front();
		return true;
	}
	else if (exp.front().tokenType == TokenType::SUB){
		exp.pop_front();
		float other;
		if(!parse_factor(exp, other)){
			return false;
		}
		result = -other;
		return true;
	}
	else
	{
		return false;
	}
}

/*
<term> ::= <factor> <term_tail>
<term_tail> ::= * <factor> <term_tail>
              | / <factor> <term_tail>
              | <empty>
*/
bool parse_term(list<Token> & exp, float & result)
{
	if (!parse_factor(exp, result))
	{
		return false;
	}
	while (!exp.empty() && (exp.front().tokenType == TokenType::MUL || exp.front().tokenType == TokenType::DIV))
	{
		TokenType tt = exp.front().tokenType;
		exp.pop_front();
		float other;
		if (!parse_factor(exp, other))
		{
			return false;
		}
		if (tt == TokenType::MUL)
		{
			result *= other;
		}
		else
		{
			result /= other;
		}
	}
	return true;
}

/*
<expr> ::= <term> <expr_tail>
<expr_tail> ::= + <term> <expr_tail>
              | - <term> <expr_tail>
              | <empty>
*/
bool parse_exp(list<Token> & exp, float & result)
{
	if (!parse_term(exp, result))
	{
		return false;
	}
	while (!exp.empty() && (exp.front().tokenType == TokenType::ADD || exp.front().tokenType == TokenType::SUB))
	{
		TokenType tt = exp.front().tokenType;
		exp.pop_front();
		float other;
		if (!parse_term(exp, other))
		{
			return false;
		}
		if (tt == TokenType::ADD)
		{
			result += other;
		}
		else
		{
			result -= other;
		}
	}
	return true;
}