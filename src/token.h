#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace RuleParser
{

	enum TokenType
	{
		TOKEN_UNKNOWN = 0,
		TOKEN_ENT_NAME,
		TOKEN_LEFT_BRACKET,
		TOKEN_RIGHT_BRACKET,
		TOKEN_LEFT_BRACE,
		TOKEN_RIGHT_BRACE,
		TOKEN_SEMICOLON,
		TOKEN_VERT_LINE,
		TOKEN_VARIABLE,
		TOKEN_IMPLIES, // =>
		TOKEN_PLUS,
		TOKEN_DOUBLECOLON, // ::
		TOKEN_EQUALS,
		TOKEN_EOF,
		TOKEN_SPACE,
		TOKEN_MULTIPLE_NUM,
		TOKEN_COMMA,
		TOKEN_EQUIVALENT, // <=>
		TOKEN_DOT
	};

	struct Token
	{
		TokenType type;
		std::string value;
		unsigned start;
		unsigned end;

		Token() : type(TOKEN_UNKNOWN), value(""), start(0), end(0) {}
		Token(TokenType t) : type(t), value(""), start(0), end(0) {}

		void append(char chr)
		{
			value += chr;
			++end;
		}

		void pop()
		{
			value.resize(value.size() - 1);
			--end;
		}

	};
}

#endif
