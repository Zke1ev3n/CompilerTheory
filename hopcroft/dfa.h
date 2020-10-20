#ifndef DFA_H
#define DFA_H

#include <set>
#include <map>

using namespace std;

class DFA {
private:
    // Shows if the input file could be opened.
    bool opened_file;

    // The initial node (by definition always 0).
    int start_state;

    // Contains all final states in the DFA.
    set<int> accept_states;

    //Contains all states in thr DFA.
    set<int> states;

    //Contains all alphabets for the DFA
    set<char> alphabets;

    // Dictionary that maps a state and a symbol to a set of states.
    map<pair<int,char>, set<int>> transitions;

    // So that MFA can access private values.
    friend class MFA;
public:
    explicit DFA(string filepath);
    // Returns if the file could be found.
    bool found_file();
};

#endif