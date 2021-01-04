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

//产生式
typedef struct Production {
    Symbol* left;
    vector<Symbol*> right;
}Production;

//项目
typedef struct LR_Item {
    Production* prod;
    int dot;
    vector<Symbol*> forwards;
} LR_Item;

//项目集
typedef struct LR_State{
    int index;
    vector<LR_Item*> items;
    //跳转表
    map<Symbol*, int> goto_map;
}LR_State;


class LALR1{
    //产生式
    vector<Production*> productions;
    Production* start;
    //符号集合
    vector<Symbol*> symbols;
    vector<LR_State*> states;

    //终结符集、非终结符集
    //set<string> nonterminal, terminal;
    //nullable集合
    //set<string> nullable;
    //first集、follow集
    //map<string, set<string>> first;
    //起始符
    //string start;
    //用于预测分析程序中
    //string x;
    //预测分析表
    //int** table;

public:
    //解析bnf文件
    void Init(const string filename);
    //保存产生式
    void lalr1_pushback(Production* mid) { productions.push_back(mid); }
    Symbol* NewSymbol(const string name, SYMBOL_TYPE type);
    Symbol* NewSymbol(const string name); 
    Symbol* FindSymbol(const string name);
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
    //测试输出
    void PrintTest();
};

#endif //LL_1_LL_1_GRAMMER_H
