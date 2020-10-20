#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

vector<short int> final_states, dfa_final_states;

//vector<vector<short int>> dfa_id, is_visited_vec;

struct NFA
{
  //int state_nums;
  vector<int> states;
  //int symbol_nums;
  vector<char> symbols;
  //int accept_state_nums;
  vector<int> accept_states;
  int start_state;
  //vector<short int> **transtions;
  //vector<map<char, vector<short int>>> **egdes;
  //状态根据不同的字符到达的状态集合
  //map<int, map<char, set<int>>> transtions;
  map<pair<int,char>, set<int> > transitions;
};

typedef struct NFA NFA;
//定义nfa结构
NFA nfa;
/*
输入NFA转换表
状态用数字表示，符号用小写字母，eps用*表示
第一行：
状态个数 终结状态个数 符号个数
开始状态
终结状态
起始状态 符号 目标状态
*/
int main(int argc, char *argv[])
{
  //每个边的开始状态和结束的状态
  int nfa_state_nums, nfa_accept_state_nums, nfa_symbol_nums;
  int start, end;
  //字母
  char c;
  int i;
  //int st, end,
  ifstream in("data.in");
  in >> nfa_state_nums >> nfa_accept_state_nums >> nfa_symbol_nums;
  //nfa.states = new vector<int>(nfa_state_nums);
  //nfa.accept_states = new vector<int>(nfa_accept_state_nums);
  //nfa.symbols = new vector<char>(nfa_symbol_nums);
  nfa.states.resize(nfa_state_nums);
  nfa.accept_states.resize(nfa_accept_state_nums);
  nfa.symbols.resize(nfa_symbol_nums);
  /*
  nfa.transtions = new vector<short int>*[nfa_state_nums];
  for(int i = 0; i < nfa_state_nums; i++){
    nfa.transtions[i] = new vector<short int>[nfa_symbol_nums];
  }
  */
  //定义开始状态
  in >> nfa.start_state;
  //定义结束状态
  for (i = 0; i < nfa_accept_state_nums; i++)
  {
    in >> nfa.accept_states[i];
  }
  while (in >> start >> c >> end)
  {
    pair<int, char> key = make_pair(start, c);
    if (nfa.transitions.find(key) != nfa.transitions.end()){
            nfa.transitions[key].insert(end);
        } else {
            // If not, add a new set containing the destination state.
            nfa.transitions[key] = {end};
        }
  }
    vector<vector<short int> > unvis;
    vector<short int> init_states;
    init_states.push_back(nfa.start_state);
    //std::vector<int> eclos = eclosure(initialStateVector, NFATable);
    unvis.push_back(init_states);
    while(!unvis.empty())
    {
        vector<short int> temp=unvis.back();
        unvis.pop_back();
        isVisitedVec.push_back(temp);
        for(char s='a'; s<'a'+symbolCount; ++s)
        {
            vector<short int> currentState=Move(temp, s);
            vector<short int>::iterator j;
            if(!currentState.empty())
            {
                if(!isVisited(currentState))
                    unvis.push_back(currentState);
                t1=name(temp);
                t2=name(currentState);
                dfa[t1].push_back(make_pair(s, t2));
            }
        }
    }
}
