#include <algorithm> // find
#include "parser.h"

namespace RuleParser
{

	Token Parser::expectGet(std::initializer_list<TokenType> tokens)
	{
		Token tok = m_lastToken;
		expect(tokens);
		return tok;
	}

	void Parser::expect(std::initializer_list<TokenType> tokens)
	{
		if (std::find(tokens.begin(), tokens.end(), m_lastToken.type) == tokens.end())
			expectedError(tokens);

		nextToken();
	}

	// X{A}
	void Parser::parseStateEntity(Node* parent)
	{
		if (m_lastToken.type == TOKEN_LEFT_BRACE)
		{
			nextToken();
			parent->addChild(new TokenNode(NODE_STATE, expectGet({TOKEN_ENT_NAME})));
			expect({TOKEN_RIGHT_BRACE});
		}
	}

	// vertical line format in brackets X(A|B)
	void Parser::parsePartialCompositionChildren(Node* current)
	{
		Node* simpNode = new TokenNode(NODE_SIMPLE, expectGet({TOKEN_ENT_NAME, TOKEN_VARIABLE}));
		current->addChild(simpNode);
		parseStateEntity(simpNode);

		while (m_lastToken.type == TOKEN_COMMA)
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
			expect({TOKEN_RIGHT_BRACKET});
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
		parent->addChild(new TokenNode(NODE_VARIABLE_ENTITY, expectGet({TOKEN_ENT_NAME})));

		while (m_lastToken.type == TOKEN_COMMA)
		{
			nextToken();
			parent->addChild(new TokenNode(NODE_VARIABLE_ENTITY, expectGet({TOKEN_ENT_NAME})));
		}
	}

	void Parser::parseVariablePart(Node* parent)
	{
		if(m_lastToken.type == TOKEN_EOF)
			return;

		expect({TOKEN_SEMICOLON});

		Token t = expectGet({TOKEN_VARIABLE});
		expect({TOKEN_EQUALS});

		expect({TOKEN_LEFT_BRACE});

		Node* entNode = new TokenNode(NODE_VARIABLE, t);
		parseVariableChildren(entNode);
		parent->addChild(entNode);

		expect({TOKEN_RIGHT_BRACE});
	}

	void Parser::parse()
	{
		try {
			m_tree.reset(new Node(NODE_SEMICOLON));
			parseEquationPart(m_tree.get());
			parseVariablePart(m_tree.get());
		}
		catch (const EofException&) {
			;
		}
	}

}
