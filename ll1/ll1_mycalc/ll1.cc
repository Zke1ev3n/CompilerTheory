//
// Created by Memo on 2018/11/24.
//

#include "ll1.h"

//bool LL_1_grammer::compare(DEDUCTION x, DEDUCTION y)
//{
//    return x.left < y.left ;
//}

void LL1::init(const string filename){
    ifstream input;
    string line;
    string current_symbol;
    input.open(filename);
    while(getline(input, line)) {
        stringstream ss(line);
        getline(ss, current_symbol, ' ');
        if(current_symbol[0] == '<' and current_symbol.back() == '>' && current_symbol.length() > 2) {
            if(find(nonterminal.begin(), nonterminal.end(), current_symbol) == nonterminal.end()){
                nonterminal.insert(current_symbol);
            }
        }
        //跳过->符号
        getline(ss, current_symbol, ' ');
        while(getline(ss, current_symbol, ' ')) {                
             if(current_symbol[0] == '<' and current_symbol.back() == '>' && current_symbol.length() > 2) {
                if(find(nonterminal.begin(), nonterminal.end(), current_symbol) == nonterminal.end()){
                    nonterminal.insert(current_symbol);
                }
            }else{
                if(find(terminal.begin(), terminal.end(), current_symbol) == terminal.end()) {
                    terminal.insert(current_symbol);
                }
            }
		}
    }
	input.close();
}

void LL1::parse_bnf(const string filename) {
    ifstream input;
    string line;
    string current_symbol;
    input.open(filename);
    while(getline(input, line)) {
        DEDUCTION deduction;
        stringstream ss(line);
        getline(ss, current_symbol, ' ');
        // if(current_symbol[0] == '<' && current_symbol.back() == '>') {
        //     cout << "error!" <<endl;
        //     return;
        // }
        if(current_symbol == "") continue;
        deduction.left = current_symbol;
        //跳过->符号
        getline(ss, current_symbol, ' ');
        while(getline(ss, current_symbol, ' ')) {                
            deduction.right.push_back(current_symbol);
		}
        ll_pushback(deduction);
    }
	start = deduction.begin()->left;
	input.close();
}

void LL1::remove_direct_left_recursion()
{
    for (int i = 0; i < deduction.size(); i++) {
        if (deduction[i].left == deduction[i].right[0])//如果有左递归
        {
            deduction[i].is_left_recursion = true;
            string temp = deduction[i].left;
            for (int j = 0; j < deduction.size(); j++)//将该非终结符左递归式全部标记
            {
                if (temp == deduction[j].left &&
                    deduction[j].left == deduction[j].right[0] && i != j)
                    deduction[j].is_left_recursion = true;
            }
            for (int j = 0; j < deduction.size(); j++)//消除左递归
            {
                if (deduction[j].left == temp && !deduction[j].is_left_recursion)
                    deduction[j].right.push_back(temp + "'");
                if (deduction[j].left == temp && deduction[j].is_left_recursion) {
                    deduction[j].left = temp + "'";
                    deduction[j].right.erase(deduction[j].right.begin());
                    deduction[j].right.push_back(temp + "'");
                    deduction[j].is_left_recursion = false;
                }
            }
            DEDUCTION mid;
            mid.left = temp + "'";
            mid.right.push_back("~");
            deduction.push_back(mid);
            nonterminal.insert(temp + "'");
            if (find(terminal.begin(), terminal.end(), "~") == terminal.end())
                terminal.insert("~");
        }
    }
}

void LL1::print_test()
{
    cout << "---------------------------" << endl;
    cout << "expresion:" << endl;
	for(int i=0; i < deduction.size(); i++){
		cout << i << ". " << deduction[i].left << " -> ";
        for(auto s : deduction[i].right){
            cout << s << " ";
        }
        cout << endl;
	}

    cout << "---------------------------" << endl;
    cout << "nonterminal set:" << endl;
    for (auto i: nonterminal)
        cout << i << endl;

    cout << "---------------------------" << endl;
    cout << "terminal set:" << endl;
    for (auto i: terminal)
        cout << i << endl;

    cout << "---------------------------" << endl;
    cout << "first set:" << endl;
	for(auto it = first.begin(); it != first.end(); ++it) {
		cout<<it->first<<" : ";
		for(auto firsts_it = it->second.begin(); firsts_it != it->second.end(); ++firsts_it) {
			cout<<*firsts_it<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";


    cout << "---------------------------" << endl;
    cout << "follow set:" << endl;
	for(auto it = follow.begin(); it != follow.end(); ++it) {
		cout<<it->first<<" : ";
		for(auto follows_it = it->second.begin(); follows_it != it->second.end(); ++follows_it) {
			cout<<*follows_it<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";

}

/*
      first(Epsilon) -> EmptySet
      first(A -> a) -> { a } 
                        
      first(A -> A B) -> { first(A) U first( B),   if nullable(A)
                        { first(A),              otherwise
      first(A -> A1 | A2 | ... | AN) -> first(A1) U first(A2 U ... U first(AN)
*/

//TODO 实现不动点算法
void LL1::get_first_set(string to_get_first)//构建first集
{
  	// cout<<"Finding firsts of "<<non_term<<"\n";

    for (auto i:deduction) {
		// Find productions of the non terminal
		if(i.left != to_get_first) {
			continue;
		}

		// cout<<"Processing production "<<it->first<<"->"<<it->second<<"\n";

		vector<string> rhs = i.right;
		// Loop till a non terminal or no epsilon variable found
        for(auto iter = rhs.begin(); iter != rhs.end(); iter++) {
			// If first char in production a non term, add it to firsts list
            if(find(nonterminal.begin(), nonterminal.end(), *iter) == nonterminal.end()) {
                first[to_get_first].insert(*iter);
				break;
			}
			else {
				// If char in prod is non terminal and whose firsts has no yet been found out
				// Find first for that non terminal
				if(first[*iter].empty()) {
                    //cout<<*iter<<endl;
					get_first_set(*iter);
				}
				// If variable doesn't have epsilon, stop loop
                // nullable集也表示first集中包含空
				if(first[*iter].find("~") == first[*iter].end()) {
					first[to_get_first].insert(first[*iter].begin(), first[*iter].end());
					break;
				}

                //如果属于nullable
				set<string> firsts_copy(first[*iter].begin(), first[*iter].end());

                //如果当前非终结符不是最后一个，则推导符号属于nullable
				// Remove epsilon from firsts if not the last variable
				if(iter + 1 != rhs.end()) {
					firsts_copy.erase("~");
				}

				// Append firsts of that variable
				first[to_get_first].insert(firsts_copy.begin(), firsts_copy.end());
			}
		}
		
	}

}

void LL1::first_set()
{
    for (auto i:nonterminal)
        get_first_set(i);
}

void LL1::follow_set()
{
    //for (auto i:nonterminal)
    //    get_follow_set(i);
	cout<<"start: " << start <<endl;
	follow[start].insert("$");
	get_follow_set(start);
	// Find follows for rest of variables
	for(auto it = nonterminal.begin(); it != nonterminal.end(); ++it) {
		if(follow[*it].empty()) {
			get_follow_set(*it);
		}
	}
}

void LL1::get_follow_set(string to_get_follow)//构建follow集
{
    //cout<<"Finding follow of "<<to_get_follow<<"\n";
    for (auto i:deduction) {

		// finished is true when finding follow from this production is complete
		bool finished = true;
		auto ch = i.right.begin();

		// Skip variables till reqd non terminal
		for(;ch != i.right.end() ; ++ch) {
			if(*ch == to_get_follow) {
				finished = false;
				break;
			}
		}
		if(*ch == i.left) continue;
		++ch;

		for(;ch != i.right.end() && !finished; ++ch) {
			// If non terminal, just append to follow
            if(find(nonterminal.begin(), nonterminal.end(), *ch) == nonterminal.end()) {
				follow[to_get_follow].insert(*ch);
				finished = true;
				break;
			}

			set<string> firsts_copy(first[*ch]);
			// If char's firsts doesnt have epsilon follow search is over 
			//如果当前符号不属于nullable集合，则直接并上First集，并且跳出循环
			if(firsts_copy.find("~") == firsts_copy.end()) {
				follow[to_get_follow].insert(firsts_copy.begin(), firsts_copy.end());
				finished = true;
				break;
			}
			// Else next char has to be checked after appending firsts to follow
			firsts_copy.erase("~");
			follow[to_get_follow].insert(firsts_copy.begin(), firsts_copy.end());

		}

		// If end of production, follow same as follow of variable
		if(ch == i.right.end() && !finished) {
			// Find follow if it doesn't have
			if(follow[i.left].empty()) {
				//cout<<"i.left:" << i.left<<endl;
				get_follow_set(i.left);
			}
			follow[to_get_follow].insert(follow[i.left].begin(), follow[i.left].end());
		}

	}
    
}

void LL1::analysis_table()
{
    //TODO 更好的方式
    terminal.insert("$");
	table = new int*[nonterminal.size()];
    for(int i =0; i < nonterminal.size(); i++){
        table[i] = new int[terminal.size()];
    }
	for(int i = 0; i < nonterminal.size(); i++){
		for(int j = 0; j < terminal.size(); j++){
			table[i][j] = -1;
		}
	}

	for(auto prod = deduction.begin(); prod != deduction.end(); ++prod) {
		vector<string> rhs = prod->right;

		set<string> next_list;
		bool finished = false;
		for(auto ch = rhs.begin(); ch != rhs.end(); ++ch) {
            if(find(nonterminal.begin(), nonterminal.end(), *ch) == nonterminal.end()) {
				if(*ch != "~") {
					next_list.insert(*ch);
					finished = true;
					break;
				}
				continue;
			}

			set<string> firsts_copy(first[*ch].begin(), first[*ch].end());
			if(firsts_copy.find("~") == firsts_copy.end()) {
				next_list.insert(firsts_copy.begin(), firsts_copy.end());
				finished = true;
				break;
			}
			firsts_copy.erase("~");
			next_list.insert(firsts_copy.begin(), firsts_copy.end());
		}
		// If the whole rhs can be skipped through epsilon or reaching the end
		// Add follow to next list
		if(!finished) {
			next_list.insert(follow[prod->left].begin(), follow[prod->left].end());
		}


		for(auto ch = next_list.begin(); ch != next_list.end(); ++ch) {
			int row = distance(nonterminal.begin(), nonterminal.find(prod->left));
			int col = distance(terminal.begin(), terminal.find(*ch));
			int prod_num = distance(deduction.begin(), prod);
            //string prod_str = prod.left + "->" + *ch;
			if(table[row][col] != -1) {
				cout<<"Collision at ["<<row<<"]["<<col<<"] for production "<<prod_num<<"\n";
				continue;
			}
			table[row][col] = prod_num;
		}

	}
	// Print parse table
	cout<<"Parsing Table: \n";
	cout<<"   ";
	for(auto i = terminal.begin(); i != terminal.end(); ++i) {
		cout<<*i<<" ";
	}
	cout<<"\n";
	for(auto row = nonterminal.begin(); row != nonterminal.end(); ++row) {
		cout<<*row<<"  ";
		for(int col = 0; col < terminal.size(); ++col) {
			int row_num = distance(nonterminal.begin(), row);
			if(table[row_num][col] == -1) {
				cout<<"- ";
				continue;
			}
			cout<<table[row_num][col]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

void LL1::analysis_program(vector<Token> tokens)
{
	cout << "-------------------------------------------------------------------------------" << endl;
	bool accepted = true;
	stack<string> st;
	st.push("$");
	st.push(start);

	while(!st.empty()) {
		// If stack top same as input string char remove it
		if(tokens[0].tokenType == st.top()) {
			st.pop();
			tokens.erase(tokens.begin());
		}
		else if(find(terminal.begin(), terminal.end(), st.top()) != terminal.end()) {
			cout<<tokens[0].tokenType<<endl;
			cout<<st.top()<<endl;
			cout<<"Unmatched terminal found\n";
			accepted = false;
			break;
		}
		else {
			string stack_top = st.top();
			int row = distance(nonterminal.begin(), nonterminal.find(stack_top));
			int col = distance(terminal.begin(), terminal.find(tokens[0].tokenType));
			int prod_num = table[row][col];

			if(prod_num == -1) {
				cout<<"No production found in parse table\n";
				accepted = false;
				break;
			}

			st.pop();
			vector<string> rhs = deduction[prod_num].right;
			if(rhs[0] == "~") {
				continue;
			}
			for(auto ch = rhs.rbegin(); ch != rhs.rend(); ++ch) {
				st.push(*ch);
			}
		}
	}
	if(accepted) {
		cout<<"Input string is accepted"<<endl;;
	}
	else {
		cout<<"Input string is rejected"<<endl;
	}
	cout << "-------------------------------------------------------------------------------" << endl;
}