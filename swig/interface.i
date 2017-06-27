%module RuleParser

%include "std_string.i"
%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%{
#include "../src/node.h"
#include "interface.h"
%}

%ignore RuleParser::Node::Node;
%ignore RuleParser::Node::getChildren;
%ignore RuleParser::Node::getClass;
%ignore RuleParser::TokenNode;
%ignore RuleParser::EntityNode;

%extend RuleParser::Node {
	Node* getChild(int i) {
		return self->getChildren().at(i).get();
	}

	int getNumChildren() {
		return self->getChildren().size();
	}

	std::string getTokenValue() {
		if (self->getClass() == RuleParser::NODE_CTYPE_TOKEN)
			return reinterpret_cast<RuleParser::TokenNode*>(self)->getToken().value;

		return {""};
	}

	Node* getNodeEntity() {
		if (self->getClass() == RuleParser::NODE_CTYPE_ENTITY)
			return reinterpret_cast<RuleParser::EntityNode*>(self)->getEntity();

		return nullptr;
	}
}

%include "../src/node.h"
%include "interface.h"
