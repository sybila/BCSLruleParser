#include <memory>
#include <string>
#include <vector>

#include "parser.h"

struct ParseResult;

struct ParseError
{
	int getStart() const { return m_start; }
	std::string getUnexpected() const { return m_unexpected; }
	int getNumExpected() const { return m_expected.size(); }
	std::string getExpected(int i) const { return m_expected.at(i); }

private:
	friend struct ParseResult;

	int m_start;
	std::string m_unexpected;
	std::vector<std::string> m_expected;
};

struct ParseResult
{
	bool isOk() const { return m_success; }
	const RuleParser::Node* getTree() const;
	const ParseError& getError() const;

private:
	ParseResult(std::string eq);
	friend ParseResult* parse(std::string input);

	bool m_success;
	std::shared_ptr<RuleParser::Node> m_tree;
	std::shared_ptr<ParseError> m_error;
};

ParseResult* parse(std::string input);
std::string parseEquations(std::string input);
