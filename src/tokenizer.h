#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <cctype>
#include <stack>
#include "exceptions.h"
#include "token.h"
#include "tokenizer.h"
#include "node.h"

namespace RuleParser
{

	enum TokenizerMode
	{
		MODE_NORMAL,
		MODE_VARIABLE
	};

	struct Tokenizer
	{

	private:
		std::string m_string;
		int m_pos;
		int m_end;
		TokenizerMode m_mode;
		std::stack<char> m_expected;

		char nextChar();
		bool isEntChar(char ch) { return isalnum(ch) || ch == '-' || ch == '_' || ch == '*' || ch == '+'; }
		bool isAllowedAfterEnt(char ch);
		void modeNormal(Token& token, char ch, char prev);
		void modeVariable(Token& token, char ch);
		Token _nextToken(char prev);

	public:
		Tokenizer(std::string s) : m_string(s), m_pos(0), m_end(s.size()), m_mode(MODE_NORMAL) { }

		int getPos() { return m_pos; }
		Token nextToken() { return _nextToken(0); }

	};

}

#endif
