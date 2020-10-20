#include "dfa.h"

using namespace std;

class MFA {
private:
    DFA* dfa;

    set<int> states;

    int start_state;

    set<int> accept_states;

    set<char> alphabets;

    // Dictionary that maps a state and a symbol to a set of states.
    map<pair<int,char>, set<int>> transitions;

    /* 在Hopcroft算法中,需要用来判断两次划分有没有变化, P用来保存前一次划分的结果, T用来保存划分后的结果 */
    set<set<int>> T, P;

    void Hopcroft();

    int FindMFAState(int state);

    pair<set<int>, set<int>> Split(set<int> S);
public:
    explicit MFA(DFA& dfa);

    // Writes the DFA to the given filepath.
    bool dump_file(string filepath);
};