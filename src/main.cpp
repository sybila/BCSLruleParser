#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "stream_factory.h"
#include "parser.h"

using namespace RuleParser;

const char* translateTokenType(TokenType type)
{
	switch(type) {
		case TOKEN_EOF: return "end of input";
		case TOKEN_ENT_NAME: return "entity name";
		case TOKEN_LEFT_BRACKET: return "(";
		case TOKEN_RIGHT_BRACKET: return ")";
		case TOKEN_LEFT_BRACE: return "{";
		case TOKEN_RIGHT_BRACE: return "}";
		case TOKEN_SEMICOLON: return ";";
		case TOKEN_VERT_LINE: return "|";
		case TOKEN_VARIABLE: return "?X";
		case TOKEN_IMPLIES: return "=>";
		case TOKEN_PLUS : return "+";
		case TOKEN_DOUBLECOLON : return "::";
		case TOKEN_EQUALS : return "=";
		case TOKEN_MULTIPLE_NUM : return "number";
		case TOKEN_COMMA : return ",";
		case TOKEN_EQUIVALENT : return "<=>";
		default: return "unknown";
	}
}

std::string escapeJson(char ch)
{
	if (ch == '"')
		return "\\\"";

	return std::string(1, ch);
}

std::string escapeJson(std::string str)
{
    size_t start_pos = 0, from = 0;

    while ((start_pos = str.find('"', from)) != std::string::npos)
    {
        str.replace(from, 1, "\\\"");
        from = start_pos;
    }

    return str;
}

void jsonTree(std::ostream& ss, Node* node);

void jsonChildren(std::ostream& ss, Node* node)
{
	bool first = true;
	for (Node* child : node->getChildren())
	{
		if (first)
			first = false;
		else
			ss << ",";

		jsonTree(ss, child);
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

void communicate(CommType type, std::istream& in, std::ostream& out)
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
			jsonTree(out, p.getTree());
			out << std::endl;
			p.deleteTree();
		}
		catch (const InvalidSyntax& e) {
			out << "{\"error\":1,\"start\":" << e.m_start <<
				",\"unexpected\":\"" << escapeJson(e.m_unexpected) <<
				"\",\"expected\":[\"" << escapeJson(e.m_expected) << "\"]}" << std::endl;
		}
		catch (const InvalidSyntaxToken& e) {
			out << "{\"error\":1,\"start\":" << e.m_start <<
				",\"unexpected\":\"" << escapeJson(translateTokenType(e.m_unexpected)) <<
				"\",\"expected\":[";

			bool first = true;
			for (TokenType t : e.m_expected)
			{
				if (first)
					first = false;
				else
					out << ",";

				out << "\"" << escapeJson(translateTokenType(t)) << "\"";
			}

			out << "]}" << std::endl;
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
