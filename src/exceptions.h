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
			std::runtime_error(""), start(start), unexpected(unexpected), expected('\0') {}
		InvalidSyntax(unsigned start, std::string unexpected, char expected) :
			std::runtime_error(""), start(start), unexpected(unexpected), expected(expected) {}

		unsigned start;
		std::string unexpected;
		char expected;
	};

	struct InvalidSyntaxToken : public std::runtime_error
	{
		InvalidSyntaxToken(unsigned start, std::string unexpected, std::initializer_list<TokenType> expected) :
			std::runtime_error(""), start(start), unexpected(unexpected), expected(expected) {}

		unsigned start;
		std::string unexpected;
		std::initializer_list<TokenType> expected;
	};

	struct EofException : public std::runtime_error
	{
		EofException() : runtime_error("") {}
	};
};

#endif
