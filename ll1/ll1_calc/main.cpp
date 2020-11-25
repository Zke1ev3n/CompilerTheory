#include <iostream>
#include <fstream>
#include "LL_1_grammer.h"

using namespace std;

int main()
{
    ifstream fin;
    char C;
    string filename;
    cout << "please input file:" << endl;
    cin >> filename;
    fin.open(filename);

    LL_1_grammer ll;
    ll.init();
    fin.get(C);
    DEDUCTION mid;
    while (!fin.eof()) {//将产生式存入
        mid.left = C;
        fin.get(C);
        while (C != '>')
            fin.get(C);//跳过箭头
        fin.get(C);
        while (C != '\n' && !fin.eof()) {
            if (C == ' ') {}
            else if (C == '|') {
                ll.ll_pushback(mid);
                mid.right.clear();
            } else {
                mid.right.push_back(C);
            }
            fin.get(C);
        }
        ll.ll_pushback(mid);
        mid.right.clear();
        fin.get(C);
    }
    ll.remove_direct_left_recursion();
    ll.first_set();
    ll.follow_set();
    ll.print_test();
    ll.analysis_table();
    string text;
    cout << "please input string: " << endl;
    cin >> text;
    text = ll.number_process(text);
    cout << text << endl;
    ll.analysis_program(text);

    return 0;
}
