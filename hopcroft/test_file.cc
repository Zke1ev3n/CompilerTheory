#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

using namespace std;

int main() {
    string input_file;
    fstream file;
    set<int> states;
    int temp_amount;
    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    file.open(input_file);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        return -1;
    }

    file >> temp_amount;
    for(int i = 0; i < temp_amount; i++) {
        int state;
        file  >> state;
        cout << state; 
    }
    cout << endl;

    file >> temp_amount;
    for(int i = 0; i < temp_amount; i++) {
        char alpha;
        file >> alpha;
        cout << alpha;
    }
    cout << endl;

    file.close();

    return 0;
}