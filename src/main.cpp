#include <iostream>
#include <fstream>
#include <vector>
#include "stream_factory.h"
#include "parser.h"

using namespace EquationParser;

void jsonTree(std::ostream& ss, Node* node);

void jsonChildren(std::ostream& ss, Node* node)
{
	bool first = true;
	for (Node* c : node->getChildren())
	{
		if (first)
			first = false;
		else
			ss << ",";

		jsonTree(ss, c);
	}
}

void jsonTree(std::ostream& ss, Node* node)
{
	ss << "{\"type\":" << node->getType();
	
	switch (node->getClass())
	{
		case NODE_CTYPE_NODE:
			break;
		case NODE_CTYPE_TOKEN:
		{
			TokenNode* tn = reinterpret_cast<TokenNode*>(node);
			ss << ",\"token\":\"" << tn->getToken().value << "\"";
			break;
		}
		case NODE_CTYPE_ENTITY:
		{
			EntityNode* en = reinterpret_cast<EntityNode*>(node);
			ss << ",\"entity\":";
			jsonTree(ss, en->getEntity());
			break;
		}
	}

	ss << ",\"children\":[";
	jsonChildren(ss, node);
	ss << "]}";
}

enum CommType {
	TYPE_JSON,
};

enum StreamType {
	STREAM_TYPE_UNK,
	STREAM_TYPE_IO,
	STREAM_TYPE_FILE,
};

void communicate(CommType type, std::istream& in, std::ostream& out)
{
	while (in.good())
	{
		std::string line;
		std::getline(in, line);
		try {
			Parser p(line);
			p.parse();
			jsonTree(out, p.getTree());
			out << std::endl;
			p.deleteTree();
		}
		catch (const InvalidSyntax& e) {
			out << "{\"error\":\"" << e.what() << "\",\"position\":" << e.getPosition() << "}" << std::endl;
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

	try {
		istreamFactory iFact(argv[2]);
		ostreamFactory oFact(argv[3]);
		communicate(type, iFact.get(), oFact.get());
	}
	catch (const std::runtime_error& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}

	return 0;
}
