#include <fstream>
#include <iostream>
#include "mfa.h"

//传入DFA，输出最小化DFA
MFA::MFA(DFA& dfa) {
    this->dfa = &dfa;

    this->alphabets = this->dfa->alphabets;

    //使用hopcroft算法最小化
    Hopcroft();

    //生成总状态
    set<set<int>>::iterator it = T.begin();
    for(int i = 0; i < T.size(); i++, it++) {
        states.insert(i);
        for(auto s : *it) {
            if(this->dfa->accept_states.find(s) != this->dfa->accept_states.end()) {
                //生成接受状态
                accept_states.insert(i);
            }
        }
        if((*it).find(this->dfa->start_state) != (*it).end()) {
            start_state = i;
        }
    }

    //构建新的边
    for(auto trans : this->dfa->transitions) {
        //找到起点和重点
        for(auto s : trans.second) {
            int from = FindMFAState(trans.first.first);
            int to = FindMFAState(s);
            pair<int, char> key = make_pair(from, trans.first.second);
            if (this->transitions.find(key) != transitions.end()){
                transitions[key].insert(to);
            } else {
                this->transitions[key] = {to};
            }
        }
    }
}

int MFA::FindMFAState(int state) {
    set<set<int>>::iterator it = T.begin();
    for(int i = 0; i < T.size(); i++, it++) {
        if((*it).find(state) != (*it).end()) {
            return i;
        }
    }
    //正常情况肯定能找到
    return -1;
}

//hopcroft的核心思想是不动点算法, 即当状态不再改变后退出
void MFA::Hopcroft() {
    set<int> naccept_states;
    //取差集
    set_difference(dfa->states.begin(), dfa->states.end(), dfa->accept_states.begin(), dfa->accept_states.end(), inserter(naccept_states, naccept_states.begin()));

    T.insert(dfa->accept_states);
    T.insert(naccept_states);

    while(T != P){
        P = T;
        T.clear();
        for(auto it = P.begin(); it != P.end(); ++it){
            auto temp = *it;
            auto pair = Split(temp);
            if(!pair.first.empty()) {
                T.insert(pair.first);
            }
            if(!pair.second.empty()){
                T.insert(pair.second);
            }
        }
    }
}

pair<set<int>, set<int>> MFA::Split(set<int> S) {
    set<int> rv;
	set<int> rv2 = S;
    set<set<int>>::iterator last = P.end();
    for (auto symbol : dfa->alphabets) {
        //map<pair<int,char>, set<int>> transitions;
        //遍历每条边
        for(auto trans : dfa->transitions){
            //遍历根据当前符号转移并且起点在S中的
            if(trans.first.second == symbol && S.find(trans.first.first) != S.end()){
                //遍历终点
                for(auto to: trans.second){
                    //如果终点不在S中
                    if(S.find(to) == S.end()){
                        //遍历P集合
                        for(auto k_it = P.begin(); k_it != P.end(); ++k_it){
                            //排除S
                            if(*k_it == S){
                                continue;
                            }
                            if(k_it->find(to) != k_it->end()){
                                //每次只拆分一次，如果发现last已经记录了就只用last
                                if(last == P.end()) {
                                    last = k_it;
                                }
                                if(k_it == last) {
                                    rv.insert(trans.first.first);
                                }
                            }
                        }
                    }
                }
            }
        }
        //通过以c为字符的边，一个集合中的所有点指向自身，或者指向同一个另外的集合，就不用分割。
		if (!rv.empty() && rv.size() != S.size()) {
			//addnewset = true;
			for (auto &d : rv) rv2.erase(d);
			break;
        }else{
            rv.clear();
        }
    }
    return make_pair(rv, rv2);
}

bool MFA::dump_file(string filepath) {
    // We need to update the translations from set of states to an int.

    ofstream file;
    file.open(filepath);
    // Check if we could open the provided filepath, if not, return false.
    if (!file.is_open()) {
        cout << "File to write results not opened correctly" << endl;
        return false;
    }

    //写状态
    file << states.size() << "\n";
    for(auto s: states) {
        file << s << " ";
    }
    file << "\n";

    //写字符表
    file << alphabets.size() << "\n";
    for(auto s: alphabets) {
        file << s << " ";
    }
    file << "\n";

    //写初始状态
    file << start_state << "\n";

    //写结束状态
    file << accept_states.size() << "\n";
    for(auto s: accept_states) {
        file << s << " ";
    }
    file << "\n";

    //写边信息
    file << transitions.size() << "\n";
    for(auto trans:transitions){
        for(auto to : trans.second) {
            file << trans.first.first << " " << to << " " << trans.first.second << "\n";
        }
    }

    file.close();

    // Return that the file was written successfully.
    return true;
}