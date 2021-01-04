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
        //这里要为增广文法预留一条
        production->index = productions.size() + 1;
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
    start->index = 0;
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
            if(j->dot == j->prod->right.size()) cout<<" •";
            cout<<" [";
            for(auto m : j->forwards) {
                cout<<" "<<m->name;
            }
            cout<<" ] "<< endl;
        }
    }

    cout << "---------------------------" << endl;
    cout << "lr action table:" << endl;

    cout << "STATE 	";
	for(int i = 0; i < symbols.size(); ++i)
	{
		cout << symbols[i]->name << "	";
	}
	cout << std::endl;

    for(int i = 0; i < states.size(); ++i)
	{
		std::cout << i << "	";
		for(int j = 0; j < states[i]->actions.size(); ++j)
		{
			int valid_action_count = 0;
			vector<Action>& aa = states[i]->actions[j];
			for(int m = 0; m < aa.size(); ++m)
			{
				assert(valid_action_count <= 1);
				Action& act = aa[m];
				std::stringstream ss;
				switch(act.type)
				{
				case LR_ACTION_REDUCE:
					ss << "R" << act.prod->index << "	";
					valid_action_count++;
					break;
				case LR_ACTION_SHIFT:
					ss << "S" << act.next_state << "	";
					valid_action_count++;
					break;
				case LR_ACTION_GOTO:
					ss << "G" << act.next_state << "	";
					valid_action_count++;
					break;
				case LR_ACTION_ACCEPT:
					ss << "accept" << "	";
					valid_action_count++;
					break;
				case LR_ACTION_ERROR:
					ss << "-	";
					valid_action_count++;
				}

				cout << ss.str();
			}
		}
		cout << std::endl;
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

//符号是否在set中
bool is_symbol_in_set(Symbol* sym, vector<Symbol*> vec) {
    for(int i = 0; i < vec.size(); i++) {
        if(vec[i] == sym) return true;
    }

    return false;
}

/*
摘录自虎书
FIRST、FOLLOW、nullable计算算法：

将所有FIRST,FOLLOW,nullable设置为空
for 每一个终结符Z
	FIRST[Z] = {Z}

repeat
	for 每个产生式 X->Y1 Y2 ... Yk
		for 每个i从1到k 每个j从i+1到k
			if 所有Yi都可为空的 then
				nullable[X] = true
			if Y1 ... Yi-1都可为空的 then
				FIRST[X] = FIRST[X] U FIRST[Yi]
			if Yi+1 ... Yk 都可为空的 then
				FOLLOW[Yi] = FOLLOW[Yi] U FOLLOW[X]
			if Yi+1 .. Yj-1 都可为空的 then
				FOLLOW[Yi] = FOLLOW[Yi] U FIRST[Yj]
			if Yi+1是终结符并且Yi是非终结符
				FOLLOW[Yi] = FOLLOW[Yi] U Yi
			if Yi+1是非终结符并且Yi也是非终结符
				FOLLOW[Yi] = FOLLOW[Yi] U FIRST[Yi+1]
until FIRST,FOLLOW,nullable都没有变化
*/
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
    //这里可以不用加$
    //item->forwards.push_back()

    start_state.items.push_back(item);
    //求开始状态的闭包
    Closure(start_state);

    AddLRState(start_state);

    int progress = 1;
    do{
        progress = 0;
        for(int i = 0; i < states.size(); i++) {
            LR_State* state = states[i];
            for(int j = 0; j < state->items.size(); j++) {
                LR_Item* item = state->items[j];
                if(item->dot >= item->prod->right.size()) continue;

                Symbol* symbol = item->prod->right[item->dot];
                if(symbol->name == "$") continue;
            
                LR_State nstate = LRGoto(*state, symbol);
                //这里合并了同心项目集，状态数会少于LR(1)
                progress += AddLRState(nstate);
                progress += AddLRGoto(*state, symbol, nstate.index);
            }
        }
    }while(progress);
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
                //TODO 这种逻辑导致forwards中的集合比较大
                // if (item->forwards.size() != 0) {
                //     for (int m = 0; m < item->forwards.size(); m++) {
                //         for (int n = 0; n < item->forwards[m]->firstset.size(); n++) {
                //             tt.forwards.push_back(item->forwards[m]->firstset[n]);
                //             progress += AddItem(lr_state, tt);
                //             tt.forwards.clear();
                //         }
                //     }
                // }
                // if (item->dot < item->prod->right.size() - 1) {
                //     for (int m = 0; m < item->prod->right[item->dot + 1]->firstset.size(); m++) {
                //         tt.forwards.push_back(item->prod->right[item->dot + 1]->firstset[m]);
                //         progress += AddItem(lr_state, tt);
                //         tt.forwards.clear();
                //     }
                // }
                if (item->dot < item->prod->right.size() - 1) {
                    for (int m = 0; m < item->prod->right[item->dot + 1]->firstset.size(); m++) {
                        tt.forwards.push_back(item->prod->right[item->dot + 1]->firstset[m]);
                    }
                    if (item->prod->right[item->dot + 1]->nullable) {
                        //forwards都是终结符
                        for (int n = 0; n < item->forwards.size(); n++) {
                            tt.forwards.push_back(item->forwards[n]->firstset[0]);
                        }
                    }
                }else{
                    //dot在最后
                    for (int n = 0; n < item->forwards.size(); n++) {
                            tt.forwards.push_back(item->forwards[n]->firstset[0]);
                    }
                }
                progress += AddItem(lr_state, tt);
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
        //合并同心项目集
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

/*
Goto(I,X) =
    J <- {}
    for I中的任意项(A -> a.Xb, z)
        将(A->aX.b, z)加入到J中
    return Closure(J)
*/    
LR_State LALR1::LRGoto(LR_State& state, Symbol* sym) {
    LR_State next_state;
    for(int i = 0; i < state.items.size(); i++) {
        LR_Item* item = state.items[i];
        if(item->dot >= item->prod->right.size()) continue;

        if(item->prod->right[item->dot] == sym) {
            LR_Item tt;
            tt.prod = item->prod;
            tt.dot = item->dot + 1;
            tt.forwards = item->forwards;
            AddItem(next_state, tt);
        }
    }

    Closure(next_state);

    return next_state;
}

int LALR1::AddLRGoto(LR_State& state, Symbol* sym, int index) {
    if(state.goto_map.count(sym) > 0) return 0;

    state.goto_map[sym] = index;

    return index;
}


/*
R <- {}
for T中的每一个状态I
    for I中的每一个项(A->a. , z)
        R <= R U {(I,z,A->a)}

对于每一条边I-X->J, 若X为终结符，则在位置(I,X)中放移进J(sJ)；
若X为非终结符，则将转换J(gJ)放在位置(I,X)中。
对于包含项S1->S.$的每个状态I,我们再位置(I,$)中放置接受(a).
最后，对于包含项A->y.(尾部有圆点的产生式n)的状态，
对每一个单词Y, 放置规约n(rn)于(I,Y)中。
*/
void LALR1::MakeLALR1Table() {

    for(int i = 0; i< states.size(); i++) {
        LR_State* state = states[i];
        //行数，symbol的个数
        state->actions.resize(symbols.size());

        //goto & shift action
        for(map<Symbol*, int>::iterator it = state->goto_map.begin(); it != state->goto_map.end(); it++) {
            Symbol* sym = it->first;

            Action act;
            act.next_state = it->second;
            act.prod = nullptr;
            act.lookahead = sym;

            if(sym->type == ST_TERMINAL) {
                act.type = LR_ACTION_SHIFT;
            }else if(sym->type == ST_NONTERMINAL) {
                act.type = LR_ACTION_GOTO;
            }

            state->actions[sym->index].push_back(act);
        }

        for(int j = 0; j < state->items.size(); j++) {
            LR_Item* item = state->items[j];
            if(item->dot != item->prod->right.size()) continue;

            for(int k = 0; k < state->actions.size(); k++) {
                if(symbols[k]->type != ST_TERMINAL) continue;
                //FOR LALR
                if(!is_symbol_in_set(symbols[k], item->forwards)) continue;
                //FOR SLR
                //if(!is_symbol_in_set[symbols[j], item->prod->left->followset) continue;

                Action act;
                act.type = LR_ACTION_REDUCE;
                act.next_state = 0;
                act.prod = item->prod;
                act.lookahead = symbols[k];

                state->actions[k].push_back(act);
            }
        }

       //accept action
       for(int k = 0; k < state->items.size(); k++) {
           LR_Item* item = state->items[k];
           if(item->dot != item->prod->right.size() - 1) continue;

           if(item->prod->right[item->dot]->name == "$") {
               Action act;
               act.prod = item->prod;
               act.type = LR_ACTION_ACCEPT;
               act.next_state = 0;
               act.lookahead = item->prod->right[item->dot];

               state->actions[item->prod->right[item->dot]->index].push_back(act);
           }
       }
    }

   for(int i = 0; i < states.size(); i++) {

       LR_State* state = states[i];
       for(int j = 0; j < state->actions.size(); j++) {
           vector<Action>& aa = state->actions[j];
           if(aa.size() == 0) {
               Action act;
               act.prod = nullptr;
               act.type = LR_ACTION_ERROR;
               act.next_state = 0;
               act.lookahead = nullptr;

               aa.push_back(act);
           }
       }
   }

    //TODO 暂时不支持冲突处理，需要针对语法特殊处理
    //resolve conflict
//    int conflict = 0;
//    for(int i = 0; i < states.size(); i++) {
//        LR_State* state = states[i];
//        for(int j = 0; j < state->actions.size(); j++) {
//            vector<Action>& aa = state->actions[j];
//            if(aa.size() < 2) continue;

//            for(int k = 1; k < aa.size(); k++) {
//                conflict += ResolveConflict(aa[k-1], aa[k]);
//            }
//        }
//    }

//    cout << "conflict: " << conflict << endl;

}

int LALR1::ResolveConflict(Action& a1, Action& a2) {

    int error = 0;
    assert(a1.lookahead == a2.lookahead);

    //两个都是移进
    if(a1.type == LR_ACTION_SHIFT && a2.type == LR_ACTION_SHIFT) {
        a2.type = LR_ACTION_SSCONFLICT;
        error++;
    }

    if(a1.type == LR_ACTION_SHIFT && a2.type == LR_ACTION_REDUCE) {
        Symbol* s1 = a1.lookahead;
        //Symbol* s2 = a2->pr

    }

    return error;

    /*
    int xbytes::resolve_conflict(action& x, action& y)
{
	int error = 0;
	assert(x.lookahead == y.lookahead);

	if(x.type == LR_ACTION_SHIFT && y.type == LR_ACTION_SHIFT)
	{
		y.type = LR_ACTION_SSCONFLICT;
		error++;
	}

	if(x.type == LR_ACTION_SHIFT && y.type == LR_ACTION_REDUCE)
	{
		symbol* xs = x.lookahead;
		symbol* ys = y.r->precedence_symbol;

		if(ys == NULL || xs->precedence < 0 || ys->precedence < 0)
		{
			//Not enough precedence information
			y.type = LR_ACTION_SRCONFLICT;
			error++;
		}
		else if(xs->precedence > ys->precedence)
		{
			y.type = LR_ACTION_RDRESOLVED;
		}
		else if(xs->precedence < ys->precedence)
		{
			x.type = LR_ACTION_SHRESOLVED;
		}
		else if(xs->precedence == ys->precedence && xs->associativity == AT_RIGHT)//Use operator
		{
			y.type = LR_ACTION_RDRESOLVED;//associativity
		}
		else if(xs->precedence == ys->precedence && xs->associativity == AT_LEFT)//to break tie
		{
			x.type = LR_ACTION_SHRESOLVED;
		}
		else
		{
			assert(xs->precedence == ys->precedence && xs->associativity == AT_NONE);
			x.type = LR_ACTION_ERROR;
		}
	}
	else if(x.type = LR_ACTION_REDUCE && y.type == LR_ACTION_REDUCE)
	{
		symbol* xs = x.r->precedence_symbol;
		symbol* ys = y.r->precedence_symbol;

		if(xs == NULL || ys == NULL || xs->precedence < 0 || 
			ys->precedence < 0 || xs->precedence == ys->precedence)
		{
			y.type = LR_ACTION_RRCONFLICT;
			error++;
		}
		else if(xs->precedence > ys->precedence)
		{
			y.type = LR_ACTION_RDRESOLVED;
		}
		else if(xs->precedence < ys->precedence)
		{
			x.type = LR_ACTION_RDRESOLVED;
		}
	}
	else
	{
		assert(
			x.type == LR_ACTION_SHRESOLVED ||
			x.type == LR_ACTION_RDRESOLVED ||
			x.type == LR_ACTION_SSCONFLICT ||
			x.type == LR_ACTION_SRCONFLICT ||
			x.type == LR_ACTION_RRCONFLICT ||
			y.type == LR_ACTION_SHRESOLVED ||
			y.type == LR_ACTION_RDRESOLVED ||
			y.type == LR_ACTION_SSCONFLICT ||
			y.type == LR_ACTION_SRCONFLICT ||
			y.type == LR_ACTION_RRCONFLICT
			);
	}

	return error;
}
*/

}

//处理优先级
void LALR1::FindSymbolPrecedence() {


}
/*
void xbytes::find_symbol_precedence()
{
	//make symbol's precedence & associativity
	for(int i = 0; i < assoc_precs.size(); ++i)
	{
		int ass = assoc_precs[i].associativity;
		string_array& sa = assoc_precs[i].symbol_names;
		for(int m = 0; m < sa.size(); ++m)
		{
			symbol* s = find_symbol(sa[m]);
			if(s)
			{
				s->associativity = ass;
				s->precedence = i;
			}
		}
	}	
}
void xbytes::find_rule_precedence()
{


	//make rule's precedence
	for(int i = 0; i < rules.size(); ++i)
	{
		rule* r = rules[i];
		if(r->precedence_symbol != NULL) continue;

		for(int m = 0; m < r->rights.size() && r->precedence_symbol == NULL; ++m)
		{
			symbol* s = r->rights[m];
			if(s->type != ST_TERMINAL) continue;
			
			if(s->precedence >= 0)
			{
				r->precedence_symbol = s;
				break;
			}
		}
	}
}
*/