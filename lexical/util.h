#pragma once
#include<string>
#include<map>

using namespace std;

class Location
{
public:
	Location() :line(1), column(0) {}
	int line;
	int column;
};

extern void mapInit();
extern map<string, string> punctuatorType;
extern string keyword[];