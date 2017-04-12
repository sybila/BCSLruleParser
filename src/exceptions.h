#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>

namespace EquationParser
{
	struct InvalidSyntax : public std::runtime_error
	{
		InvalidSyntax(const std::string& what, unsigned position) : runtime_error(what), m_pos(position) {}
		unsigned getPosition() const { return m_pos; }

	private:
		unsigned m_pos;
	};

	struct EofException : public std::runtime_error
	{
		EofException() : runtime_error("") {}
	};
};

#endif
