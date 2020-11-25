//
// Created by Memo on 2018/11/24.
//

#ifndef LL_1_LL_1_GRAMMER_H
#define LL_1_LL_1_GRAMMER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iomanip>
#include <stack>

using namespace std;

typedef struct DEDUCTION
{
    string left;
    string right;
    bool is_left_recursion = false;
} DEDUCTION;


class LL_1_grammer
{

    //����ʽ
    vector<DEDUCTION> deduction;
    //�ս���������ս����
    vector<string> nonterminal, terminal;
    //first����follow��
    map<string, vector<string>> first, follow;
    //��ʼ��
    string start;
    //����Ԥ�����������
    string x;
    //����Ԥ�������
    string table[7][10];

public:
    //����ֱ����ݹ�
    void remove_direct_left_recursion();
    //��ʼ��
    void init();
    //�������ֱ����ݹ��Ĳ���ʽ�����ս�������ս������first����follow��
    void print_test();
    //���ڱ������ʽ
    void ll_pushback(DEDUCTION mid) { deduction.push_back(mid); }
    //���ɸ����ս����first��
    void first_set();
    //���ɸ����ս����follow��
    void follow_set();
    //��ȡfirst��
    vector<string> get_first_set(string to_get_first);
    //��ȡfollow��
    vector<string> get_follow_set(string to_get_follow);
    //����Ԥ�������
    void analysis_table();
    //Ԥ���������
    void analysis_program(string text);

    int number_non(string s);

    int number_ter(string s);
    //��������
    string number_process(string input);
};

#endif //LL_1_LL_1_GRAMMER_H
