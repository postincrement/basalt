APP = basalt

FLEX = flex
BISON = bison

LINK.cc=c++

CXXFLAGS        += -std=c++17 -g  
BASALT_LDFLAGS  += -g  
BASALT_LDLIBS   +=  

OBJDIR = ./obj
DEPDIR = ./.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SRCS_CC = basalt.cc \
          mbasic.lex.cc mbasic.tab.cc \
	        ast.cc \
	        codegen.cc common.cc \
	        c/c_codegen.cc \
	        z80/z80_codegen.cc 

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

all: $(APP) libbasaltrt.a

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(OBJDIR):
	@mkdir -p $@

$(DEPFILES): $(DEPDIR)

$(DEPDIR):
	@mkdir -p $@

############################################################

libbasaltrt.a: basaltrt.o
	ar rcs libbasaltrt.a basaltrt.o

clean:
	rm -f basalt $(OBJS) libbasaltrt.a mbasic.lex.cpp mbasic.tab.cpp

mbasic.lex.o: mbasic.lex.cpp mbasic.tab.hpp
	g++ -c $(CXXFLAGS) mbasic.lex.cpp -Wno-unused-function -Wno-sign-compare -o $@

mbasic.lex.cc: mbasic.l
	$(FLEX) -o mbasic.lex.cc mbasic.l

mbasic.tab.cc mbasic.tab.hpp: mbasic.ypp
	$(BISON) -o mbasic.tab.cc -v -d mbasic.ypp	

include $(wildcard $(DEPFILES))