#include <iostream>
#include <fstream>
#include <sstream>
#include "lalr1.h"

using namespace std;

int main(int argc, char* argv[])
{
    ifstream input;
	float result;
    LALR1 lalr1;
	
    if(argc < 2) {
        cout << "please input grammer file" << endl;
        return -1;
    }
    string filename = argv[1];
    lalr1.Init(filename);
    lalr1.FindFirstSet();
    lalr1.FindStates();
    lalr1.PrintTest();
}