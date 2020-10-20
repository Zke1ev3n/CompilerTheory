#include <fstream>
#include <iostream>
#include <sstream>
#include "dfa.h"

using namespace std;

DFA::DFA(string filepath) {
    fstream file;
    int amount;

    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        opened_file = false;
        return;
    }
    // We could open the file.
    opened_file = true;

    //读取状态
    file >> amount;
    for(int i = 0; i < amount; i++) {
        int state;
        file  >> state;
        states.insert(state);
    }

    //读取字符表
    file >> amount;
    for(int i = 0; i < amount; i++) {
        char alpha;
        file >> alpha;
        alphabets.insert(alpha);
    }

    //读取初始状态
    file >> start_state;

    //读取接受状态
    file >> amount;
    for(int i = 0; i < amount; i++) {
        int accept_state;
        file >> accept_state;
        accept_states.insert(accept_state);
    }

    //读取边信息
    file >> amount;
    for(int i = 0; i < amount; ++i){
        int from, to;
        char symbol;
        // Gets the transition data from the file.
        file >> from >> to >> symbol;
        //cout << from << to << symbol << endl;
        // Create a pair that will function as the key in our map.
        pair<int, char> key = make_pair(from, symbol);
        // If there are already existing transitions for the given key, just add the new destination to the set.
        if (this->transitions.find(key) != transitions.end()){
            transitions[key].insert(to);
        } else {
            // If not, add a new set containing the destination state.
            this->transitions[key] = {to};
        }
    }

    file.close();
}

bool DFA::found_file() {
    return opened_file;
}