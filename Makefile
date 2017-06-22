
FLEX = flex
BISON = bison

LINK.cc=c++

CXXFLAGS += -std=c++11 -g `llvm-config --cxxflags` 
BASALT_LDFLAGS  += `llvm-config --ldflags` 
BASALT_LDLIBS   += `llvm-config --libs --system-libs all` 

all: basalt libbasaltrt.a

basalt: basalt.o mbasic.lex.o mbasic.tab.o codegen.o
	g++ $^ $(LOADLIBES) $(BASALT_LDFLAGS) $(BASALT_LDLIBS) -o $@

basalt.o mbasic.lex.o mbasic.tab.o codegen.o: codegen.h

libbasaltrt.a: basaltrt.o
	ar rcs libbasaltrt.a basaltrt.o

clean:
	rm -f basalt basalt.o mbasic.lex.o mbasic.tab.o codegen.o basaltrt.o libbasaltrt.a

mbasic.lex.o: mbasic.lex.cpp mbasic.tab.hpp

mbasic.lex.cpp: mbasic.l
	$(FLEX) -o mbasic.lex.cpp mbasic.l

mbasic.tab.cpp mbasic.tab.hpp: mbasic.ypp
	$(BISON) -d mbasic.ypp	
