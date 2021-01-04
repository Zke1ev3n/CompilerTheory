#include "lalr1.h"

// bool LL_1_grammer::compare(DEDUCTION x, DEDUCTION y)
//{
//    return x.left < y.left ;
//}

Symbol *LALR1::NewSymbol(const string name, SYMBOL_TYPE type) {
    Symbol *sp;
    sp = FindSymbol(name);
    if (sp == nullptr) {
        sp = new Symbol();
        sp->index = symbols.size();
        sp->type = type;
        sp->name = name;
        sp->nullable = false;
        symbols.push_back(sp);
    }
    return sp;
}

Symbol *LALR1::FindSymbol(const string name) {
    for(int i = 0; i < symbols.size(); i++){
        if(symbols[i]->name == name) {
            return symbols[i];
        }
    }
    return nullptr;
}

void LALR1::Init(const string filename) {
    ifstream input;
    string line;
    Symbol *current_noterminal;
    string current_name;
    input.open(filename);
    while (getline(input, line)) {
        Production *production = new Production();
        stringstream ss(line);

        getline(ss, current_name, ' ');
        while (current_name == "" && ss) {
            getline(ss, current_name, ' ');
        }

        if (current_name == "" || current_name == ";") continue;

        if (current_name == "|") {
            production->left = current_noterminal;
        } else {
            current_noterminal = NewSymbol(current_name, ST_NONTERMINAL);
            production->left = current_noterminal;
            //跳过 : 符号
            getline(ss, current_name, ' ');
        }

        while (getline(ss, current_name, ' ')) {
            if (current_name[0] == '\'' and current_name.back() == '\'' && current_name.length() > 2) {
                //插入终结符
                current_name = current_name.substr(1, current_name.size() - 2);
                Symbol *symbol = NewSymbol(current_name, ST_TERMINAL);
                production->right.push_back(symbol);
            } else {
                //插入非终结符
                Symbol *symbol = NewSymbol(current_name, ST_NONTERMINAL);
                production->right.push_back(symbol);
            }
        }
        lalr1_pushback(production);
    }

    input.close();
    //增广文法(Augmented Grammar)
    //新增一个标记为开始的非终结符，防止开始的非终结符出现在右边
    //TODO 这里先设置为terminal
    Symbol *accept_symbol = NewSymbol("$", ST_TERMINAL);
    Symbol *start_symbol = productions[0]->left;
    start = new Production();
    start->left = NewSymbol(start_symbol->name + '\'', ST_NONTERMINAL);
    start->right.push_back(start_symbol);
    start->right.push_back(accept_symbol);
    //将新增的产生式添加到最前面
    productions.insert(productions.begin(), start);
    //插入结束符号
    // terminal.insert("$");
}

void LALR1::PrintTest() {
    cout << "---------------------------" << endl;
    cout << "expresion:" << endl;
    for (int i = 0; i < productions.size(); i++) {
        cout << i << ". " << productions[i]->left->name << " -> ";
        for (auto s : productions[i]->right) {
            cout << s->name << " ";
        }
        cout << endl;
    }

    cout << "---------------------------" << endl;
    cout << "nonterminal set:" << endl;
    for (auto i : symbols) {
        if (i->type == ST_NONTERMINAL) cout << i->name << endl;
    }

    cout << "---------------------------" << endl;
    cout << "terminal set:" << endl;
    for (auto i : symbols) {
        if (i->type == ST_TERMINAL) cout << i->name << endl;
    }

    cout << "---------------------------" << endl;
    cout << "nullable set:" << endl;
    for (auto i : symbols) {
        if (i->type == ST_NONTERMINAL && i->nullable == true) cout << i->name << endl;
    }
    cout << endl;
    ;

    cout << "---------------------------" << endl;
    cout << "first set:" << endl;
    for (auto i : symbols) {
        if (i->type == ST_NONTERMINAL && i->firstset.size() > 0) {
            cout << i->name << " : ";
            for (auto j : i->firstset) {
                cout << j->name << " ";
            }
            cout << endl;
        }
    }
    cout << "\n";

    // cout << "---------------------------" << endl;
    // cout << "follow set:" << endl;
    // for(auto it = follow.begin(); it != follow.end(); ++it) {
    // 	cout<<it->first<<" : ";
    // 	for(auto follows_it = it->second.begin(); follows_it !=
    // it->second.end(); ++follows_it) { 		cout<<*follows_it<<" ";
    // 	}
    // 	cout<<"\n";
    // }
    // cout<<"\n";
    cout << "---------------------------" << endl;
    cout << "lr state set:" << endl;
    for (auto i : states) {
        cout << "state" << i->index << " : " << endl;
        for ( auto j : i->items) {
            cout<<j->prod->left->name<<" -> ";
            for (int k = 0; k < j->prod->right.size(); k ++) {
                if(k == j->dot) cout << "•";
                cout << " " << j->prod->right[k]->name;
            }
            cout<<" [";
            for(auto m : j->forwards) {
                cout<<m->name;
            }
            cout<<"] "<< endl;
        }
    }
}

//插入value到set中
int set_add(vector<Symbol *> &set1, Symbol *value) {
    int rv = 1;
    for (auto symbol : set1) {
        if (symbol == value) {
            rv = 0;
        }
    }
    if (rv == 1) {
        set1.push_back(value);
    }
    return rv;
}

//合并set2到set1
int set_union(vector<Symbol *> &set1, vector<Symbol *> &set2) {
    int insert_count;
    ;
    bool exist;

    insert_count = 0;
    for (auto symbol2 : set2) {
        exist = false;
        for (auto symbol1 : set1) {
            if (symbol1 == symbol2) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            insert_count++;
            set1.push_back(symbol2);
        }
    }
    return insert_count;
}

void LALR1::FindFirstSet() {
    int i;
    int progress;

    //每一个终结符的first都是它自己
    for(auto s : symbols) {
        if(s->type == ST_TERMINAL) {
            s->firstset.push_back(s);
        }
    }

    //构建nullable集合
    do {
        progress = 0;
        for (auto p : productions) {
            if (p->left->nullable) continue;
            vector<Symbol *> rhs = p->right;

            // TODO 简化?
            if (rhs.size() == 1 && rhs[0]->name == "~") {
                p->left->nullable = true;
                progress = 1;
            }
            // Loop till a non terminal or no epsilon variable found
            for (i = 0; i < rhs.size(); i++) {
                //不是非终结符或者不在nullable集合中
                if (rhs[i]->type != ST_NONTERMINAL || rhs[i]->nullable == false) {
                    break;
                }
            }
            if (i == rhs.size()) {
                p->left->nullable = true;
                progress = 1;
            }
        }
    } while (progress);

    do {
        progress = 0;
        Symbol *s1;
        for (auto p : productions) {
            s1 = p->left;
            for (auto s2 : p->right) {
                // s2 = j-;
                if (s2->type == ST_TERMINAL) {
                    progress += set_add(s1->firstset, s2);
                    break;
                } else if (s1 == s2) {
                    if (s1->nullable == false) break;
                } else {
                    progress += set_union(s1->firstset, s2->firstset);
                    if (s2->nullable == false) break;
                }
            }
        }
    } while (progress);

    return;
}

/*摘录自虎书
初始化T为{Closure({S1 -> . S$})}
初始化E为空
repeat
    for T中的每一个状态I
        for I中的每一项A->a.Xb
            let J是Goto(I, X)
            T <- T U {J}
            E <- E U { I -X-> J}
until E和T在本轮中没有改变
*/
void LALR1::FindStates() {
    //开始状态
    LR_State start_state;
    LR_Item *item = new LR_Item();
    item->prod = start;
    item->dot = 0;
    //item->forwards.push_back()

    start_state.items.push_back(item);
    //求开始状态的闭包
    Closure(start_state);

    AddLRState(start_state);


    /******/
}

/*
Closure(I) = 
repeat
    for I中的任意项(A->a.XB, z)
        for 任意产生式X->y
            for 任意w in FIRST(Bz)
                I <- I U {(X -> .y, w)}
until I 没有改变
return I
*/
void LALR1::Closure(LR_State &lr_state) {
    int progress;
    do {
        progress = 0;
        //TODO 这种写法会有问题，如果在执行过程中items发生变化，会crash
        //for (auto item : lr_state.items) {
        for(int i = 0; i < lr_state.items.size(); ++i) {
            LR_Item* item = lr_state.items[i];
            if (item->dot >= item->prod->right.size()) continue;
            if (item->prod->right[item->dot]->type == ST_TERMINAL) continue;

            Symbol *symbol = item->prod->right[item->dot];

            for(int j = 0; j < productions.size(); j++) {
                Production* prod = productions[j];
                if(prod->left != symbol) continue;
                LR_Item tt;
                tt.prod = prod;
                tt.dot = 0;
                //forwards中的都是终结符
                //TODO 弄清楚
                if (item->forwards.size() != 0) {
                    for (int m = 0; m < item->forwards.size(); m++) {
                        for (int n = 0; n < item->forwards[m]->firstset.size(); n++) {
                            tt.forwards.push_back(item->forwards[m]->firstset[n]);
                            progress += AddItem(lr_state, tt);
                            tt.forwards.clear();
                        }
                    }
                }
                if (item->dot < item->prod->right.size() - 1) {
                    for (int m = 0; m < item->prod->right[item->dot + 1]->firstset.size(); m++) {
                        tt.forwards.push_back(item->prod->right[item->dot + 1]->firstset[m]);
                        progress += AddItem(lr_state, tt);
                        tt.forwards.clear();
                    }
                }
                //progress += AddItem(lr_state, tt);
            }
        }
    } while (progress);
    return;
}

//添加项目到状态中。只有前看符号不同，则合并前看符号
int LALR1::AddItem(LR_State &lr_state, LR_Item &item) {
    for (auto i : lr_state.items) {
        LR_Item &ii = *i;
        //完全一样则返回
        if (IsSameItem(ii, item)) return 0;

        //如果前看符号不同则合并前看符号
        if (ii.prod == item.prod && ii.dot == item.dot) {
            return set_union(ii.forwards, item.forwards);
        }
    }

    //插入新的项目
    LR_Item *new_item = new LR_Item();
    *new_item = item;
    lr_state.items.push_back(new_item);
    return 1;
}

//判断两个项目是否相同
bool LALR1::IsSameItem(LR_Item &item1, LR_Item &item2) {
    if (item1.prod != item2.prod || item1.dot != item2.dot || item1.forwards.size() != item2.forwards.size())
        return false;
    for (int i = 0; i < item1.forwards.size(); i++) {
        if (item1.forwards[i] != item2.forwards[i]) return false;
    }
    return true;
}

int LALR1::AddLRState(LR_State& state) {
    for(auto s : states) {
        LR_State& ss = *s;
        //是否可以合并
        if(IsMergeLRState(ss, state)) {
            //state.
            if(IsSameLRState(ss, state)) return 0;
            return MergeLRState(ss, state);
        }
    }

    state.index = states.size();
    LR_State* new_state = new LR_State();
    *new_state = state;
    states.push_back(new_state);
    return 1;
}

bool LALR1::IsMergeLRState(LR_State& s1, LR_State& s2) {
   if(s1.items.size() != s2.items.size()) return false;
   for(int i = 0; i < s1.items.size(); i++) {
       LR_Item* i1 = s1.items[i];
       LR_Item* i2 = s2.items[i];

       if(i1->prod != i2->prod || i1->dot != i2->dot) return false;
   }
   return true;

}

bool LALR1::IsSameLRState(LR_State& s1, LR_State& s2) {
   if(s1.items.size() != s2.items.size()) return false;
   for(int i = 0; i< s1.items.size(); i++) {
       if(!IsSameItem(*(s1.items[i]), *(s2.items[i]))) return false;
   }
   return true;
}

int LALR1::MergeLRState(LR_State& s1, LR_State& s2) {
    int count = 0;
    for(int i = 0; i < s1.items.size(); i++) {
        LR_Item* i1 = s1.items[i];
        LR_Item* i2 = s2.items[i];
        count += set_union(i1->forwards, i2->forwards);
    }
    return count;
}