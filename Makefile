SRCDIR=src
BUILDDIR=build
BINDIR=bin
CC=g++ $(CFLAGS)
CFLAGS=-std=c++14 -Wall
.PHONY: all ruleparser clean

all: ruleparser

ruleparser: $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
	$(CC) $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o -o $(BINDIR)/rule_parser

clean:
	rm $(BUILDDIR)/* $(BINDIR)/*

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@
