#include "lalr1.h"

//bool LL_1_grammer::compare(DEDUCTION x, DEDUCTION y)
//{
//    return x.left < y.left ;
//}

void LALR1::parse_bnf(const string filename) {
    ifstream input;
    string line;
    string notermainal_symbol;
    string current_symbol;
    input.open(filename);
    while(getline(input, line)) {
        Production production;
        stringstream ss(line);

        getline(ss, current_symbol, ' ');
        while(current_symbol == "" && ss){
            getline(ss, current_symbol, ' ');
        }
                    
        if(current_symbol == "" || current_symbol == ";") continue;
        
        if(current_symbol == "|"){
            production.left = notermainal_symbol;
        }else{
            if(find(nonterminal.begin(), nonterminal.end(), current_symbol) == nonterminal.end()){
                nonterminal.insert(current_symbol);
            }
            notermainal_symbol = current_symbol;
            production.left = current_symbol;
            //跳过 : 符号
            getline(ss, current_symbol, ' ');
        }
        
        while(getline(ss, current_symbol, ' ')) {                
            if(current_symbol[0] == '\'' and current_symbol.back() == '\'' && current_symbol.length() > 2) {
                //插入终结符
                current_symbol = current_symbol.substr(1, current_symbol.size() - 2);
                production.right.push_back(current_symbol);
                if(find(terminal.begin(), terminal.end(), current_symbol) == terminal.end()){
                    terminal.insert(current_symbol);
                }
            }else{
                //插入非终结符
                production.right.push_back(current_symbol);
                if(find(nonterminal.begin(), nonterminal.end(), current_symbol) == nonterminal.end()) {
                    nonterminal.insert(current_symbol);
                }
            }
		}
        lalr1_pushback(production);
    }

	input.close();
    //增广文法(Augmented Grammar)
    string start_ = productions.begin()->left;
    start = start_ + '\'';
    Production p0;
    p0.left = start;
    p0.right.push_back(start_);
    //productions.insert(productions.begin(), p0);
    //插入增广符号
    nonterminal.insert(start);
    //插入结束符号
    terminal.insert("$");
    lalr1_pushback(p0);
}

void LALR1::print_test()
{
    cout << "---------------------------" << endl;
    cout << "expresion:" << endl;
	for(int i=0; i < productions.size(); i++){
		cout << i << ". " << productions[i].left << " -> ";
        for(auto s : productions[i].right){
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
    cout << "nullable set:" << endl;
	for(auto it = nullable.begin(); it != nullable.end(); ++it) {
		cout<<*it<<endl;
	}
	cout<<"\n";

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


    // cout << "---------------------------" << endl;
    // cout << "follow set:" << endl;
	// for(auto it = follow.begin(); it != follow.end(); ++it) {
	// 	cout<<it->first<<" : ";
	// 	for(auto follows_it = it->second.begin(); follows_it != it->second.end(); ++follows_it) {
	// 		cout<<*follows_it<<" ";
	// 	}
	// 	cout<<"\n";
	// }
	// cout<<"\n";

}

//插入value到set中
int set_add(set<string> &set1, string value) {
    int rv;
    rv = set1.find(value) == set1.end();
    set1.insert(value);
    return rv;
}

//合并两个set
int set_union(set<string> &set1, set<string> &set2) {
  int i, progress;
  progress = 0;
  for(auto it = set2.begin(); it != set2.end(); it++){
      if(find(set1.begin(), set1.end(), *it) == set1.end()){
          progress = 1;
          set1.insert(*it);
      }
  }
  return progress;
}

/*
<expr> : ( - num 
<expr>' : + - ~ 
<factor> : ( - num 
<term> : ( - num 
<term>' : * / ~
*/
void LALR1::find_first_sets() {

    int progress;

    //构建nullable集合
    do{
        progress = 0;
        for(auto p:productions){
            if(find(nullable.begin(), nullable.end(), p.left) != nullable.end()){
                continue;
            }
            vector<string> rhs = p.right;
            
            if(rhs.size() == 1 && rhs[0] == "~"){
                nullable.insert(p.left);
            }
            vector<string>::iterator iter;
		    // Loop till a non terminal or no epsilon variable found
            for(iter = rhs.begin(); iter != rhs.end(); iter++) {
                //不是非终结符或者不在nullable集合中
                if(find(nonterminal.begin(), nonterminal.end(), *iter) == nonterminal.end() || 
                    find(nullable.begin(), nullable.end(), p.left) == nullable.end()){
                    break;
                }
            }
            if(iter == rhs.end()) {
                nullable.insert(p.left);
                progress = 1;
            }

        }
    }while(progress);

    do {
        progress = 0;
        string s1, s2;
        for(auto p:productions){
            s1 = p.left;
            for(auto iter=p.right.begin(); iter != p.right.end(); iter++){
                s2 = *iter;
                if(find(terminal.begin(), terminal.end(), s2) != terminal.end()) {
                    progress += set_add(first[s1], s2);
                }else if(s1 == s2){
                    if(find(nullable.begin(), nullable.end(), s1) == nullable.end()) break;
                }else{
                    progress += set_union(first[s1], first[s2]);
                    if(find(nullable.begin(), nullable.end(), s2) == nullable.end()) break;
                }
            }
        }
    }while(progress);

    return;
}

//TODO 实现不动点算法
void LALR1::get_first_set(string to_get_first)//构建first集
{
  	// cout<<"Finding firsts of "<<non_term<<"\n";

    cout<<to_get_first<<endl;
    for (auto i:productions) {
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

void LALR1::first_set()
{
    for (auto i:nonterminal)
        get_first_set(i);
}