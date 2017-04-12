#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <vector>
#include <functional>
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
	//TYPE_HTML_TEST,
};

enum StreamType {
	STREAM_TYPE_UNK,
	STREAM_TYPE_IO,
	STREAM_TYPE_FILE
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

template<typename T, typename U>
class streamFactory
{
protected:
	StreamType m_type;
	U m_fstream;

public:
	streamFactory(std::string type) {
		if (type == "stream")
			m_type = STREAM_TYPE_IO;
		else if (type.find("file:") == 0)
		{
			m_type = STREAM_TYPE_FILE;
			m_fstream.open(type.substr(sizeof("file:") - 1));
		}
		else
			m_type = STREAM_TYPE_UNK;
	}
};

struct istreamFactory : public streamFactory<std::istream, std::ifstream>
{
	istreamFactory(std::string t) : streamFactory(t) {}
	std::istream& get() {
		switch (m_type)
		{
		case STREAM_TYPE_IO: return std::cin;
		case STREAM_TYPE_FILE: return m_fstream;
		default: throw std::invalid_argument("Invalid input_type");
		}
	}
};

struct ostreamFactory : public streamFactory<std::ostream, std::ofstream>
{
	ostreamFactory(std::string t) : streamFactory(t) {}
	std::ostream& get() {
		switch (m_type)
		{
		case STREAM_TYPE_IO: return std::cout;
		case STREAM_TYPE_FILE: return m_fstream;
		default: throw std::invalid_argument("Invalid output_type");
		}
	}
};

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
	if (strcmp(argv[1], "json") == 0)
		type = TYPE_JSON;
	//else if (strcmp(argv[0], "htmltest") == 0)
		//type = TYPE_HTML_TEST;

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
