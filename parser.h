#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "exceptions.h"
#include "token.h"
#include "tokenizer.h"
#include "node.h"

namespace EquationParser
{

	struct Parser
	{
	private:
		Tokenizer m_tokenizer;
		std::string m_equation;
		Token m_lastToken;
		Node* m_tree;

		bool isEquals() const { return m_lastToken.type == TOKEN_EQUIVALENT || m_lastToken.type == TOKEN_IMPLIES; }
		void nextToken()
		{
			// std::cout << translateTokenType(m_lastToken.type) << "-" << m_lastToken.value << std::endl;
			if (m_lastToken.type == TOKEN_EOF)
				throw EofException();

			m_lastToken = m_tokenizer.nextToken();
		}

		void expect(TokenType token) { expect({token}); }

		Token expectGet(TokenType token) { return expectGet({token}); }

		Token expectGet(std::initializer_list<TokenType> tokens)
		{
			Token tok = m_lastToken;
			expect(tokens);
			return tok;
		}

		void expect(std::initializer_list<TokenType> tokens)
		{
			if (std::find(tokens.begin(), tokens.end(), m_lastToken.type) == tokens.end())
				expectedError(tokens);

			nextToken();
		}

		void expectedError(std::initializer_list<TokenType> tokens)
		{
			std::string err("Unexpected ");
			err += std::string(translateTokenType(m_lastToken.type)) + ", expecting one of: ";
			bool first = true;
			for (auto token : tokens)
			{
				if (first)
					first = false;
				else
					err += ", ";

				err += translateTokenType(token);
			}
			// $pos = tokenizer->getPos() - 1;
			// if($data === null)
			// 	throw new InvalidSyntaxException(tokenizer, 'Unexpected "' . translateTokenType(lastToken.type) . '"', $pos);
			// else if(is_array($data)) {
			// 	$str = 'one of ';
			// 	foreach($data as $d)
			// 		$str .= '"' . translateTokenType($d) . '", ';
			// 	$str = substr($str, 0, -2);
			// }
			// else
			// 	$str = '"' . translateTokenType($data) . '"';

			// throw new InvalidSyntaxException(tokenizer, 'Expected ' . $str . ', got "' . translateTokenType(lastToken.type) . '"', $pos);
		}

		// void parseNodeChildren(NodeType nodeType, TokenType tokenType, std::function<void(Node*)> callback, Node* parent)
		// {
		// 	Node* current = new Node(nodeType);
		// 	parent->addChild(current);
		// 	callback(current);

		// 	while (lastToken.type == tokenType)
		// 	{
		// 		nextToken();
		// 		callback(current);
		// 	}
		// }

		const char* translateTokenType(TokenType) const;
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

	public:
		Parser(std::string equation) : m_tokenizer(Tokenizer(equation)), m_equation(equation), m_tree(nullptr) {}
		void parse();
		Node* getTree() const { return m_tree; }
		void deleteTree() { deleteTreeHelper(m_tree); }
	};

}

#endif
