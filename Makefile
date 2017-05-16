SRCDIR=src
BUILDDIR=build
BINDIR=bin
SWIGDIR=swig
CC=g++ $(CFLAGS)
CFLAGS=-std=c++14 -Wall -fPIC
PYINC=$(shell pkg-config --cflags $(pv))
PYLIB=$(shell pkg-config --libs $(pv))
.PHONY: all ruleparser python clean

all: ruleparser

ruleparser: $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
	$(CC) $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o -o $(BINDIR)/rule_parser

python: $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
	swig -outdir $(SWIGDIR) -c++ -python $(SWIGDIR)/interface.i
	$(CC) -c $(SWIGDIR)/interface_wrap.cxx -o $(BUILDDIR)/interface_wrap.o -I$(SRCDIR) $(PYINC)
	$(CC) -c $(SWIGDIR)/interface.cpp -o $(BUILDDIR)/interface.o -I$(SRCDIR)
	$(CC) -shared $(BUILDDIR)/tokenizer.o $(BUILDDIR)/parser.o $(BUILDDIR)/interface_wrap.o $(BUILDDIR)/interface.o -o $(SWIGDIR)/_RuleParser.so $(PYLIB)

clean:
	rm $(BUILDDIR)/* $(BINDIR)/*

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -MMD -c $< -o $@

-include $(BUILDDIR)/*.d
