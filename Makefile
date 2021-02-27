APP = basalt

FLEX = flex
BISON = bison

LINK.cc=c++

CXXFLAGS        += -std=c++17 -g -I. 
BASALT_LDFLAGS  += -g  
BASALT_LDLIBS   +=  

OBJDIR = ./obj
DEPDIR = ./.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SRCS_CC = basalt.cc \
          parser/ast.cc $(OBJDIR)/mbasic.lex.cpp $(OBJDIR)/mbasic.tab.cpp \
	        common.cc 

#	        codegen.cc c/c_codegen.cc c/c_runtime.cc \
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

else

$(OBJDIR)/%.o : %.c | $(DEPDIR) $(OBJDIR)
	@echo "(CC) $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<
				
%.o : %.cc
$(OBJDIR)/%.o : %.cc | $(DEPDIR) $(OBJDIR)
	@echo "(CXX) $<"
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

endif

FILENAMES := $(notdir $(basename $(SRCS_C) $(SRCS_CC)))
OBJS	    := $(addsuffix .o,$(addprefix $(OBJDIR)/,$(FILENAMES)))
DEPFILES  := $(addsuffix .d,$(addprefix $(DEPDIR)/,$(FILENAMES)))

vpath %.c  $(sort $(dir $(SRCS_C)))
vpath %.cc $(sort $(dir $(SRCS_CC)))

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
	rm -f basalt $(OBJDIR)/*

$(OBJDIR)/mbasic.lex.o: $(OBJDIR)/mbasic.lex.cpp $(OBJDIR)/mbasic.tab.hpp
	g++ -c $(CXXFLAGS) $(OBJDIR)/mbasic.lex.cpp -Wno-unused-function -Wno-sign-compare -o $@

$(OBJDIR)/mbasic.lex.cpp: parser/mbasic.l
	$(FLEX) -o $(OBJDIR)/mbasic.lex.cpp parser/mbasic.l

$(OBJDIR)/mbasic.tab.cpp $(OBJDIR)/mbasic.tab.hpp: parser/mbasic.ypp
	$(BISON) -o $(OBJDIR)/mbasic.tab.cc -v -d parser/mbasic.ypp	

include $(wildcard $(DEPFILES))