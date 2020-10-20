#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <algorithm>


class Parser{
private:
   std::string line;
   int pos;

   char nextChar();
   void previousChar();

   double factor();
   double term();
   double expression();

public:
   Parser();

   void parse();
};

#endif
