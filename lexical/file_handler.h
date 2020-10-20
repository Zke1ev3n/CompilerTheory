#pragma once
#include "../include/Token.h"
#include<fstream>

using namespace std;

class FileHandler
{
private:
	ifstream file;
	Location location;
	bool eof;
	bool flag;
	bool isPreprocessor;
	int charCount;
	string buffer;
	string fileName;
public:
	FileHandler(string _fileName) :flag(true), fileName(_fileName), isPreprocessor(true), charCount(0)
	{
		file.open(_fileName, ios::in);
		eof = file.eof();
		status = file.fail() || eof ? false : true;
	}

	Location getLocation()
	{
		return location;
	}

	void printFileInfo(ofstream& of)
	{
		of << "File Name: " << fileName << endl;
		of << "Total Characters: " << charCount << endl;
		of << "Total Lines: " << location.line << endl;
	}

	char getChar()
	{
		if (isEOF())
			return 0;
		else if (buffer.empty())
			readChar();
		char c = buffer.front();
		buffer.erase(buffer.begin());
		charCount++;
		if (c != '\t')
			location.column++;
		else
			location.column += 4;
		isPreprocessor = (c == '#' && flag) ? true : false;
		if (c == '\n')
		{
			location.line++;
			location.column = 0;
			flag = true;
		}
		else if (c != '\t')
			flag = false;
		return c;
	}

	string getNChar(int n)
	{
		string temp = "";
		char c;
		for (int i = 0; i < n; i++)
		{
			if ((c = getChar()) == 0)
				break;
			temp += c;
		}
		return temp;
	}

	bool isPre()
	{
		return isPreprocessor;
	}

	void readChar()
	{
		if (!eof)
		{
			buffer += file.get();
			eof = file.eof();
		}
	}

	char peek(int offset)
	{
		while (!isEOF() && buffer.size() < offset)
			readChar();
		if (buffer.size() < offset)
		{
			cout << "There is something wrong with the operation" << endl;
			return 0;
		}
		else
			return buffer[offset - 1];
	}

	bool isEOF()
	{
		return eof && buffer.empty();
	}

	bool status;
};