#ifndef NODES_H
#define NODES_H

#include <vector>
#include "token.h"

namespace RuleParser
{

	enum NodeClassType
	{
		NODE_CTYPE_NODE = 0,
		NODE_CTYPE_TOKEN,
		NODE_CTYPE_ENTITY
	};

	enum NodeType
	{
		NODE_UNKNOWN = 0,
		NODE_EQUATION = 1,
		NODE_EQUATION_SIDE,
		NODE_STOICHIOMETRY,
		NODE_RULE_AGENT,
		NODE_FULL_COMPOSITION,
		NODE_PARTIAL_COMPOSITION,
		NODE_STATE,
		NODE_SIMPLE,
		NODE_SEMICOLON,
		NODE_VARIABLE,
		NODE_VARIABLE_ENTITY
	};

	struct Node
	{
		Node(NodeType type) : Node(type, NODE_CTYPE_NODE) {}

		void addChild(Node* child) { children.push_back(child); child->m_parent = this; }
		NodeType getType() const { return m_type; }
		NodeClassType getClass() const { return m_ctype; }
		Node* getParent() const { return m_parent; }
		const CHILDREN_TYPE& getChildren() const { return children; }

	protected:
		using CHILDREN_TYPE = std::vector<Node*>;

		Node(NodeType type, NodeClassType ctype) : m_ctype(ctype), m_type(type), m_parent(nullptr) {}

	private:
		NodeClassType m_ctype;
		NodeType m_type;
		Node* m_parent;
		CHILDREN_TYPE children;
	};

	struct TokenNode : public Node
	{
		TokenNode(NodeType type) : Node(type, NODE_CTYPE_TOKEN) {}
		TokenNode(NodeType type, Token token) : Node(type, NODE_CTYPE_TOKEN), m_token(token) {}
		const Token& getToken() const { return m_token; }
		void setToken(Token t) { m_token = t; }

	private:
		Token m_token;
	};

	struct EntityNode : public Node
	{
		EntityNode(NodeType type, Node* entity) : Node(type, NODE_CTYPE_ENTITY), entity(entity) {}
		Node* getEntity() const	{ return entity; }

	private:
		Node* entity;
	};

}

#endif
