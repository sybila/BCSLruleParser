#ifndef PARSER_H
#define PARSER_H

#include <initializer_list>
#include "exceptions.h"
#include "token.h"
#include "tokenizer.h"
#include "node.h"

namespace RuleParser
{

	struct Parser
	{
		Parser(std::string equation) : m_tokenizer(Tokenizer(equation)), m_equation(equation), m_tree(nullptr) {}

		void parse();
		Node* getTree() const { return m_tree; }
		void deleteTree() { deleteTreeHelper(m_tree); }

	private:
		Tokenizer m_tokenizer;
		std::string m_equation;
		Token m_lastToken;
		Node* m_tree;

		bool isEquals() const { return m_lastToken.type == TOKEN_EQUIVALENT || m_lastToken.type == TOKEN_IMPLIES; }
		void nextToken()
		{
			if (m_lastToken.type == TOKEN_EOF)
				throw EofException();

			m_lastToken = m_tokenizer.nextToken();
		}

		void expect(std::initializer_list<TokenType> tokens);
		Token expectGet(std::initializer_list<TokenType> tokens);
		void expectedError(std::initializer_list<TokenType> tokens) { throw InvalidSyntaxToken(m_lastToken.start, m_lastToken.value, tokens); }

		void deleteTreeHelper(Node* node);

		void parseStateEntity(Node* parent);
		void parsePartialCompositionChildren(Node* parent);
		void parsePartialComposition(Node* parent);
		void parseFullComposition(Node* parent);
		void parseRuleAgent(Node* parent);
		void parseStoichiometry(Node* parent);
		void parseEquationSide(Node* parent);
		void parseEquationPart(Node* parent);
		void parseVariableChildren(Node* parent);
		void parseVariablePart(Node* parent);
	};

}

#endif
