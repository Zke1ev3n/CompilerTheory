#ifndef LALR1_H 
#define LALR1_H 

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iomanip>
#include <stack>
#include <set>

using namespace std;

enum SYMBOL_TYPE
{
	ST_NONE = 0,
	ST_TERMINAL,
	ST_NONTERMINAL,
	ST_ACCEPT,
};

//符号
typedef struct Symbol {
    int index;
    SYMBOL_TYPE type;
    int nullable;
    //vector<Symbol*> firstset;
    vector<Symbol*> firstset;
    string name;
} Symbol;

enum ASSOCIATIVITY_TYPE
{
	AT_NONE = 0,
	AT_LEFT,
	AT_RIGHT
};

typedef struct AssocPrec
{
	int associativity;
	vector<string> symbol_names;
} AssocPrec;

//产生式
typedef struct Production {
    int index;
    Symbol* left;
    vector<Symbol*> right;
}Production;

//项目
typedef struct LR_Item {
    Production* prod;
    int dot;
    vector<Symbol*> forwards;
} LR_Item;

enum LR_ACTION
{
	LR_ACTION_GOTO = 1,
	LR_ACTION_SHIFT,
	LR_ACTION_REDUCE,
	LR_ACTION_ACCEPT,
	LR_ACTION_ERROR,
	LR_ACTION_SSCONFLICT,
	LR_ACTION_SRCONFLICT,
	LR_ACTION_RRCONFLICT,
	LR_ACTION_RDRESOLVED,
	LR_ACTION_SHRESOLVED,
};

typedef struct Action{
	int type;
	int next_state;

	Production* prod;//规约动作的产生式

	Symbol* lookahead;
}Action;

//项目集
typedef struct LR_State{
    int index;
    vector<LR_Item*> items;
    //跳转表
    map<Symbol*, int> goto_map;
    //TODO 优化？
    vector<vector<Action>> actions;
}LR_State;


class LALR1{
    //产生式
    vector<Production*> productions;
    Production* start;
    //符号集合
    vector<Symbol*> symbols;
    vector<LR_State*> states;

    //符号的优先级
    vector<AssocPrec> assoc_precs;

public:
    //解析bnf文件
    void Init(const string filename);
    //保存产生式
    void lalr1_pushback(Production* mid) { productions.push_back(mid); }
    Symbol* NewSymbol(const string name, SYMBOL_TYPE type);
    Symbol* NewSymbol(const string name); 
    Symbol* FindSymbol(const string name);
    void FindSymbolPrecedence();
    //求first集合
    void FindFirstSet();
    //计算项目集
    void FindStates();
    //计算闭包
    void Closure(LR_State& lr_state);
    int AddItem(LR_State& state, LR_Item& item);
    bool IsSameItem(LR_Item& item1, LR_Item& item2);
    int AddLRState(LR_State& state);
    bool IsMergeLRState(LR_State& s1, LR_State& s2);
    bool IsSameLRState(LR_State& s1, LR_State& s2);
    int MergeLRState(LR_State& s1, LR_State& s2);
    LR_State LRGoto(LR_State& state, Symbol* sym);
    int AddLRGoto(LR_State& state, Symbol* sym, int index);
    void MakeLALR1Table();
    int ResolveConflict(Action& a1, Action& a2);
    //测试输出
    void PrintTest();
};

#endif //LL_1_LL_1_GRAMMER_H
