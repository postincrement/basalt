
FLEX = flex
BISON = bison

LINK.cc=c++

CXXFLAGS += -std=c++11 -g `llvm-config --cxxflags` 
BASALT_LDFLAGS  += -g `llvm-config --ldflags` 
BASALT_LDLIBS   += `llvm-config --libs --system-libs all` 

all: basalt libbasaltrt.a

basalt: basalt.o mbasic.lex.o mbasic.tab.o ast.o cg_dump.o cg_cxx.o cg_llvm.o
	g++ $^ $(LOADLIBES) $(BASALT_LDFLAGS) $(BASALT_LDLIBS) -o $@

basalt.o mbasic.lex.o mbasic.tab.o codegen.o: codegen.h

libbasaltrt.a: basaltrt.o
	ar rcs libbasaltrt.a basaltrt.o

clean:
	rm -f basalt basalt.o ast.o mbasic.lex.o mbasic.tab.o codegen.o basaltrt.o libbasaltrt.a mbasic.lex.cpp mbasic.tab.cpp

mbasic.lex.o: mbasic.lex.cpp mbasic.tab.hpp
	g++ -c $(CXXFLAGS) mbasic.lex.cpp -Wno-unused-function -Wno-sign-compare -o $@

mbasic.lex.cpp: mbasic.l
	$(FLEX) -o mbasic.lex.cpp mbasic.l

mbasic.tab.cpp mbasic.tab.hpp: mbasic.ypp
	$(BISON) -v -d mbasic.ypp	
