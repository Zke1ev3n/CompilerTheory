#include <stack>
#include <vector>
#include "lexer.h"
#include "lalr1.h"

using namespace std;

// enum LR_ACTION
// {
// 	LR_ACTION_GOTO = 1,
// 	LR_ACTION_SHIFT,
// 	LR_ACTION_REDUCE,
// 	LR_ACTION_ACCEPT,
// 	LR_ACTION_ERROR,
// };

struct action
{
	int type;//action type from LR_ACTION
	int target;//goto target state or reduce rule index
};

typedef struct StackState 
{
	int state;//current state
	string symbol;//current symbol
	TokenType t;
}StackState;


class Parser {

private:
    //
    LALR1* lalr1;

public:
    Parser(LALR1* lalr1) {this->lalr1 = lalr1;};
    void Parse(vector<Token>);
};