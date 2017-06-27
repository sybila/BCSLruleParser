#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <ios>

#include "exceptions.h"
#include "node.h"

using namespace RuleParser;

namespace JSON
{

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
			case TOKEN_PLUS: return "+";
			case TOKEN_DOUBLECOLON: return "::";
			case TOKEN_EQUALS: return "=";
			case TOKEN_MULTIPLE_NUM: return "number";
			case TOKEN_COMMA: return ",";
			case TOKEN_EQUIVALENT: return "<=>";
			case TOKEN_SPACE: return "whitespace";
			case TOKEN_DOT: return ".";
			default: return "unknown";
		}
	}

	std::string escape(char ch)
	{
		if (ch == '"')
			return "\\\"";

		return std::string(1, ch);
	}

	std::string escape(std::string str)
	{
		size_t start_pos = 0, from = 0;

		while ((start_pos = str.find('"', from)) != std::string::npos)
		{
			str.replace(start_pos, 1, "\\\"");
			from = start_pos + 2;
		}

		return str;
	}

	void convertTree(std::ostream&, Node*);

	void convertChildren(std::ostream& os, Node* node)
	{
		bool first = true;
		for (const std::unique_ptr<Node>& child : node->getChildren())
		{
			if (first)
				first = false;
			else
				os << ",";

			convertTree(os, child.get());
		}
	}

	void convertTree(std::ostream& os, Node* node)
	{
		os << "{\"type\":" << node->getType();

		switch (node->getClass())
		{
			case NODE_CTYPE_NODE:
				break;
			case NODE_CTYPE_TOKEN:
			{
				TokenNode* tn = reinterpret_cast<TokenNode*>(node);
				os << ",\"token\":\"" << tn->getToken().value << "\"";
				break;
			}
			case NODE_CTYPE_ENTITY:
			{
				EntityNode* en = reinterpret_cast<EntityNode*>(node);
				os << ",\"entity\":";
				convertTree(os, en->getEntity());
				break;
			}
		}

		os << ",\"children\":[";
		convertChildren(os, node);
		os << "]}";
	}

	void convertException(std::ostream& os, const InvalidSyntax& e)
	{
		os << "{\"error\":1,\"start\":" << e.start <<
				",\"unexpected\":\"" << escape(e.unexpected) <<
				"\",\"expected\":[";

		if (e.expected != '\0')
			os << "\"" << escape(e.expected) << "\"";

		os << "]}" << std::endl;
	}

	void convertException(std::ostream& os, const InvalidSyntaxToken& e)
	{
		os << "{\"error\":1,\"start\":" << e.start <<
				",\"unexpected\":\"" << escape(e.unexpected) <<
				"\",\"expected\":[";

		bool first = true;
		for (TokenType t : e.expected)
		{
			if (first)
				first = false;
			else
				os << ",";

			os << "\"" << escape(translateTokenType(t)) << "\"";
		}

		os << "]}" << std::endl;
	}

}

#endif
