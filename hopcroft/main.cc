#include <iostream>
#include "dfa.h"
#include "mfa.h"

using namespace std;

int main() {
    string input_file;
    string output_file;

    cout << "Please input the filepath for the input" << endl;
    cin >> input_file;

    // Creates an NFA from the input file.
    DFA dfa(input_file);
    // If we didn't find the file, just return.
    if(!dfa.found_file()){
        cout << "Could not open input file" << endl;
        return 0;
    }

    cout << "Please input the filepath for the output" << endl;
    cin >> output_file;

    // Creates a DFA from the given NFA.
    MFA mfa(dfa);

    // Writes the data to the specified path, checking if it was successful.
    if(mfa.dump_file(output_file)){
        cout << "Output written successfully!\n";
    } else {
        cout << "Output could not be written in the provided path\n";
    }

    return 0;
}