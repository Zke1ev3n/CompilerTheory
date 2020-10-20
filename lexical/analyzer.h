#pragma once
#include "file_handler.h"
#include <sstream>

using namespace std;
class Analyzer
{
private:
	FileHandler fileHandler;
	ofstream result;
	string buffer;
	string fileName;
	Location currentLoc;
	int keywordCnt;
	int identifierCnt;
	int constantCnt;
	int stringLiteralCnt;
	int punctuatorCnt;
	int preprocessorCnt;
	int unknownCnt;
	int errorCnt;

	void skipLineComments()
	{
		while (!fileHandler.isEOF() && fileHandler.getChar() != '\n');
	}

	void skipBlockComments()
	{
		while (!fileHandler.isEOF())
		{
			if (fileHandler.getChar() == '*' && fileHandler.peek(1) == '/')
			{
				fileHandler.getChar();
				return;
			}
		}
	}

	bool isSimpleEscape(char c)
	{
		return c == '\'' || c == '?' || c == '\"' || c == '\\' || c == 'a'
			|| c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' || c == 'v';
	}

	bool isHexaDigit(char c)
	{
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
	}

	bool isConstantChar(char c)
	{
		return isdigit(c) || c == '.' || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') ||
			c == 'x' || c == 'X' || c == 'e' || c == 'E' || c == 'p' || c == 'P"' ||
			c == 'u' || c == 'U' || c == 'l' || c == 'L';
	}

	bool isSuffix(size_t pos)
	{
		bool zero = buffer.size() == pos;
		bool one = (buffer.size() == pos + 1) &&
			((buffer[pos] == 'u' || buffer[pos] == 'U' && buffer.find_first_of(".eEpP") == string::npos)
				|| buffer[pos] == 'l' || buffer[pos] == 'L'
				|| (buffer[pos] == 'f' || buffer[pos] == 'F' && buffer.find_first_of(".eEpP") != string::npos));
		bool two = (buffer.size() == pos + 2) &&
			((buffer[pos] == 'u' && buffer[pos + 1] == 'l') || (buffer[pos] == 'U' && buffer[pos + 1] == 'l')
				|| (buffer[pos] == 'u' && buffer[pos + 1] == 'L') || (buffer[pos] == 'U' && buffer[pos + 1] == 'L'));
		return zero || one || two;
	}

	Token getStringLiteral();
	Token getCharacterConstant();
	Token getIdentifier();
	Token getNumericConstant();

public:
	bool status;
	int no;

	Analyzer(const string _fileName, const int i)
		:fileName(_fileName), fileHandler(_fileName), no(i), buffer(""),
		keywordCnt(0), identifierCnt(0), constantCnt(0), stringLiteralCnt(0),
		punctuatorCnt(0), preprocessorCnt(0), unknownCnt(0), errorCnt(0)
	{
		if (!fileHandler.status)
			status = false;
		else
		{
			status = true;
			int pos = fileName.rfind('/');
			stringstream ss;
			ss << i;
			string s = "result" + ss.str() + ".txt";
			string resultName = (pos == string::npos) ? s : fileName.substr(0, pos + 1) + s;
			result.open(resultName, ios::out);
		}
	}

	Token getToken();
	
	bool printToken(Token token)
	{
		result << token;
		result << "Location: " << fileName << ":" << token.location.line
			<< ":" << token.location.column << endl << endl;
		return fileHandler.isEOF() ? true : false;
	}

	void printInfo()
	{
		result << "Result:" << endl;
		fileHandler.printFileInfo(result);
		result << "Keyword: " << keywordCnt << endl;
		result << "Identifier: " << identifierCnt << endl;
		result << "Punctuator: " << punctuatorCnt << endl;
		result << "Constant: " << constantCnt << endl;
		result << "String Literal: " << stringLiteralCnt << endl;
		result << "Preprocessor: " << preprocessorCnt << endl;
		result << "Unknown: " << unknownCnt << endl;
		result << "Error: " << errorCnt << endl;
	}
};

