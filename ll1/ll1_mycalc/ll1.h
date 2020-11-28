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
    //����ʽ
    vector<DEDUCTION> deduction;
    //�ս���������ս����
    set<string> nonterminal, terminal;
    //first����follow��
    map<string, set<string>> first, follow;
    //��ʼ��
    string start;
    //����Ԥ�����������
    string x;

public:
    //��ʼ��
    void init(const string filename);
    //����bnf�ļ�
    void parse_bnf(const string filename);
    //����ֱ����ݹ�
    void remove_direct_left_recursion();
    //�������ֱ����ݹ��Ĳ���ʽ�����ս�������ս������first����follow��
    void print_test();
    //���ڱ������ʽ
    void ll_pushback(DEDUCTION mid) { deduction.push_back(mid); }
    //���ɸ����ս����first��
    void first_set();
    //���ɸ����ս����follow��
    void follow_set();
    //��ȡfirst��
    void get_first_set(string to_get_first);
    //��ȡfollow��
    void get_follow_set(string to_get_follow);
    //����Ԥ�������
    void print_table();
    //Ԥ���������
    void analysis_program(string text);
};

#endif //LL_1_LL_1_GRAMMER_H
