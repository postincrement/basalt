
FLEX = flex
BISON = bison

CC=c++

CXXFLAGS += -std=c++11 -g `llvm-config --cxxflags` 
LDFLAGS  += `llvm-config --ldflags` 
LDLIBS   += `llvm-config --libs --system-libs all` 

basalt: basalt.o mbasic.lex.o mbasic.tab.o codegen.o

clean:
	rm -f basalt basalt.o mbasic.lex.o mbasic.tab.o codegen.o

mbasic.lex.o: mbasic.lex.cpp mbasic.tab.hpp

mbasic.lex.cpp: mbasic.l
	$(FLEX) -o mbasic.lex.cpp mbasic.l

mbasic.tab.cpp mbasic.tab.hpp: mbasic.ypp
	$(BISON) -d mbasic.ypp	
