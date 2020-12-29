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

//产生式结构体
typedef struct Production 
{
    string left;
    vector<string> right;
} Production;


class LALR1
{
    //产生式
    vector<Production> productions;
    //终结符集、非终结符集
    set<string> nonterminal, terminal;
    //nullable集合
    set<string> nullable;
    //first集、follow集
    map<string, set<string>> first;
    //起始符
    string start;
    //用于预测分析程序中
    string x;
    //预测分析表
    int** table;

public:
    //初始化
    void init(const string filename);
    //解析bnf文件
    void parse_bnf(const string filename);
    //保存产生式
    void lalr1_pushback(Production mid) { productions.push_back(mid); }
    //生成各非终结符的first集
    void first_set();
    //获取first集
    void get_first_set(string to_get_first);
    void find_first_sets();
    //测试输出
    void print_test();
};

#endif //LL_1_LL_1_GRAMMER_H
