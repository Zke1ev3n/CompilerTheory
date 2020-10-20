#pragma once 
using namespace std;
#include "../include/Util.h"
#include <iostream>

class Token
{
public:
	string type;
	string subtype;
	string value;
	Location location;
	Token(string _type, string _subtype, string _value, Location _location) :
		type(_type), subtype(_subtype), value(_value), location(_location) {}
	Token(string _type, string _value, Location _location) :
		type(_type), subtype(""), value(_value), location(_location) {}
	Token(string _type, Location _location) :
		type(_type), subtype(""), value(""), location(_location) {}
	friend ostream& operator << (ostream& of, const Token& token);
};

