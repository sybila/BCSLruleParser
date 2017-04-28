#include <iostream>

#include "stream_factory.h"
#include "json_helpers.h"
#include "parser.h"

using namespace RuleParser;

void jsonCommunicate(CommType type, std::istream& in, std::ostream& out)
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
			JSON::convertTree(out, p.getTree());
			out << std::endl;
			p.deleteTree();
		}
		catch (const InvalidSyntax& e) {
			JSON::convertException(out, e);
		}
		catch (const InvalidSyntaxToken& e) {
			JSON::convertException(out, e);
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
	else
	{
		std::cerr << "Invalid communication_type" << std::endl;
		return 1;
	}

	try {
		istreamFactory iFact(argv[2]);
		ostreamFactory oFact(argv[3]);
		jsonCommunicate(type, iFact.get(), oFact.get());
	}
	catch (const std::runtime_error& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}

	return 0;
}
