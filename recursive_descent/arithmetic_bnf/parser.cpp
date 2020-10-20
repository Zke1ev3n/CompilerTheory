#include "parser.h"

Parser::Parser() :
   line(),
   pos(0) {

   std::cout << "Enter expression." << std::endl;
   // Get expression and remove spaces
   // Assumes valid expression, does not check input
   // Does not handle expressions with non-single digits
   std::getline(std::cin, line);
   line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
}

void Parser::parse() {
   std::cout << expression() << std::endl;
}

char Parser::nextChar() {
   return line[pos++];
}

void Parser::previousChar() {
   pos--;
}

// Checks for parentheses and returns int values for calculations
double Parser::factor() {
   double tempVal = 0;
   char tempChar = nextChar();
   // Parentheses have the highest priority when parsing
   if(tempChar == '(') {
      // Start checking expression again
      tempVal = expression();
      tempChar = nextChar();
   }
   else if (std::isdigit(tempChar)) {
      // Have to convert ascii to int to keep correct value
      tempVal = tempChar - '0';
   }
   return tempVal;
}

// First calculations occur here as multiplication and division go before addition and subtraction
double Parser::term() {
   double curVal = factor();
   char tempChar = nextChar();
   // Multiplication and Division have the second highest priorit
   if(tempChar == '*' || tempChar == '/') {
      double tempVal = term();
      if(tempChar == '*') {
         curVal *= tempVal;
      }
      else {
         curVal /= tempVal;
      }
   }
   else {
      previousChar();
   }
   return curVal;
}

// Starts evaluating expression character by character using recursive desent parsing
// Also hold lowest priority on tokens
double Parser::expression() {
   double curVal = term();
   char tempChar = nextChar();
   // Addition and Subtraction has the lowest priority so they are done last
   if(tempChar == '-' || tempChar == '+') {
      double tempVal = expression();
      if(tempChar == '+') {
         curVal += tempVal;
      }
      else {
         curVal -= tempVal;
      }
   }
   else {
      previousChar();
   }
   return curVal;
}
