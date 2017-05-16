Requirements: swig, g++ v. 5.x, python-x-dev

usage: rule_parser \<communication_type\> \<input_type\> \<output_type\>
- communication_type: json
- input_type: stream, file:\<path\>, eq:"equation string"
- output_type: stream, file:\<path\>

Each equation to parse is one line on the input  
For each line on the input there is a line on the output containing JSON response: either **SEMICOLON** node or {"error":1, "start": int, "unexpected": "token name", "expected": ["token", "names"]}

Every node has "type" and "children" values, some of them have "token" or "entity" values.
- "type": string
- "children": list of child nodes
- "token": string
- "entity": child node

There are 11 possible node types:
- **SEMICOLON** = 9 (items split with ;, 1 or 2 children, **EQUATION** always, **VARIABLE** optional)

_
- **EQUATION** = 1 (always first child of **SEMICOLON**, items split with either <=> or =>, 1 or 2 children of type **EQUATION_SIDE**)
- **EQUATION_SIDE** = 2 (items split with +, 1 or more children of type **STOICHIOMETRY**)
- **STOICHIOMETRY** = 3 (has token, 1 child of type **RULE_AGENT**)
- **RULE_AGENT** = 4 (items split with ::, 1 or more children of type **FULL_COMPOSITION**)
- **FULL_COMPOSITION** = 5 (items split with ., 1 or more children of type **PARTIAL_COMPOSITION**)
- **PARTIAL_COMPOSITION** = 6 (has entity SIMPLE, format E(A|B), children are in brackets, 0 or more children of type **SIMPLE**)
- **SIMPLE** = 8 (has token, 0 or 1 children of type STATE)
- **STATE** = 7 (has token, 0 children)

_
- **VARIABLE** = 10 (always second child of **SEMICOLON** if present, has token, 1 or more children of type **VARIABLE_ENTITY**)
- **VARIABLE_ENTITY** = 11 (has token, 0 children)

Every one of **SEMICOLON**, **EQUATION**, **EQUATION_SIDE**, **STOICHIOMETRY**, **RULE_AGENT**, **FULL_COMPOSITION**, **PARTIAL_COMPOSITION**, **SIMPLE** nodes is present at least once in each valid equation tree.
