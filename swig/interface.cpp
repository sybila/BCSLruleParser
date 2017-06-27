#include <sstream>
#include "json_helpers.h"

#include "interface.h"

ParseResult* parse(std::string eq)
{
	return new ParseResult(eq);
}

const RuleParser::Node* ParseResult::getTree() const
{
	if (!m_success)
		throw std::runtime_error("Cannot access tree on 'not ok' parse result");

	return m_tree.get();
}

const ParseError& ParseResult::getError() const
{
	if (m_success)
		throw std::runtime_error("Cannot access error on 'ok' parse result");

	return *m_error;
}

ParseResult::ParseResult(std::string eq) : m_success(false), m_tree(nullptr), m_error(nullptr)
{
	try {
		RuleParser::Parser parser(eq);
		parser.parse();
		m_tree = parser.getTree();
		m_success = true;
	}
	catch (const RuleParser::InvalidSyntax& e) {
		m_error.reset(new ParseError());
		m_error->m_start = e.start;
		m_error->m_unexpected = e.unexpected;
		m_error->m_expected = {std::string(1, e.expected)};
	}
	catch (const RuleParser::InvalidSyntaxToken& e) {
		m_error.reset(new ParseError());
		m_error->m_start = e.start;
		m_error->m_unexpected = e.unexpected;
		m_error->m_expected.reserve(e.expected.size());

		for (RuleParser::TokenType tok : e.expected)
			m_error->m_expected.push_back(JSON::translateTokenType(tok));
	}
}

std::string parseEquations(std::string input)
{
	std::istringstream in(input);
	std::ostringstream os;

	while (in.good())
	{
		std::string line;
		std::getline(in, line);

		if (line.size() == 0)
			continue;

		try {
			RuleParser::Parser p(line);
			p.parse();
			JSON::convertTree(os, p.getTree().get());
			os << std::endl;
		}
		catch (const InvalidSyntax& e) {
			JSON::convertException(os, e);
		}
		catch (const InvalidSyntaxToken& e) {
			JSON::convertException(os, e);
		}
	}

	return os.str();
}
