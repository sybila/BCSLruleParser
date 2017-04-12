#include "parser.h"

namespace EquationParser
{

	const char* Parser::translateTokenType(TokenType type) const
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
			case TOKEN_MULTIPLE_NUM : return "multiplication number";
			case TOKEN_COMMA : return ",";
			case TOKEN_EQUIVALENT : return "<=>";
			default: return "unknown";
		}
	}

	// X{A}
	void Parser::parseStateEntity(Node* parent)
	{
		if (m_lastToken.type == TOKEN_LEFT_BRACE)
		{
			nextToken();
			parent->addChild(new TokenNode(NODE_STATE, expectGet({TOKEN_ENT_NAME})));
			expect(TOKEN_RIGHT_BRACE);
		}
	}

	// vertical line format in brackets X(A|B)
	void Parser::parsePartialCompositionChildren(Node* current)
	{
		Node* simpNode = new TokenNode(NODE_SIMPLE, expectGet({TOKEN_ENT_NAME, TOKEN_VARIABLE}));
		current->addChild(simpNode);
		parseStateEntity(simpNode);

		while (m_lastToken.type == TOKEN_VERT_LINE)
		{
			nextToken();
			Node* simpNode = new TokenNode(NODE_SIMPLE, expectGet({TOKEN_ENT_NAME, TOKEN_VARIABLE}));
			current->addChild(simpNode);
			parseStateEntity(simpNode);
		}
	}

	// entity name + brackets - X(...)
	void Parser::parsePartialComposition(Node* parent)
	{
		Node* entNode = new TokenNode(NODE_SIMPLE, expectGet({TOKEN_ENT_NAME, TOKEN_VARIABLE}));
		Node* current = new EntityNode(NODE_PARTIAL_COMPOSITION, entNode);
		parent->addChild(current);

		if (m_lastToken.type == TOKEN_LEFT_BRACKET)
		{
			nextToken();
			parsePartialCompositionChildren(current);
			expect(TOKEN_RIGHT_BRACKET);
		}
		else
			parseStateEntity(entNode);
	}

	// dot format - X.Y
	void Parser::parseFullComposition(Node* parent)
	{
		Node* current = new Node(NODE_FULL_COMPOSITION);
		parent->addChild(current);
		parsePartialComposition(current);

		while (m_lastToken.type == TOKEN_DOT)
		{
			nextToken();
			parsePartialComposition(current);
		}
	}

	// X::Y::Z
	void Parser::parseRuleAgent(Node* parent)
	{
		Node* current = new Node(NODE_RULE_AGENT);
		parent->addChild(current);
		parseFullComposition(current);

		while (m_lastToken.type == TOKEN_DOUBLECOLON)
		{
			nextToken();
			parseFullComposition(current);
		}
	}

	// 5 X
	void Parser::parseStoichiometry(Node* parent)
	{
		Token t;
		if (m_lastToken.type == TOKEN_MULTIPLE_NUM) {
			t = m_lastToken;
			nextToken();
		}
		else {
			t = Token(TOKEN_MULTIPLE_NUM);
			// t.type = TOKEN_MULTIPLE_NUM;
			t.value = "1";
		}

		Node* current = new TokenNode(NODE_STOICHIOMETRY, t);
		parent->addChild(current);
		parseRuleAgent(current);
	}

	// X + Y
	void Parser::parseEquationSide(Node* parent)
	{
		Node* current = new Node(NODE_EQUATION_SIDE);
		parent->addChild(current);
		parseStoichiometry(current);

		while (m_lastToken.type == TOKEN_PLUS)
		{
			nextToken();
			parseStoichiometry(current);
		}
	}

	void Parser::parseEquationPart(Node* parent)
	{
		TokenNode* current = new TokenNode(NODE_EQUATION);
		parent->addChild(current);
		nextToken();

		if(isEquals()) {
			current->setToken(m_lastToken);
			nextToken();
			parseEquationSide(current);

			if(isEquals())
				expectedError({TOKEN_EQUIVALENT, TOKEN_IMPLIES});
		}
		else {
			parseEquationSide(current);
			if(!isEquals())
				expectedError({TOKEN_EQUIVALENT, TOKEN_IMPLIES});

			current->setToken(m_lastToken);
			nextToken();

			if(m_lastToken.type != TOKEN_EOF)
				parseEquationSide(current);
		}
	}

	void Parser::parseVariableChildren(Node* parent)
	{
		parent->addChild(new TokenNode(NODE_VARIABLE_ENTITY, expectGet(TOKEN_ENT_NAME)));

		while (m_lastToken.type == TOKEN_COMMA)
		{
			nextToken();
			parent->addChild(new TokenNode(NODE_VARIABLE_ENTITY, expectGet(TOKEN_ENT_NAME)));
		}
	}

	void Parser::parseVariablePart(Node* parent)
	{
		if(m_lastToken.type == TOKEN_EOF)
			return;

		expect(TOKEN_SEMICOLON);

		Token t = expectGet({TOKEN_VARIABLE});
		expect(TOKEN_EQUALS);

		expect(TOKEN_LEFT_BRACE);

		Node* entNode = new TokenNode(NODE_VARIABLE, t);
		parseVariableChildren(entNode);
		parent->addChild(entNode);

		expect(TOKEN_RIGHT_BRACE);
	}

	void Parser::parse()
	{
		try {
			m_tree = new Node(NODE_SEMICOLON);
			parseEquationPart(m_tree);
			parseVariablePart(m_tree);
		}
		catch (const EofException&) {
			;
		}
	}

	void Parser::deleteTreeHelper(Node* node)
	{
		for (Node* child : node->getChildren())
			deleteTreeHelper(child);
		if (node->getClass() == NODE_CTYPE_ENTITY)
			deleteTreeHelper(reinterpret_cast<EntityNode*>(node)->getEntity());

		delete node;
	}

}
