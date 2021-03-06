#include <iostream>
#include "tokenizer.h"

namespace RuleParser
{

	char Tokenizer::nextChar()
	{
		if (m_pos == m_end)
		{
			if (m_expected.empty())
				throw InvalidSyntax(getPos(), "end of input");
			else
				throw InvalidSyntax(getPos(), "end of input", m_expected.top());
		}

		return m_string[m_pos++];
	}

	Token Tokenizer::_nextToken(char prev)
	{
		if (m_pos == m_end)
			return {TOKEN_EOF, "end of input"};

		Token tok;
		tok.start = tok.end = getPos();
		char ch = nextChar();
		tok.append(ch);

		// skip whitespace
		if (isspace(ch))
			return _nextToken(ch);

		switch (m_mode)
		{
			case MODE_NORMAL: modeNormal(tok, ch, prev); break;
			case MODE_VARIABLE: modeVariable(tok, ch); break;
		}

		while (!m_expected.empty())
		{
			ch = nextChar();
			if (m_expected.top() != ch)
				throw InvalidSyntax(getPos() - 1, std::string(1, ch), m_expected.top());

			m_expected.pop();
			tok.append(ch);
		}

		if (tok.type == TOKEN_UNKNOWN)
			throw InvalidSyntax(tok.start, tok.value);

		return tok;
	}

	void Tokenizer::modeNormal(Token& tok, char ch, char prev)
	{
		switch(ch)
		{
			case '(': tok.type = TOKEN_LEFT_BRACKET; return;
			case ')': tok.type = TOKEN_RIGHT_BRACKET; return;
			case '{': tok.type = TOKEN_LEFT_BRACE; return;
			case '}': tok.type = TOKEN_RIGHT_BRACE; return;
			case ',': tok.type = TOKEN_COMMA; return;
			case '.': tok.type = TOKEN_DOT; return;
			case '?': tok.type = TOKEN_VARIABLE; m_expected.push('X'); return;
			case ':': tok.type = TOKEN_DOUBLECOLON; m_expected.push(':'); return;
			case '<': tok.type = TOKEN_EQUIVALENT; m_expected.push('>'); m_expected.push('='); return;
			case '=': tok.type = TOKEN_IMPLIES; m_expected.push('>'); return;
			case ';': tok.type = TOKEN_SEMICOLON; m_mode = MODE_VARIABLE; return;
			case '+':
				if (prev == ' ')
				{
					--tok.start;
					tok.type = TOKEN_PLUS;
					tok.value.insert(0, 1, ' ');
					m_expected.push(' ');
					return;
				}
				// no break/return intended
		}

		if (isEntChar(ch))
		{
			if (isdigit(ch))
				tok.type = TOKEN_MULTIPLE_NUM;
			else
				tok.type = TOKEN_ENT_NAME;
		}
		else
			return;

		while (m_pos != m_end)
		{
			if (!isdigit(ch) && tok.type == TOKEN_MULTIPLE_NUM)
				tok.type = TOKEN_ENT_NAME;

			ch = nextChar();
			if (!isEntChar(ch))
			{
				if (m_pos != 0)
					--m_pos;
				
				break;
			}

			tok.append(ch);
		}
	}

	void Tokenizer::modeVariable(Token& tok, char ch)
	{
		switch(ch)
		{
			case '=': tok.type = TOKEN_EQUALS; return;
			case '?': tok.type = TOKEN_VARIABLE; m_expected.push('X'); return;
			case '{': tok.type = TOKEN_LEFT_BRACE; return;
			case '}': tok.type = TOKEN_RIGHT_BRACE; return;
			case ',': tok.type = TOKEN_COMMA; return;
			case '.': tok.type = TOKEN_DOT; return;
		}

		if (!isEntChar(ch))
			return;

		tok.type = TOKEN_ENT_NAME;
		while (m_pos != m_end)
		{
			ch = nextChar();
			if (!isEntChar(ch))
			{
				if (m_pos != 0)
					--m_pos;
				break;
			}
			tok.append(ch);
		}
	}


}
