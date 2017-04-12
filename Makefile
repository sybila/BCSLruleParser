CODE_DIR=src
CC=g++
CFLAGS=-std=c++14 -Wall
LDFLAGS=
SOURCES=main.cpp parser.cpp tokenizer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rule_parser

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -C $(CODE_DIR) $(CFLAGS) $< -o $@

clean:
	rm *o rule_parser
