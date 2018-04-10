#include <iostream>

#include "stream_factory.h"
#include "json_helpers.h"
#include "parser.h"

using namespace RuleParser;

void jsonCommunicate(std::istream& in, std::ostream& out)
{
	while (in.good())
	{
		std::string line;
		std::getline(in, line);

		if (line.size() == 0)
			continue;

		try {
			Parser p(line);
			p.parse();
			JSON::convertTree(out, p.getTree().get());
			out << std::endl;
		}
		catch (const InvalidSyntax& e) {
			JSON::convertException(out, e);
		}
		catch (const InvalidSyntaxToken& e) {
			JSON::convertException(out, e);
		}
	}
}

void stringMarkPos(std::string eq, int pos)
{
	std::cout << eq << std::endl;
	std::cout << std::string(pos, ' ') << "^" << std::endl;
}

extern "C"
{
	char* lastAlloc = nullptr;

	char* RuleParserParse(char* inRaw)
	{
		std::string in(inRaw);
		std::stringstream inSs(in);
		std::stringstream outSs;
		jsonCommunicate(inSs, outSs);
		std::string out = outSs.str();
		char* outC = new char[out.size() + 1];
		lastAlloc = outC;
		out.copy(outC, out.size());
		outC[out.size()] = '\0';
		return outC;
	}

	void RuleParserDealloc()
	{
		if (lastAlloc != nullptr)
		{
			delete[] lastAlloc;
			lastAlloc = nullptr;
		}
	}
}
