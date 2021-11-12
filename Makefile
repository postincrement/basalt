APP = basalt

FLEX = /opt/homebrew/opt/flex/bin/flex
BISON = /opt/homebrew/opt/bison/bin/bison

LINK.cc=c++

CXXFLAGS        += -std=c++17 -g -I. -I./src
BASALT_LDFLAGS  += -g  
BASALT_LDLIBS   +=  

#  export LDFLAGS="-L/opt/homebrew/opt/flex/lib"
#  export CPPFLAGS="-I/opt/homebrew/opt/flex/include"

OBJDIR = ./obj
DEPDIR = ./.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SRCS_CC = src/basalt.cc src/pass.cc src/common.cc src/codegen.cc \
          parser/ast.cc parser/mbasic.lex.cpp parser/mbasic.tab.cpp \
		  		pretty/pretty_codegen.cc c/c_codegen.cc 
	         
#	        codegen.cc c/c_runtime.cc \
#	        z80/z80_codegen.cc 

############################################################

COMPILE.c  = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c

ifeq ($(VERBOSE),1)

%.o : %.c 
$(OBJDIR)/%.o : %.c | $(DEPDIR) $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<
				
%.o : %.cc
$(OBJDIR)/%.o : %.cc | $(DEPDIR) $(OBJDIR)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

%.o : %.cpp
$(OBJDIR)/%.o : %.cpp | $(DEPDIR) $(OBJDIR)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

else

$(OBJDIR)/%.o : %.c | $(DEPDIR) $(OBJDIR)
	@echo "(CC) $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<
				
%.o : %.cc
$(OBJDIR)/%.o : %.cc | $(DEPDIR) $(OBJDIR)
	@echo "(CXX) $<"
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

%.o : %.cpp
$(OBJDIR)/%.o : %.cpp | $(DEPDIR) $(OBJDIR)
	@echo "(CXX) $<"
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

endif

FILENAMES := $(notdir $(basename $(SRCS_C) $(SRCS_CC)))
OBJS	    := $(addsuffix .o,$(addprefix $(OBJDIR)/,$(FILENAMES)))
DEPFILES  := $(addsuffix .d,$(addprefix $(DEPDIR)/,$(FILENAMES)))

vpath %.c  $(sort $(dir $(SRCS_C)))
vpath %.cc $(sort $(dir $(SRCS_CC)))
vpath %.cpp $(sort $(dir $(SRCS_CC)))

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(OBJDIR):
	@mkdir -p $@

$(DEPFILES): $(DEPDIR)

$(DEPDIR):
	@mkdir -p $@

############################################################

clean: 
	rm -rf basalt .deps $(OBJDIR)/* \
	parser/mbasic.tab.cpp \
	parser/mbasic.tab.hpp \
	parser/mbasic.lex.cpp

$(OBJDIR)/mbasic.lex.o: parser/mbasic.lex.cpp parser/mbasic.tab.hpp
#	$(CXX) -c $(CXXFLAGS) parser/mbasic.lex.cc -Wno-unused-function -Wno-sign-compare -o $@

$(OBJDIR)/mbasic.tab.o: parser/mbasic.tab.cpp parser/mbasic.tab.hpp

parser/mbasic.lex.cpp: parser/mbasic.l
	$(FLEX) -o parser/mbasic.lex.cpp parser/mbasic.l

parser/mbasic.tab.cpp parser/mbasic.tab.hpp: parser/mbasic.ypp
	$(BISON) -o parser/mbasic.tab.cpp -v -d parser/mbasic.ypp 

include $(wildcard $(DEPFILES))
