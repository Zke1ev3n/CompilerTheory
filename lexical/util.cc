#include "util.h"
#include<utility>

map<string, string> punctuatorType;

string keyword[] =
{ "auto", "break", "case", "char", "const", "continue", "default" "do",
"double", "else", "enum", "extern", "float", "for", "goto", "if",
"inline", "int", "long", "register", "restrict", "return", "short", "signed",
"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void",
"volatile", "while", "_Bool", "_Complex", "_Imaginary" };

void mapInit()
{
	punctuatorType.insert(pair<string, string>("[", "OpenBracket"));
	punctuatorType.insert(pair<string, string>("]", "CloseBracket"));
	punctuatorType.insert(pair<string, string>("(", "OpenParen"));
	punctuatorType.insert(pair<string, string>(")", "CloseParen"));
	punctuatorType.insert(pair<string, string>("{", "OpenBrace"));
	punctuatorType.insert(pair<string, string>("}", "CloseBrace"));
	punctuatorType.insert(pair<string, string>(".", "Dot"));
	punctuatorType.insert(pair<string, string>("->", "AccessThroughPointer"));
	punctuatorType.insert(pair<string, string>("+", "Plus"));
	punctuatorType.insert(pair<string, string>("-", "Minus"));
	punctuatorType.insert(pair<string, string>("&", "BitwiseAnd"));
	punctuatorType.insert(pair<string, string>("|", "BitwideOr"));
	punctuatorType.insert(pair<string, string>("~", "BitwideNot"));
	punctuatorType.insert(pair<string, string>("++", "Increment"));
	punctuatorType.insert(pair<string, string>("--", "Decrement"));
	punctuatorType.insert(pair<string, string>("*", "Star"));
	punctuatorType.insert(pair<string, string>("!", "LogicalNot"));
	punctuatorType.insert(pair<string, string>("/", "Slash"));
	punctuatorType.insert(pair<string, string>("%", "Percent"));
	punctuatorType.insert(pair<string, string>("<<", "LeftShift"));
	punctuatorType.insert(pair<string, string>(">>", "RightShift"));
	punctuatorType.insert(pair<string, string>("<", "Less"));
	punctuatorType.insert(pair<string, string>(">", "More"));
	punctuatorType.insert(pair<string, string>("<=", "LessOrEqual"));
	punctuatorType.insert(pair<string, string>(">=", "MoreOrEqual"));
	punctuatorType.insert(pair<string, string>("==", "DoubleEqual"));
	punctuatorType.insert(pair<string, string>("!=", "NotEqual"));
	punctuatorType.insert(pair<string, string>("^", "Caret"));
	punctuatorType.insert(pair<string, string>("&&", "LogicalAnd"));
	punctuatorType.insert(pair<string, string>("||", "LogicalOr"));
	punctuatorType.insert(pair<string, string>("?", "Question"));
	punctuatorType.insert(pair<string, string>(":", "Colon"));
	punctuatorType.insert(pair<string, string>(";", "Semicolon"));
	punctuatorType.insert(pair<string, string>("...", "Ellipsis"));
	punctuatorType.insert(pair<string, string>("=", "Equal"));
	punctuatorType.insert(pair<string, string>("*=", "MultiplyEqual"));
	punctuatorType.insert(pair<string, string>("/=", "DivideEqual"));
	punctuatorType.insert(pair<string, string>("%=", "ModuleEqual"));
	punctuatorType.insert(pair<string, string>("+=", "RightShift"));
	punctuatorType.insert(pair<string, string>("-=", "MinusEqual"));
	punctuatorType.insert(pair<string, string>("<<=", "LeftShiftEqual"));
	punctuatorType.insert(pair<string, string>(">>=", "RightShiftEqual"));
	punctuatorType.insert(pair<string, string>("&=", "BitwiseAndEqual"));
	punctuatorType.insert(pair<string, string>("^=", "BitwiseXorEqual"));
	punctuatorType.insert(pair<string, string>("|=", "BitwiseOrEqual"));
	punctuatorType.insert(pair<string, string>("#", "Pound"));
	punctuatorType.insert(pair<string, string>("##", "DoublePound"));
	punctuatorType.insert(pair<string, string>("<:", "LessColon"));
	punctuatorType.insert(pair<string, string>(":>", "ColonMore"));
	punctuatorType.insert(pair<string, string>("<%", "LessPercent"));
	punctuatorType.insert(pair<string, string>("%>", "PercentMore"));
	punctuatorType.insert(pair<string, string>("%:", "PercentColon"));
	punctuatorType.insert(pair<string, string>("%:%:", "DoublePercentColon"));
}
