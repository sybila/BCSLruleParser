#include <iostream>
#include "tokenizer.h"

namespace EquationParser
{

	char Tokenizer::nextChar()
	{
		if (m_pos == m_end)
			throw InvalidSyntax("Unexpected end of file", getPos());

		return m_string[m_pos++];
	}

	Token Tokenizer::_nextToken(char prev)
	{
		if (m_pos == m_end)
			return {TOKEN_EOF};

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
				throw InvalidSyntax(std::string("Unexpected ") + ch + ", expecting " + m_expected.top(), getPos());

			m_expected.pop();
			tok.append(ch);
		}

		if (tok.type == TOKEN_UNKNOWN)
			throw InvalidSyntax("Invalid token", getPos());

		return tok;
	}

	void Tokenizer::modeNormal(Token& tok, char ch, char prev)
	{
		switch(ch) {
			case '(': tok.type = TOKEN_LEFT_BRACKET; return;
			case ')': tok.type = TOKEN_RIGHT_BRACKET; return;
			case '{': tok.type = TOKEN_LEFT_BRACE; return;
			case '}': tok.type = TOKEN_RIGHT_BRACE; return;
			case '|': tok.type = TOKEN_VERT_LINE; return;
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

		if (m_pos == m_end || !isEntChar(ch))
			return;

		while (m_pos != m_end)
		{
			if (isdigit(ch))
			{
				if (tok.type == TOKEN_UNKNOWN)
					tok.type = TOKEN_MULTIPLE_NUM;
			}
			else
			{
				if (tok.type == TOKEN_MULTIPLE_NUM || tok.type == TOKEN_UNKNOWN)
					tok.type = TOKEN_ENT_NAME;
			}

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
		switch(ch) {
			case '=': tok.type = TOKEN_EQUALS; return;
			case '?': tok.type = TOKEN_VARIABLE; m_expected.push('X'); return;
			case '{': tok.type = TOKEN_LEFT_BRACE; return;
			case '}': tok.type = TOKEN_RIGHT_BRACE; return;
			case ',': tok.type = TOKEN_COMMA; return;
			case '.': tok.type = TOKEN_DOT; return;
		}

		if (m_pos == m_end || !isEntChar(ch))
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
