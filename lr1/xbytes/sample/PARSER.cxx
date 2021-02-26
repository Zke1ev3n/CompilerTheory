#include "PARSER.hxx"
#include <iostream>
namespace xbytes {

const char* symbol_table[] =
{
/*   0*/"$",
/*   1*/"MINUS",
/*   2*/"PLUS",
/*   3*/"TIMES",
/*   4*/"DIV",
/*   5*/"INT",
/*   6*/"program",
/*   7*/"exp",
};

const rule rule_table[] =
{
/*   0*/{"program -> exp(A) $", 6, 2},
/*   1*/{"exp(A) -> exp(B) MINUS exp(C)", 7, 3},
/*   2*/{"exp(A) -> exp(B) PLUS exp(C)", 7, 3},
/*   3*/{"exp(A) -> exp(B) TIMES exp(C)", 7, 3},
/*   4*/{"exp(A) -> exp(B) DIV exp(C)", 7, 3},
/*   5*/{"exp(A) -> INT(B)", 7, 1},
};

const action action_table[] =
{
/*   0*/{5,0},{5,0},{5,0},{5,0},{5,0},{2,2},{5,0},{1,1},
/*   1*/{4,0},{2,3},{2,4},{2,5},{2,6},{5,0},{5,0},{5,0},
/*   2*/{3,5},{3,5},{3,5},{3,5},{3,5},{5,0},{5,0},{5,0},
/*   3*/{5,0},{5,0},{5,0},{5,0},{5,0},{2,2},{5,0},{1,7},
/*   4*/{5,0},{5,0},{5,0},{5,0},{5,0},{2,2},{5,0},{1,8},
/*   5*/{5,0},{5,0},{5,0},{5,0},{5,0},{2,2},{5,0},{1,9},
/*   6*/{5,0},{5,0},{5,0},{5,0},{5,0},{2,2},{5,0},{1,10},
/*   7*/{3,1},{3,1},{3,1},{2,5},{2,6},{5,0},{5,0},{5,0},
/*   8*/{3,2},{3,2},{3,2},{2,5},{2,6},{5,0},{5,0},{5,0},
/*   9*/{3,3},{3,3},{3,3},{3,3},{3,3},{5,0},{5,0},{5,0},
/*  10*/{3,4},{3,4},{3,4},{3,4},{3,4},{5,0},{5,0},{5,0},
};

parser::parser()
: xstack(),
  symbol_id_map()
{
	initialize();
}

parser::~parser()
{
}

void parser::initialize()
{
	//init symbol_id_map
	for(int i = 0; i < TERMINAL_SYMBOL_COUNT; ++i)
	{
		const string& s = symbol_table[i];
		symbol_id_map[s] = i;
	}

	//init stack
	stack_state ss;
	ss.state = 0;
	ss.symbol = $;//$

	xstack.push(ss);
}

TOKEN_TYPE parser::execute_reduce_code(int rule_index, token_vector& tv)
{
	TOKEN_TYPE tt;
	if(tv.size() > 0) tt = tv[0];
	switch(rule_index)
	{
	case 0:/* program -> exp(A). */
{ std::cout << "Result=" << tv[0] << std::endl; }
	break;

	case 1:/* exp(A) -> exp(B) MINUS exp(C). */
{ tt = tv[0] - tv[2]; std::cout << tt << "=" << tv[0] << "-" << tv[2] << std::endl; }
	break;

	case 2:/* exp(A) -> exp(B) PLUS exp(C). */
{ tt = tv[0] + tv[2]; std::cout << tt << "=" << tv[0] << "+" << tv[2] << std::endl; }
	break;

	case 3:/* exp(A) -> exp(B) TIMES exp(C). */
{ tt = tv[0] * tv[2]; std::cout << tt << "=" << tv[0] << "*" << tv[2] << std::endl; }
	break;

	case 4:/* exp(A) -> exp(B) DIV exp(C). */
{
    if(tv[2] != 0)
    {
        tt = tv[0] / tv[2];
    }
    else
    {
        std::cout << "Divide by zero." << std::endl;
    }

    std::cout << tt << "=" << tv[0] << "/" << tv[2] << std::endl;
}
	break;

	case 5:/* exp(A) -> INT(B). */
{ tt = tv[0]; std::cout << tt << "=" << tv[0] << std::endl;}
	break;

	}

	return tt;
}

void parser::eat(int tt, TOKEN_TYPE t)
{
	while(xstack.size() > 0)
	{
		stack_state& ss = xstack.top();
		int action_index = SYMBOL_COUNT * ss.state + tt;
		if(action_index >= sizeof(action_table))
		{
 std::cout << "Error: Syntax error.\n" << std::endl;
			break;
		}

		const action& act = action_table[action_index];
		if(act.type == LR_ACTION_SHIFT)
		{
			stack_state nss;
			nss.state = act.target;
			nss.symbol = tt;
			nss.t = t;
			xstack.push(nss);
			break;
		}
		else if(act.type == LR_ACTION_REDUCE)
		{
			const rule& r = rule_table[act.target];
			token_vector tv(r.right_count);
			for(int i = 0; i < r.right_count; ++i)
			{
				stack_state& pss = xstack.top();
				tv[r.right_count - 1 - i] = pss.t;
				xstack.pop();				
			} 

			TOKEN_TYPE rct = execute_reduce_code(act.target, tv);

			ss = xstack.top();
			const action& goto_act = action_table[r.left + SYMBOL_COUNT * ss.state];
			if(goto_act.type != LR_ACTION_GOTO)
			{
 std::cout << "Error: Syntax error.\n" << std::endl;
				break;
			}

			stack_state nss;
			nss.state = goto_act.target;
			nss.symbol = r.left;
			nss.t = rct;
			xstack.push(nss);
		}
		else if(act.type == LR_ACTION_ACCEPT)
		{
			const rule& r = rule_table[act.target];
			token_vector tv;
			tv.push_back(ss.t);
			xstack.pop();

			execute_reduce_code(act.target, tv);
			break;
		}
		else
		{
 std::cout << "Error: Syntax error.\n" << std::endl;
			break;
		}
	}	
}

}//namespace xbytes

