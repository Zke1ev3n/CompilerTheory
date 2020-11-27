#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "ll1.h"

using namespace std;

// bool calclute(const string & exp, float & result)
// {
// 	list<Token> tokens;
// 	if (tokenize(exp, tokens))
// 	{
//         for(auto token : tokens) {
//             if(token.tokenType == TokenType::NUM){
//                 cout << token.value.num << endl;
//             }else{
//                 cout << token.value.symbol << endl;
//             }
//         }
//     }
		
// }

int main(int argc, char* argv[])
{
    ifstream input;
	string expression;
	float result;
    LL1 ll1;

    if(argc < 2) {
        cout << "please input grammer file" << endl;
        return -1;
    }
    string filename = argv[1];
    ll1.init(filename);
    ll1.parse_bnf(filename);
    ll1.remove_direct_left_recursion();
    ll1.first_set();
    ll1.follow_set();
    ll1.print_test();

    //开始校验
	// cout << "(Input empty string to exit.)\n";
	// while (true)
	// {
	// 	getline(cin, expression);
	// 	if (expression.empty())
	// 	{
	// 		return 0;
	// 	}
	// 	else if(calclute(expression, result))
	// 	{
	// 		cout << result << endl;
	// 	}
	// }
}