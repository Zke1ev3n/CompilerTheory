//
// Created by Memo on 2018/11/24.
//

#include "LL_1_grammer.h"

//bool LL_1_grammer::compare(DEDUCTION x, DEDUCTION y)
//{
//    return x.left < y.left ;
//}

void LL_1_grammer::remove_direct_left_recursion()
{
    for (int i = 0; i < deduction.size(); i++) {
        if (deduction[i].left == deduction[i].right.substr(0, deduction[i].left.size()))//如果有左递归
        {
            deduction[i].is_left_recursion = true;
            string temp = deduction[i].left;
            for (int j = 0; j < deduction.size(); j++)//将该非终结符左递归式全部标记
            {
                if (temp == deduction[j].left &&
                    deduction[j].left == deduction[j].right.substr(0, deduction[i].left.size()) && i != j)
                    deduction[j].is_left_recursion = true;
            }
            for (int j = 0; j < deduction.size(); j++)//消除左递归
            {
                if (deduction[j].left == temp && !deduction[j].is_left_recursion)
                    deduction[j].right.append(temp + "'");
                if (deduction[j].left == temp && deduction[j].is_left_recursion) {
                    deduction[j].left = temp + "'";
                    deduction[j].right.erase(0, temp.size());
                    deduction[j].right.append(temp + "'");
                    deduction[j].is_left_recursion = false;
                }
            }
            DEDUCTION mid;
            mid.left = temp + "'";
            mid.right = "~";
            deduction.push_back(mid);
            nonterminal.emplace_back(temp + "'");
            if (find(terminal.begin(), terminal.end(), "~") == terminal.end())
                terminal.emplace_back("~");
        }
    }
}

void LL_1_grammer::print_test()
{
    cout << "---------------------------" << endl;
    cout << "expresion:" << endl;
    for (auto i: deduction)
        cout << i.left << "->" << i.right << endl;

    cout << "---------------------------" << endl;
    cout << "nonterminal set:" << endl;
    for (auto i: nonterminal)
        cout << i << endl;

    cout << "---------------------------" << endl;
    cout << "terminal set:" << endl;
    for (auto i: terminal)
        cout << i << endl;

    cout << "---------------------------" << endl;
    cout << "first set:" << endl;
    for (auto i:first) {
        cout << i.first << " : ";
        for (int j = 0; j < i.second.size(); j++)
            if (j == i.second.size() - 1)
                cout << i.second[j];
            else
                cout << i.second[j] << " , ";
        cout << endl;
    }

    cout << "---------------------------" << endl;
    cout << "follow set:" << endl;
    for (auto i:follow) {
        cout << i.first << " : ";
        for (int j = 0; j < i.second.size(); j++)
            if (j == i.second.size() - 1)
                cout << i.second[j];
            else
                cout << i.second[j] << " , ";
        cout << endl;
    }

}

void LL_1_grammer::init()
{
    start = "E";
    nonterminal.emplace_back("E");
    nonterminal.emplace_back("T");
    nonterminal.emplace_back("F");
    terminal.emplace_back("+");
    terminal.emplace_back("-");
    terminal.emplace_back("*");
    terminal.emplace_back("/");
    terminal.emplace_back("(");
    terminal.emplace_back(")");
    terminal.emplace_back("num");//代表数字
}


vector<string> LL_1_grammer::get_first_set(string to_get_first)//构建first集
{
    vector<string> mid;
    for (auto i:deduction) {
        if (i.left == to_get_first) {
            auto iter = first.find(to_get_first);
            if (iter != first.end())
                mid = first[to_get_first];
            string head = i.right.substr(0, 1);
            for (auto nt:nonterminal) {
                if (head[0] == nt[0]) {
                    return get_first_set(head);
                }
            }
            for (auto t:terminal) {
                if (head[0] == t[0]) {
                    if (find(mid.begin(), mid.end(), t) == mid.end())//若没有该终结符，则加入first集
                        mid.push_back(t);
                    break;
                }
            }
        }
    }
    return mid;
}

void LL_1_grammer::first_set()
{
    for (auto i:nonterminal)
        first[i] = get_first_set(i);
}

void LL_1_grammer::follow_set()
{
    for (auto i:nonterminal)
        follow[i] = get_follow_set(i);
}

vector<string> LL_1_grammer::get_follow_set(string to_get_follow)//构建follow集
{
    if (!follow[to_get_follow].empty())
        return follow[to_get_follow];
    vector<string> mid;
    if (to_get_follow == "E")
        mid.emplace_back("$");
    for (auto i:deduction) {
        int cur_ch = i.right.find(to_get_follow);
        int length = to_get_follow.size();
        if (cur_ch < 0 || (length == 1 && i.right[cur_ch + 1] == '\'') || to_get_follow == i.left)
            continue;

        if (cur_ch == i.right.size() - 1) {
            if (i.left != i.right.substr(cur_ch, cur_ch))
                continue;
            vector<string> temp = get_follow_set(i.left);
            for (auto i:temp) {
                if (i != "~" && find(mid.begin(), mid.end(), i) == mid.end())
                    mid.push_back(i);
            }
        } else if (cur_ch == i.right.size() - 2) {
            string two_words = i.right.substr(cur_ch, i.right.size() - 1);
            if (i.left == two_words)
                continue;
            if (find(nonterminal.begin(), nonterminal.end(), two_words) != nonterminal.end()) {
                vector<string> temp = get_follow_set(i.left);

                for (auto i:temp) {
                    if (find(mid.begin(), mid.end(), i) == mid.end())
                        mid.push_back(i);
                }
            } else {
                cur_ch++;
                string mid2 = i.right.substr(cur_ch, cur_ch);
                if ((find(terminal.begin(), terminal.end(), mid2) != terminal.end()) &&
                    (find(mid.begin(), mid.end(), mid2) == mid.end()))
                    mid.push_back(mid2);
                else {
                    vector<string> temp = get_first_set(mid2);

                    for (auto j:temp) {
                        if (j != "~") {
                            if (find(mid.begin(), mid.end(), j) == mid.end())
                                mid.push_back(j);
                        } else {
                            vector<string> temp2 = get_follow_set(i.left);

                            for (auto k:temp2) {
                                if (k != "~" && find(mid.begin(), mid.end(), k) == mid.end())
                                    mid.push_back(k);
                            }
                        }
                    }
                }
            }
        } else {
            cur_ch++;
            string two_words = i.right.substr(cur_ch, i.right.size() - 1);
            if (find(nonterminal.begin(), nonterminal.end(), two_words) != nonterminal.end()) {
                vector<string> temp = get_first_set(two_words);

                for (auto j:temp) {
                    if (j != "~") {
                        if (find(mid.begin(), mid.end(), j) == mid.end())
                            mid.push_back(j);
                    } else {
                        vector<string> temp2 = get_follow_set(i.left);

                        for (auto k:temp2) {
                            if (find(mid.begin(), mid.end(), k) == mid.end())
                                mid.push_back(k);
                        }
                    }
                }
            }
        }
    }
    return mid;
}

void LL_1_grammer::analysis_table()
{
    cout << "\n\ntable:" << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
    cout << setw(10);
    terminal.emplace_back("$");
    for (int i = 0; i < terminal.size(); i++) {
        if (terminal[i] != "~")
            cout << terminal[i] << setw(10);
    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;

    for (auto i:nonterminal) {
        vector<string> line;
        cout  << i << setw(10);
        for (int j = 0; j < terminal.size(); j++) {
            if (terminal[j] == "~")
                continue;
            if (find(first[i].begin(), first[i].end(), "~") != first[i].end() &&
                find(follow[i].begin(), follow[i].end(), terminal[j]) != follow[i].end()) {
                line.emplace_back(i + "->~");
                table[number_non(i)][number_ter(terminal[j])] = i + "->~";
            } else if (find(first[i].begin(), first[i].end(), terminal[j]) != first[i].end()) {
                for (auto k:deduction) {
                    if (k.left == i) {
                        string temp = k.right.substr(0, 1);
                        if (k.right[0] == terminal[j][0]) {
                            line.emplace_back(k.left + "->" + k.right);
                            table[number_non(i)][number_ter(terminal[j])] = k.left + "->" + k.right;
                        } else if (find(first[temp].begin(), first[temp].end(), terminal[j]) != first[temp].end()) {
                            line.emplace_back(k.left + "->" + k.right);
                            table[number_non(i)][number_ter(terminal[j])] = k.left + "->" + k.right;
                        }

                    }
                }
            } else
                line.emplace_back(" ");
        }
        for (auto out:line)
            cout << setiosflags(ios::right) << setw(10) << out << resetiosflags(ios::right);
        cout << "\n";
    }
    cout << "\n\n";
}

void LL_1_grammer::analysis_program(string text)
{
    char a;
    int cur = 0;
    stack<string> buffer;
    buffer.push("$");
    buffer.push(start);

    cout << "-------------------------------------------------------------------------------" << endl;

    do {
        x = buffer.top();
        a = text[cur];
        if (x == "$" && a == '$')
            break;
        //判断x是否为终结符号
        bool judge = (find(terminal.begin(), terminal.end(), x) == terminal.end() && x != "n");
        if (!judge)//是终结符号
        {
            if (x[0] == a) {
                cout << "match" << endl;
                buffer.pop();
                cur++;
            } else {
                cout << "error" << endl;
                return;
            }
        } else {
            string temp1(1, a);
            int i = number_non(x);
            int j = number_ter(temp1);
            if (table[i][j] != "") {
                buffer.pop();
                string temp2 = table[i][j];
                cout << temp2 << endl;
                temp2 = temp2.substr(temp2.find(">") + 1, temp2.length() - (temp2.find(">") + 1));
                //逆序进栈
                for (int i = temp2.size() - 1; i >= 0; i--) {
                    if (temp2[i] == 'm')
                        i -= 2;
                    if (temp2[i] == '~')
                        break;
                    if (temp2[i] == '\'') {
                        i--;
                        string temp3;
                        temp3 = temp2.substr(i, 2);
                        buffer.push(temp3);
                    } else {
                        string temp3;
                        temp3 = temp2.substr(i, 1);
                        buffer.push(temp3);
                    }

                }
            } else {
                cout << "error" << endl;
                return;
            }
        }
    } while (1);
    cout << "success" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
}

int LL_1_grammer::number_non(string s)
{
    if (s == "E")
        return 1;
    else if (s == "T")
        return 2;
    else if (s == "F")
        return 3;
    else if (s == "E'")
        return 4;
    else if (s == "T'")
        return 5;
}

int LL_1_grammer::number_ter(string s)
{
    if (s == "+")
        return 1;
    else if (s == "-")
        return 2;
    else if (s == "*")
        return 3;
    else if (s == "/")
        return 4;
    else if (s == "(")
        return 5;
    else if (s == ")")
        return 6;
    else if (s == "num" || s == "n")
        return 7;
    else if (s == "$")
        return 8;
}

string LL_1_grammer::number_process(string input)
{
    bool flag = false;
    string result;
    for (int i = 0; i < input.length(); i++) {
        char c = input.at(i);
        if ('0' <= c && c <= '9') {
            if (!flag) {
                result += "n";
            }
            flag = true;
        } else {
            result += c;
            flag = false;
        }
    }
    return result + "$";
}
