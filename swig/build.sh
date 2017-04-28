swig -c++ -python interface.i
g++ -c -std=c++14 -fPIC interface_wrap.cxx -I/usr/include/python3.5 -I/home/lachtan/rule-parser/src
g++ -c -std=c++14 -fPIC interface.cpp -I/home/lachtan/rule-parser/src
g++ -shared -std=c++14 -fPIC ../build/tokenizer.o ../build/parser.o interface_wrap.o interface.o -o _RuleParser.so -L/usr/lib/python3.5
