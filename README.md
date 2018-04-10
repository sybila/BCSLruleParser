**Requirements**: cmake, swig, g++ v. 5.x, python-x-dev
**Requirements-JS**: Emscripten

Build using cmake

Options list:
WITH_BIN, WITH_PHP, WITH_PHP7, WITH_PYTHON - self-explanatory
WITH_JS/WITH_WASM - build into JavaScript as asm.js or WebAssembly (requires Emscripten)

Example build with binaries and python bindings (supposing you are in a source directory):

```mkdir build
mkdir bin
cd build
cmake -DWITH_BIN=1 -DWITH_PYTHON=1 -DCMAKE_INSTALL_PREFIX=../bin ../
make
make install```

Moreover, for `python 2` use `-DPYTHON_INCLUDE_DIR=/usr/include/python2.7 -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so`

Binaries are then in bin/main, python bindings in bin/lib/python

Example build with WebAssembly binary (assuming you are in a source directory and have activated emsdk environment):

```mkdir build
mkdir bin
cd build
cmake -DWITH_WASM=1 -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_INSTALL_PREFIX=../bin
make
make install```

Generated files are then in bin/js

---

usage: RuleParserBin \<communication_type\> \<input_type\> \<output_type\>
- communication_type: json, string
- input_type: stream, file:\<path\>, eq:"equation string"
- output_type: stream, file:\<path\>, string

Each equation to parse is one line on the input  
For each line on the input there is a line on the output containing JSON response: either **SEMICOLON** node or {"error":1, "start": int, "unexpected": "token name", "expected": ["token", "names"]}

Every node has "type" and "children" values, some of them have "token" or "entity" values.
- "type": string
- "children": list of child nodes
- "token": string
- "entity": child node

There are 11 possible node types:
- **SEMICOLON** = 9 (items split with ;, 2 children, **EQUATION** and **VARIABLE**)

Equation part:
- **EQUATION** = 1 (items split with either <=> or =>, 2 children of type **EQUATION_SIDE**)
- **EQUATION_SIDE** = 2 (items split with +, 0 or more children of type **STOICHIOMETRY**)
- **STOICHIOMETRY** = 3 (has token, 1 child of type **RULE_AGENT**)
- **RULE_AGENT** = 4 (items split with ::, 1 or more children of type **FULL_COMPOSITION**)
- **FULL_COMPOSITION** = 5 (items split with ., 1 or more children of type **PARTIAL_COMPOSITION**)
- **PARTIAL_COMPOSITION** = 6 (has entity SIMPLE, format E(A|B), children are in brackets, 0 or more children of type **SIMPLE**)
- **SIMPLE** = 8 (has token, 0 or 1 children of type STATE)
- **STATE** = 7 (has token, no children)

Variable part:
- **VARIABLE** = 10 (has token (may be empty), 0 or more children of type **VARIABLE_ENTITY**)
- **VARIABLE_ENTITY** = 11 (has token, no children)

Every one of **SEMICOLON**, **EQUATION**, **EQUATION_SIDE**, **STOICHIOMETRY**, **RULE_AGENT**, **FULL_COMPOSITION**, **PARTIAL_COMPOSITION**, **SIMPLE** nodes is present at least once in each valid equation tree.
