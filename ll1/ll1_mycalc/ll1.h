//
// Created by Memo on 2018/11/24.
//

#ifndef LL1_H 
#define LL1_H 

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

typedef struct DEDUCTION
{
    string left;
    vector<string> right;
    bool is_left_recursion = false;
} DEDUCTION;


class LL1
{
    //产生式
    vector<DEDUCTION> deduction;
    //终结符集、非终结符集
    set<string> nonterminal, terminal;
    //first集、follow集
    map<string, set<string>> first, follow;
    //起始符
    string start;
    //用于预测分析程序中
    string x;

public:
    //初始化
    void init(const string filename);
    //解析bnf文件
    void parse_bnf(const string filename);
    //消除直接左递归
    void remove_direct_left_recursion();
    //输出消除直接左递归后的产生式、非终结符集、终结符集、first集、follow集
    void print_test();
    //用于保存产生式
    void ll_pushback(DEDUCTION mid) { deduction.push_back(mid); }
    //生成各非终结符的first集
    void first_set();
    //生成各非终结符的follow集
    void follow_set();
    //获取first集
    void get_first_set(string to_get_first);
    //获取follow集
    void get_follow_set(string to_get_follow);
    //生成预测分析表
    void print_table();
    //预测分析程序
    void analysis_program(string text);
};

#endif //LL_1_LL_1_GRAMMER_H
