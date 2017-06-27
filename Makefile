SRCDIR=src
BUILDDIR=build
BINDIR=bin
SWIGDIR=swig
CC=g++ $(CFLAGS)
CFLAGS=-std=c++14 -fPIC -O2
python_INC=$(shell pkg-config --cflags $(pv))
python_LIB=$(shell pkg-config --libs $(pv))
php_INC=$(shell php-config --includes)
php_LIB=$(shell php-config --ldflags)
LANGUAGES=php python
#$(shell ls -d $(SWIGDIR)/*/)
.PHONY: all ruleparser python clean

all: ruleparser

ruleparser: $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
	$(CC) $(BUILDDIR)/main.o $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o -o $(BINDIR)/rule_parser

# python: $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
# 	swig -cppext cpp -outcurrentdir $(BUILDDIR)/$@ -outdir $(SWIGDIR)/$@ -c++ -$@ $(SWIGDIR)/interface.i
# 	$(CC) -c $(SWIGDIR)/interface_wrap.cxx -o $(BUILDDIR)/interface_wrap.o -I$(SRCDIR) $(PYINC)
# 	$(CC) -c $(SWIGDIR)/interface.cpp -o $(BUILDDIR)/interface.o -I$(SRCDIR) -I$(SWIGDIR)
# 	$(CC) -shared $(BUILDDIR)/tokenizer.o $(BUILDDIR)/parser.o $(BUILDDIR)/interface_wrap.o $(BUILDDIR)/interface.o -o $(SWIGDIR)/_RuleParser.so $(PYLIB)

# php: $(BUILDDIR)/parser.o $(BUILDDIR)/tokenizer.o
# 	swig -cppext cpp -outcurrentdir $(BUILDDIR)/$@ -outdir $(SWIGDIR)/$@ -c++ -$@ $(SWIGDIR)/interface.i
# 	$(CC) -c $(SWIGDIR)/interface_wrap.cpp -o $(BUILDDIR)/interface_wrap.o -I$(SRCDIR) $(PHPINC)
# 	$(CC) -c $(SWIGDIR)/interface.cpp -o $(BUILDDIR)/interface.o -I$(SRCDIR) -I$(SWIGDIR)
# 	$(CC) -shared $(BUILDDIR)/tokenizer.o $(BUILDDIR)/parser.o $(BUILDDIR)/interface_wrap.o $(BUILDDIR)/interface.o -o $(SWIGDIR)/RuleParser.so $(PHPLIB)

$(LANGUAGES):
	mkdir -p $(BUILDDIR)/$@
	swig -cppext cpp -outdir $(SWIGDIR)/$@ -c++ -$@ $(SWIGDIR)/$@/interface.i
	$(CC) -c $(SWIGDIR)/$@/interface_wrap.cpp -o $(BUILDDIR)/$@/interface_wrap.o -I$(SWIGDIR) -I$(SRCDIR) $($@_INC)
	$(CC) -c $(SWIGDIR)/interface.cpp -o $(BUILDDIR)/$@/interface.o -I$(SRCDIR) -I$(SWIGDIR)/$@
	$(CC) -shared $(BUILDDIR)/tokenizer.o $(BUILDDIR)/parser.o $(BUILDDIR)/$@/interface_wrap.o $(BUILDDIR)/$@/interface.o -o $(SWIGDIR)/$@/RuleParser.so $($@_LIB)

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/*

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -MMD -c $< -o $@

-include $(BUILDDIR)/*.d
