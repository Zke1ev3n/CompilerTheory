SOURCES = main.cc dfa.cc mfa.cc
MAIN = mfa

$(MAIN): $(SOURCES)
	g++ -std=c++11 $^ -Wall --pedantic -o $@

clean:
	rm -f $(MAIN) *.o *.zip

.PHONY: clean