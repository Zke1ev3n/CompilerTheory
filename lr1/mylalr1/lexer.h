#pragma once
#include <vector>
#include <string>
using std::vector;
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

const string token_symbols[] = {
    "num",
    "+",
    "-",
	"*",
	"/",
	"(",
	")"
};

//如果是数字，则记录值，如果是变量，则记录变量名
union TokenValue
{
	float num;
	char symbol;
};

struct Token
{
	string tokenType;
	TokenValue value;
};

bool tokenize(const string &, vector<Token> &);