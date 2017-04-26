#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>
#include "token.h"

namespace RuleParser
{
	struct InvalidSyntax : public std::runtime_error
	{
		InvalidSyntax(unsigned start, std::string unexpected) :
			std::runtime_error(""), m_start(start), m_unexpected(unexpected) {}
		InvalidSyntax(unsigned start, std::string unexpected, char expected) :
			std::runtime_error(""), m_start(start), m_unexpected(unexpected), m_expected(expected) {}

		unsigned m_start;
		std::string m_unexpected;
		char m_expected;
	};

	struct InvalidSyntaxToken : public std::runtime_error
	{
		InvalidSyntaxToken(unsigned start, TokenType unexpected, std::initializer_list<TokenType> expected) :
			std::runtime_error(""), m_start(start), m_unexpected(unexpected), m_expected(expected) {}

		unsigned m_start;
		TokenType m_unexpected;
		std::initializer_list<TokenType> m_expected;
	};

	struct EofException : public std::runtime_error
	{
		EofException() : runtime_error("") {}
	};
};

#endif
