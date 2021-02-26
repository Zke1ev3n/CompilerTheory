
#include "parser.h"

//建立symbol映射
// void Parser::Init() {

// }

void Parser::Parse(vector<Token> tokens) {
    int error = 0;

    stack<StackState> xstack;

    int token_index = 0;

    // init stack
    StackState ss;
    ss.state = 0;
    ss.symbol = "$";  //$

    xstack.push(ss);

    while (token_index < tokens.size()) {
        if (error == 1) break;

        while (xstack.size() > 0) {
            /*
                取出当前的state
            */
            Token t = tokens[token_index];
            int symbol_index = lalr1->symbol_map[t.tokenType];
            StackState ss = xstack.top();
            //先定位到行，在定位到列
            pair<int, int> action = lalr1->output_table[ss.state][symbol_index];
            //cout<<action.first<<","<<action.second<<endl;
            if (action.first == LR_ACTION_SHIFT) {
                StackState nss;
                nss.state = action.second;
                nss.symbol = t.tokenType;
                // nss.t = t;
                xstack.push(nss);
                break;
            } else if (action.first == LR_ACTION_REDUCE) {
                pair<int, int> prod_info = lalr1->reduce_map[action.second];
                for (int i = 0; i < prod_info.second; i++) {
                    xstack.pop();
                }

                ss = xstack.top();
                pair<int, int> goto_action = lalr1->output_table[ss.state][prod_info.first];
                if (goto_action.first != LR_ACTION_GOTO) {
                    std::cout << "Error: Syntax error.\n" << std::endl;
                    error = 1;
                    break;
                }
                cout<<t.tokenType<<endl;;

                StackState nss;
                nss.state = goto_action.second;
                nss.symbol = prod_info.first;
                // nss.t = rct;
                xstack.push(nss);

            } else if (action.first == LR_ACTION_ACCEPT) {
                xstack.pop();
                std::cout << "Success: Accept.\n" << std::endl;
                break;
            } else {
                std::cout << "Error: Syntax error.\n" << std::endl;
                error = 1;
                break;
            }
        }
        token_index++;
    }
}