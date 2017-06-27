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

void stringCommunicate(std::istream& in, std::ostream& out)
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
			std::cout << "Unexpected " << e.unexpected << " at " << e.start << ", expecting: " << std::string(1, e.expected) << std::endl;
			stringMarkPos(line, e.start);
		}
		catch (const InvalidSyntaxToken& e) {
			std::cout << "Unexpected " << e.unexpected << " at " << e.start << ", expecting: " << JSON::translateTokenType(*e.expected.begin()) << std::endl;
			stringMarkPos(line, e.start);
		}
	}
}

int main(int argc, const char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage: equation_parser <communication_type> <input_type> <output_type>" << std::endl
				<< "communication_type: json" << std::endl
				<< "input_type: stream, file:<path>" << std::endl
				<< "output_type: stream, file:<path>" << std::endl;
		return 1;
	}

	CommType type;
	std::string typeRaw(argv[1]);
	if (typeRaw == "json")
		type = TYPE_JSON;
	else if (typeRaw == "string")
		type = TYPE_STRING;
	else
	{
		std::cerr << "Invalid communication_type" << std::endl;
		return 1;
	}

	try {
		istreamFactory iFact(argv[2]);
		ostreamFactory oFact(argv[3]);
		if (type == TYPE_JSON)
			jsonCommunicate(iFact.get(), oFact.get());
		else if (type == TYPE_STRING)
			stringCommunicate(iFact.get(), oFact.get());
	}
	catch (const std::runtime_error& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}

	return 0;
}
