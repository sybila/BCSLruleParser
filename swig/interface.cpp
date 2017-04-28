#include <sstream>
#include "json_helpers.h"
#include "parser.h"

#include "interface.h"

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
			JSON::convertTree(os, p.getTree());
			os << std::endl;
			p.deleteTree();
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
