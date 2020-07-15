#include "../basalt.h"

#include "codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

using namespace std;

#define TEMP_PREFIX     "temp_"

struct AsmTypeInfoRec {
  const char * m_asmtype; 
  const char * m_initializer;
  const char * m_printFn;
  const char * m_strFn;
  const char * m_suffix;
};

// must be indexed by VarType
static AsmTypeInfoRec g_varTypeInfo[] = {
  { 0 },                        // none
  { "dw",     "0", "basalt_print_int16",  "basalt_str_int16",  "i16"  },     // eInt16
  { "dw",     "0", "basalt_print_int32",  "basalt_str_int32",  "i32"  },     // eInt32
  { "dw",     "0", "basalt_print_single", "basalt_str_single", "f"    },    // eSingle
  { "dw",     "0", "basalt_print_double", "basalt_str_double", "d"    },    // eDouble
  { "dw",     "0", "basalt_print_string", 0,                   "s"    }     // eString
};

////////////////////////////////////////////////////////////

Z80_CodeGenerator::Z80_CodeGenerator ()
  : CodeGenerator(
    {
      ".asm", TEMP_PREFIX, "", ""
    })
{
}

static bool CreateAVar(const std::string & name,
                       const AST::VarInfo & var, 
                       Z80_CodeGenerator::AsmVarDef & avar,
                       int tag)
{
  std::string str;
  stringstream strm;
  {
    for (auto r : name) {
      if (isalnum(r))
        strm << r;
    }
    str = strm.str().substr(0, 3);
  }

  strm.str("");
  strm << "v_" << str;
  if (tag != 0)
    strm << tag;
  strm << "_" << g_varTypeInfo[(int)var.m_type].m_suffix;
  avar.m_aname = strm.str();
  avar.m_type  = var.m_type;

  return true;
} 


bool Z80_CodeGenerator::Body()
{
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
    AsmVarDef avar;
    for (;;) {
      if (!CreateAVar(r.first, info, avar, tag)) {
        cerr << "internal error: cannot map var to asm type" << endl;
        return false;
      }
      if (m_anames.count(avar.m_aname) == 0)
        break;
      ++tag;  
    }
    m_globalVars[r.first] = avar;
    m_anames.insert(avar.m_aname);
  }

  *m_outputStream
           << "; Z80 generator by basalt\n"
           << "\n"
           ;

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    //*m_outputStream << "struct LineFunction " LINEFN_PREFIX << i << "(); /* " << r->GetBasicLineNumber() << " */\n";
  }

  *m_outputStream << "\n"
                  << "; CP/M BDOS entry point\n"
                  << "wboot:\tequ\t0\n"
                  << "bdos:\tequ\t5\n"
                  << "\n"
                  << "\torg\t0x100\n"
                  << "\n"
                  ;

  stringstream code;
  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    Push();
    code << "line_" << r->GetSourceLineNumber() << ":\n"; 
    r->Generate(*this);
    code << Pop();
  }

  m_usePrintStr  = m_usePrintStr  || m_usePrintNewLine;
  m_usePrintChar = m_usePrintChar || m_usePrintStr || m_usePrintI16;
  m_useDiv10     = m_useDiv10 || m_usePrintI16;

  if (m_usePrintNewLine) {
    if (AST::g_stringConstants.count("\r\n") == 0) {
      int index = AST::g_stringConstantIndex++;
      AST::g_stringConstants["\r\n"] = index;
    }
  }

  if (AST::g_stringConstants.size() > 0) {
    *m_outputStream << "\tjp\tstart\n"
                    << "; String constants\n"
                    ;
    for (auto & r : AST::g_stringConstants) {
      *m_outputStream << "str_" << r.second << ":";
      const std::string & str = r.first;
      int i = 0;
      std::string prefix = "\t";
      if (!isprint(str[i])) {
        prefix += "db\t";
        while (i < str.length()) {
          *m_outputStream << prefix << "0x" << setw(2) << setfill('0') << hex << (int)str[i] << dec;
          prefix = ", ";
          ++i;
        }
      }
      else {
        int start = i;
        while ((i < str.length()) && (isprint(str[i])))
          ++i;
        *m_outputStream << prefix << "dm\t\"" << str.substr(start, i-start) << "\"";
      } 
      *m_outputStream << "\n";
      prefix = "\t";
    }
    *m_outputStream << "\n"
                    << "start:\n"
                    ;
  }
  *m_outputStream << "  ld    hl,(wboot+1)\n"
                  << "  ld    de,9\n"
                  << "  add   hl,de\n"
                  << "  ld    (conout+1),hl\n"
                  << "  ld    sp,0x100\n"
                  ;

  *m_outputStream << code.str();

  *m_outputStream << "\n"
                  << "end:\n"
                  << "  rst   0\n";

  if (m_usePrintI16) {
    *m_outputStream << "\n"
                    << "; print int16 in HL\n"
                    << "print_i16:\n"
                    << "  ld    a,' '\n"
                    << "  bit   7,h   ; check if negative\n"
                    << "  jr    z,print_i16p\n"
                    << "  ld    de,0\n"
                    << "  ex    de,hl\n"
                    << "  xor   a\n"
                    << "  sbc   hl,de\n"
                    << "  ld    a,'-'\n"
                    << "print_i16p:\n"
                    << "  call    print_ch\n"
                    << "print_i16n:\n"
                    << "  ld    a,h\n"
                    << "  or    a\n"
                    << "  jr    nz,print_i16s\n"
                    << "  ld    a,l\n"
                    << "  cp    10\n"
                    << "  jr    c,print_i16r\n"
                    << "print_i16s:\n"
                    << "  call  div_10\n"
                    << "  push  af\n"
                    << "  call  print_i16n\n"
                    << "  pop   af\n"
                    << "print_i16r:\n"
                    << "  add   a,'0'\n"
                    << "  jp    print_ch\n"
                    ;
  }

  if (m_usePrintNewLine) {
    int index = AST::g_stringConstants["\r\n"];  
    *m_outputStream << "\n"
                    << "; print newline\n"
                    << "print_newline:\n"
                    << "  ld    hl,str_" << index << "\n"    
                    << "  ld    bc,2\n"
                    << "; fall through to print_str\n"
                    ;
  }                

  if (m_usePrintStr) {
    *m_outputStream << "\n"
                    << "; print string at HL with BC chars\n"
                    << "print_str:\n"
                    << "  ld    a,(hl)\n"
                    << "  call  print_ch\n"
                    << "  dec   bc\n"
                    << "  inc   hl\n"
                    << "  ld    a,c\n"
                    << "  or    b\n"
                    << "  jr    nz,print_str\n"
                    << "  ret\n"
                    ;
  }

  if (m_usePrintChar) {                 
    *m_outputStream << "\n"
                    << "; print char in A\n"
                    << "print_ch:\n"
                    << "  push  bc\n"
                    << "  push  hl\n"
                    << "  ld    c,a\n"
                    << "  call  conout\n"
                    << "  pop   hl\n"
                    << "  pop   bc\n"
                    << "  ret\n"
                    ;
  }


  if (m_useDiv10) {
    *m_outputStream << "; Divide HL by 10.\n"
                    << "; HL quotient, A = remainder\n" 
                    << "div_10:\n"
                    << "  ld    bc,0x0d0a\n"
                    << "  xor   a\n"
                    << "  add   hl,hl\n"
                    << "  rla\n"
                    << "  add   hl,hl\n"
                    << "  rla\n"
                    << "  add   hl,hl\n"
                    << "  rla\n"
                    << "div_10_1:\n"
                    << "  add   hl,hl\n"
                    << "  rla\n"
                    << "  cp    c\n"
                    << "  jr    c,div_10_2\n"
                    << "  sub   c\n"
                    << "  inc   l\n"
                    << "div_10_2:\n"
                    << "  djnz  div_10_1\n"
                    << "  ret\n";
  }   

  *m_outputStream << "\n"
                  << "; Vars\n"
                  << "conout: jp    0 ; replaced with address of BIOS conout\n"
                  ;         


  for (auto & r : m_globalVars) {
    AsmTypeInfoRec & info = g_varTypeInfo[(int)r.second.m_type];
    *m_outputStream << r.second.m_aname << ":\t"
                    << info.m_asmtype
                    << "\t"
                    << info.m_initializer
                    << "\t; " << r.first << "\n"
                    ;
  }
  *m_outputStream << "\n";

  return true;           
}

int Z80_CodeGenerator::Generate(const AST::SourceLine & line)
{
  TopOutput(false) << "; " << line.GetLine() << "\n";
  return CodeGenerator::Generate(line);
}


int Z80_CodeGenerator::Generate(const AST::End & expr)
{
  TopOutput() << "\tjp\tend\n";
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::StringConstant & expr)
{
  int index = AST::g_stringConstants[expr.GetValue()];
  TopOutput(false) << " ld    hl,str_" << index << "\n"
                   << " ld    bc," << expr.GetValue().length() << "\n"
                   << " call  print_str\n";

  m_usePrintStr = true;                

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  TopOutput(false) << " ld    hl," << expr.GetValue() << "\n"
                   << " call  print_i16\n"
                   ;

  m_usePrintI16 = true;                

  return 0;
}

///////////////////////////////////////////////////////////////////

bool Z80_CodeGenerator::LookupGlobalVar(const std::string & varName, 
                                       AsmVarDef & avar)
{
  if (m_globalVars.count(varName) == 0)
    return false;

  avar = m_globalVars[varName];
  return true;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];
  result = avar.m_aname;  
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::Print & expr)
{
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }
  m_usePrintNewLine = true;                
  TopOutput(false) << " call  print_newline\n";
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::Goto & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0) {
    cerr << "internal error: cannot resolve goto destination '" << expr.GetRef() << "'" << endl;
    return 1;
  }

  TopOutput(false) << " jp    line_" << line << "\n"; 
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), avar))
    return -1;

  if (expr.m_rhs == nullptr) {
    if (!g_disableWarnings)
      us.Output() << "#warning \"missing rhs\"\n";
    return -1;
  }

  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);

  if (expr.m_rhs->GetType() == VarType::eInt16) {
    if (expr.m_rhs->IsConstant()) {
      us.Output() << "  ld    hl," << rhs << "\n"
                  << "  ld    (" << avar.m_aname << "),hl\n" << endl;
    }
    else if (expr.m_rhs->IsVarRef()) {
      us.Output() << "  ld    hl,(" << rhs << ")\n"
                  << "  ld    (" << avar.m_aname << "),hl\n" << endl;
    }
  }

  return 0;
}
