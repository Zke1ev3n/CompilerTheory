#include "analyzer.h"
#include<algorithm>

Token Analyzer::getToken()
{
	buffer = "";
	char next;
	if (fileHandler.isEOF())
		return Token("EOF", fileHandler.getLocation());

	char c = fileHandler.getChar();
	while (c == ' ' || c == '\t' || c == '\n')
		c = fileHandler.getChar();

	if (fileHandler.isEOF())
		return Token("EOF", fileHandler.getLocation());

	currentLoc = fileHandler.getLocation();

	switch (c)
	{
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
	case 'I': case 'J': case 'K': /*case 'L': */case 'M': case 'N': case 'O': case 'P':
	case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
	case 'Y': case 'Z': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
	case 'w': case 'x': case 'y': case 'z': case '_':
		buffer += c;
		return getIdentifier();

	case 'L':
		next = fileHandler.peek(1);
		if (next == '\'' || next == '\"')
			return getToken();
		else
			buffer += c;
		return getIdentifier();

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		buffer += c;
		return getNumericConstant();

	case '[': case ']': case '{': case '}': case '(':
	case ')': case ',': case ';': case '?': case '~':
		buffer += c;
		punctuatorCnt++;
		return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);

	case '+': case '|': case '&': case '=':
	case '-': case '*': case '!': case '^':
		buffer += c;
		punctuatorCnt++;
		next = fileHandler.peek(1);
		if (next == '=' || (next == c && c != '*' && c != '!' && c != '^') || (c == '-' && next == '>'))
		{
			buffer += fileHandler.getChar();
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);

	case '/':
		if (fileHandler.peek(1) == '/')
		{
			fileHandler.getChar();
			skipLineComments();
			return getToken();
		}
		else if (fileHandler.peek(1) == '*')
		{
			fileHandler.getChar();
			skipBlockComments();
			return getToken();
		}
		else if (fileHandler.peek(1) == '=')
		{
			buffer += c;
			buffer += fileHandler.getChar();
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else
		{
			buffer += c;
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}

	case '%':
		buffer += c;
		punctuatorCnt++;
		if (fileHandler.peek(1) == '=' || fileHandler.peek(1) == '>')
		{
			buffer += fileHandler.getChar();
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else if (fileHandler.peek(1) == ':')
		{
			if (fileHandler.peek(2) == '%' && fileHandler.peek(3) == ':')
			{
				buffer += fileHandler.getNChar(3);
				return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
			}
			else
			{
				buffer += fileHandler.getChar();
				return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
			}
		}
		else
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);

	case '<': case '>':
		buffer += c;
		punctuatorCnt++;
		if (fileHandler.peek(1) == c)
		{
			buffer += fileHandler.getChar();
			if (fileHandler.peek(2) == '=')
			{
				buffer += fileHandler.getChar();
				return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
			}
			else
				return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else if (fileHandler.peek(1) == '=' || (fileHandler.peek(1) == ':' ||
			fileHandler.peek(1) == '%' && c == '<'))
		{
			buffer += fileHandler.getChar();
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);

	case '.':
		buffer += c;
		if (fileHandler.peek(1) == '.' && fileHandler.peek(2) == '.')
		{
			buffer += fileHandler.getNChar(2);
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else if (isdigit(fileHandler.peek(1)))
			return getNumericConstant();
		else
		{
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}


	case '#':
		buffer += c;
		if (fileHandler.peek(1) == '#')
		{
			buffer += fileHandler.getChar();
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else if (fileHandler.isPre())
		{
			while (!fileHandler.isEOF() && (fileHandler.peek(1) != '\n' || c == '\\'))
				buffer += fileHandler.getChar();
			preprocessorCnt++;
			return Token("Preprocessor", buffer, currentLoc);
		}
		else
		{
			punctuatorCnt++;
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}

	case ':':
		buffer += c;
		punctuatorCnt++;
		if (fileHandler.peek(1) == '>')
		{
			buffer += fileHandler.getChar();
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);
		}
		else
			return Token("Punctuator", punctuatorType[buffer], buffer, currentLoc);

	case '\'':
		buffer += c;
		return getCharacterConstant();

	case '\"':
		buffer += c;
		return getStringLiteral();

	default:
		buffer += c;
		unknownCnt++;
		return Token("Unknown", buffer, currentLoc);
	}
}

Token Analyzer::getStringLiteral()
{
	while (!fileHandler.isEOF())
	{
		if (fileHandler.peek(1) != '\\' && fileHandler.peek(1) != '\"')
			buffer += fileHandler.getChar();
		else if (fileHandler.peek(1) == '\\')
		{
			if (isSimpleEscape(fileHandler.peek(2)))
				buffer += fileHandler.getNChar(2);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7'
				&& fileHandler.peek(3) >= '0' && fileHandler.peek(3) <= '7'
				&& fileHandler.peek(4) >= '0' && fileHandler.peek(4) <= '7')
				buffer += fileHandler.getNChar(4);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7'
				&& fileHandler.peek(3) >= '0' && fileHandler.peek(3) <= '7')
				buffer += fileHandler.getNChar(3);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7')
				buffer += fileHandler.getNChar(2);
			else if (fileHandler.peek(2) == 'x')
			{
				if (!isHexaDigit(fileHandler.peek(3)))
				{
					buffer += fileHandler.getNChar(3);
					while (!fileHandler.isEOF() && fileHandler.peek(1) != '\"')
						buffer += fileHandler.getChar();
					if (fileHandler.peek(1) != '\"')
						buffer += fileHandler.getChar();
					errorCnt++;
					return Token("StringLiteral", "Error", buffer, currentLoc);
				}
				else
				{
					buffer += fileHandler.getNChar(3);
					while (!fileHandler.isEOF() && isHexaDigit(fileHandler.peek(1)))
						buffer += fileHandler.getChar();
					continue;
				}
			}
			else
			{
				buffer += fileHandler.getNChar(2);
				while (!fileHandler.isEOF() && fileHandler.peek(1) != '\"')
					buffer += fileHandler.getChar();
				errorCnt++;
				if (fileHandler.peek(1) == '\"')
					buffer += fileHandler.getChar();
				return Token("StringLiteral", "Error", buffer, currentLoc);
			}
		}
		else
		{
			stringLiteralCnt++;
			buffer += fileHandler.getChar();
			return Token("StringLiteral", buffer, currentLoc);
		}
	}
	errorCnt++;
	return Token("StringLiteral", "Error", buffer, currentLoc);
}

Token Analyzer::getCharacterConstant()
{
	while (!fileHandler.isEOF())
	{
		if (fileHandler.peek(1) != '\\' && fileHandler.peek(1) != '\'')
			buffer += fileHandler.getChar();
		else if (fileHandler.peek(1) == '\\')
		{
			if (isSimpleEscape(fileHandler.peek(2)))
				buffer += fileHandler.getNChar(2);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7'
				&& fileHandler.peek(3) >= '0' && fileHandler.peek(3) <= '7'
				&& fileHandler.peek(4) >= '0' && fileHandler.peek(4) <= '7')
				buffer += fileHandler.getNChar(4);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7'
				&& fileHandler.peek(3) >= '0' && fileHandler.peek(3) <= '7')
				buffer += fileHandler.getNChar(3);
			else if (fileHandler.peek(2) >= '0' && fileHandler.peek(2) <= '7')
				buffer += fileHandler.getNChar(2);
			else if (fileHandler.peek(2) == 'x')
			{
				if (!isHexaDigit(fileHandler.peek(3)))
				{

					buffer += fileHandler.getNChar(3);
					while (!fileHandler.isEOF() && fileHandler.peek(1) != '\'')
						buffer += fileHandler.getChar();
					errorCnt++;
					if (fileHandler.peek(1) == '\'')
						buffer += fileHandler.getChar();
					return Token("Constant", "Error", buffer, currentLoc);
				}
				else
				{
					buffer += fileHandler.getNChar(3);
					while (!fileHandler.isEOF() && isHexaDigit(fileHandler.peek(1)))
						buffer += fileHandler.getChar();
					continue;
				}
			}
			else
			{
				buffer += fileHandler.getNChar(2);
				while (!fileHandler.isEOF() && fileHandler.peek(1) != '\'')
					buffer += fileHandler.getChar();
				if (fileHandler.peek(1) != '\'')
					buffer += fileHandler.getChar();
				errorCnt++;
				return Token("Constant", "Error", buffer, currentLoc);
			}
		}
		else
		{
			constantCnt++;
			buffer += fileHandler.getChar();
			return Token("Constant", "CharacterConstant", buffer, currentLoc);
		}
	}
	errorCnt++;
	return Token("Constant", "Error", buffer, currentLoc);
}

Token Analyzer::getIdentifier()
{
	char temp;
	string* p;
	while (!fileHandler.isEOF() && (isalnum(temp = (fileHandler.peek(1))) || temp == '_'))
		buffer += fileHandler.getChar();
	if ((p = find(keyword, keyword + 36, buffer)) != keyword + 36)
	{
		keywordCnt++;
		return Token("Keyword", buffer, currentLoc);
	}
	else
	{
		identifierCnt++;
		return Token("Identifier", buffer, currentLoc);
	}
}

Token Analyzer::getNumericConstant()
{
	while (!fileHandler.isEOF() && isConstantChar(fileHandler.peek(1)))
	{
		if ((fileHandler.peek(1) == 'e' || fileHandler.peek(1) == 'E' ||
			fileHandler.peek(1) == 'p' || fileHandler.peek(1) == 'P') &&
			(fileHandler.peek(2) == '+' || fileHandler.peek(2) == '-'))
			buffer += fileHandler.getChar();
		buffer += fileHandler.getChar();
	}
		
	size_t pos;
	stof(buffer, &pos);
	if (isSuffix(pos))
	{
		constantCnt++;
		if (buffer.find_first_of(".eEpP") != string::npos)
			return Token("Constant", "FloatingConstant", buffer, currentLoc);
		else
			return Token("Constant", "IntegerConstant", buffer, currentLoc);
	}
	else
		return Token("Constant", "Error", buffer, currentLoc);
}

ostream& operator << (ostream& of, const Token& token)
{
	of << "Token type: ";
	if (token.subtype != "")
		of << token.type << " (" << token.subtype << ")" << endl;
	else
		of << token.type << endl;
	of << "Value: " << token.value << endl;
	return of;
}