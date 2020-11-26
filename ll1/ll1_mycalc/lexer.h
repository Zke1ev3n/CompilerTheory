#pragma once
#include <list>
#include <string>
using std::list;
using std::string;

enum class TokenType
{
	NUM,
	ADD,
	SUB,
	MUL,
	DIV,
	LPAREN,
	RPAREN
};

union TokenValue
{
	float num;
	char symbol;
};

struct Token
{
	TokenType tokenType;
	TokenValue value;
};

bool tokenize(const string &, list<Token> &);