import sys
from graphviz import Digraph

class FA(object):
    def __init__(self, filename):
        self.states = []
        self.start_state = 0
        self.alphabets = []
        self.accept_states = []
        self.transitions = {}

        with open(filename, 'r') as f:
            state_amount = f.readline()
            states = f.readline()
            self.states = [int(i) for i in states.split()]
            alphabet_amount = f.readline()
            alphabets = f.readline()
            self.alphabets = [i for i in alphabets.split()]
            self.start_state = int(f.readline())
            accept_amount = f.readline()
            accept_states = f.readline()
            self.accept_states = [int(i) for i in accept_states.split()]
            transition_amount = int(f.readline())
            for i in range(0, transition_amount):
                trans = f.readline().split()
                key = (trans[0], trans[2])
                if key in self.transitions:
                    self.transitions[key].add(trans[1])
                else:
                    self.transitions[key] = {trans[1]}
            

    def print(self, name):
        dot = Digraph(filename=name, comment=name, format='png', graph_attr={'rankdir': 'LR'})
        for state in self.states:
            if state in self.accept_states:
                dot.node(str(state), shape='doublecircle')
            else:
                dot.node(str(state), shape='circle')
        for k in self.transitions:
                for to in self.transitions[k]:
                    print(k[0] + " " + to + " " + k[1])
                    dot.edge(str(k[0]), str(to), label=k[1])

        dot.view()
        pass

if __name__ == "__main__":
    if len(sys.argv) < 2:
        exit()
    file_name = sys.argv[1]
    fa = FA(file_name)
    fa.print("test")
