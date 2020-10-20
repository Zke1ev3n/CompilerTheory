import sys
from graphviz import Digraph

operator_priority_map = {
  ')': 1, 
  '|': 2,
  '.': 2,
  '(': 3,
}

def is_higher_pri(a, b):
    return operator_priority_map[a] > operator_priority_map[b]

def insert_concatenation(regex):
    temp = regex
    j = 1
    for i in range(1, len(temp)):
        if temp[i] not in operator_priority_map or temp[i] == '(':
            if not (temp[i - 1] == '|' or temp[i - 1] == '('):
                regex = regex[:j] + '.' + regex[j + 1:]
                j+=1
        regex = regex[:j] + temp[i] + regex[j + 1:]
        j+=1
    return regex

def parse_postfix(regex):
    contact_regex = insert_concatenation(regex)
    postfix_regex = ""
    operator_stack = []
    j = 0
    for c in contact_regex:
        if c in operator_priority_map:
            while(operator_stack):
               op2 = operator_stack[-1]
               if op2 == '(' or is_higher_pri(c, op2):
                   break
               else:
                   postfix_regex = postfix_regex[:j] + op2 + postfix_regex[j+1:]
                   j += 1
                   operator_stack.pop()
            if c == ')':
                if operator_stack[-1] != '(':
                    print("Invalid Infix expression: no matching '(' before ')'") 
                    exit()
                operator_stack.pop()
            else:
                operator_stack.append(c)
        else:
            postfix_regex = postfix_regex[:j] + c + postfix_regex[j+1:]
            j+=1
    while(operator_stack):
        postfix_regex = postfix_regex[:j] + operator_stack[-1] + postfix_regex[j+1:]
        j+=1
        operator_stack.pop()

    return postfix_regex

class nfa_link():
    def __init__(self, link):
        self.link = link

class nfa_state():
    def __init__(self, char_in, out, out1):
        self.char_in = char_in
        self.out = nfa_link(out)
        self.out1 = nfa_link(out1)

class nfa_fragment():
    def __init__(self, start_state, out_ref_list):
        self.start_state = start_state
        self.out_ref_list = out_ref_list

all_nfa_states = []
all_nfa_fragments = []

def new_nfa_state(char_in, out, out1):
    x = nfa_state(char_in, out, out1)
    all_nfa_states.append(x)
    return x

def new_nfa_fragment_ref_list(start_state, out_ref_list):
    x = nfa_fragment(start_state, out_ref_list)
    all_nfa_fragments.append(x)
    return x

def new_nfa_fragment_state(start_state, out_state_ref):
    out_ref_list = []
    out_ref_list.append(out_state_ref)
    x = new_nfa_fragment_ref_list(start_state, out_ref_list)
    all_nfa_fragments.append(x)
    return x

def frag_connect(out1, in2):
    for i in range(len(out1)):
        out1[i].link = in2

def compile_regex(regex):
    regex = regex.replace('&', '')
    postfix_regex = parse_postfix(regex)
    nfa_stack = []
    for c in postfix_regex:
        if c == "|":
            frag2 = nfa_stack[-1] 
            nfa_stack.pop()
            frag1 = nfa_stack[-1]
            nfa_stack.pop()
            state = new_nfa_state("", frag1.start_state, frag2.start_state)
            v = frag1.out_ref_list[:]
            v.extend(frag2.out_ref_list)
            frag3 = new_nfa_fragment_ref_list(state, v)
            nfa_stack.append(frag3)
        elif c == ".":
            frag2 = nfa_stack[-1]
            nfa_stack.pop()
            frag1 = nfa_stack[-1]
            nfa_stack.pop()
            frag_connect(frag1.out_ref_list, frag2.start_state)
            frage3 = new_nfa_fragment_ref_list(frag1.start_state, frag2.out_ref_list)
            nfa_stack.append(frage3)
        else:
            state = new_nfa_state(c, None, None)
            frag = new_nfa_fragment_state(state, state.out)
            nfa_stack.append(frag)
    
    nfa = nfa_stack[-1]
    nfa_stack.pop()
    fisrt_state = new_nfa_state("start", nfa.start_state, None)
    final_state = new_nfa_state("end", None, None)
    frag_connect(nfa.out_ref_list, final_state)
    return fisrt_state

def print_nfa(name):
    dot = Digraph(filename=name, comment=name, format='png', graph_attr={'rankdir': 'LR'}, node_attr={'shape': 'box'})
    for state in all_nfa_states:
        dot.node(str(id(state)), state.char_in)
    for state in all_nfa_states:
        if state.out.link != None:
            dot.edge(str(id(state)), str(id(state.out.link)))
        if state.out1.link != None:
            dot.edge(str(id(state)), str(id(state.out1.link)))
    dot.view()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("need expression!")
        exit()
    regex = sys.argv[1]
    compile_regex(regex)
    print_nfa(regex)