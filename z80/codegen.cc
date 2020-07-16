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

#define PrintCh_FUNC        "print_ch"
#define PrintTab_FUNC       "print_tab"
#define PrintNewline_FUNC   "print_newline"
#define PrintI16_FUNC       "print_i16"
#define PrintI16s_FUNC      "print_i16s"
#define PrintI32_FUNC       "print_i32"
#define PrintF_FUNC         "print_f"
#define PrintD_FUNC         "print_d"
#define PrintStr_FUNC       "print_str"
#define Div10_FUNC          "div10"


// must be indexed by VarType
static AsmTypeInfoRec g_varTypeInfo[] = {
  { 0 },                        // none
  { "dw",     "0", PrintI16s_FUNC, "basalt_str_int16",  "i16"  },    // eInt16
  { "dw",     "0", PrintI32_FUNC,  "basalt_str_int32",  "i32"  },    // eInt32
  { "dw",     "0", PrintF_FUNC,    "basalt_str_single", "f"    },    // eSingle
  { "dw",     "0", PrintD_FUNC,    "basalt_str_double", "d"    },    // eDouble
  { "dw",     "0", PrintStr_FUNC,  0,                   "s"    }     // eString
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

  *m_outputStream << ""
                  << "wboot: equ 0  ; warm boot\n"
                  << "bdos:  equ 5  ; CP/M BDOS\n"
                  << "\n"
                  << "    org  0x100\n"
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

  #define USED(s) m_funcsUsed.count(s##_FUNC)

  if (USED(PrintNewline))
    m_funcsUsed.insert(PrintStr_FUNC);

  if (USED(PrintI16s))  
    m_funcsUsed.insert(PrintI16_FUNC);

  if (USED(PrintStr) || USED(PrintI16) || USED(PrintTab))
    m_funcsUsed.insert(PrintCh_FUNC);

  if (USED(PrintI16))
    m_funcsUsed.insert(Div10_FUNC);
  
  if (USED(PrintNewline)) {
    if (AST::g_stringConstants.count("\r\n") == 0) {
      AST::g_stringConstants["\r\n"] = AST::g_stringConstantIndex++;
    }
  }

  *m_outputStream << "start:\n"
                  << "    ld    ix,vars\n"
                  << "    ld    hl,(bdos+1)\t; set SP to just below BDOS\n"
                  << "    ld    sp,hl\n"
                  << "    ld    hl,(wboot+1)\t; get BIOS conout vector\n"
                  << "    ld    de,9\n"
                  << "    add   hl,de\n"
                  << "    ld    (conout+1),hl\n"
                  ;

  *m_outputStream << code.str();

  *m_outputStream << "end:\n"
                  << "    rst   0  ; warm boot\n"
                  << "\n"
                  ;

  *m_outputStream << "; Functions\n"
                  << "\n"
                  ;

  if (USED(PrintI16s)) {
    *m_outputStream << "; print int16 in HL with trailing space\n"
                    << PrintI16s_FUNC ":\n"
                    << "    call  " PrintI16_FUNC "\n"
                    << "    ld    a,' '\n"
                    << "    jp    " PrintCh_FUNC "\n"
                    << "\n"
                    ;
  }
  if (USED(PrintI16)) {
     *m_outputStream << "; print int16 in HL\n"
                    << PrintI16_FUNC ":\n"
                    << "    ld    a,' '\n"
                    << "    bit   7,h   ; check if negative\n"
                    << "    jr    z,print_i16p\n"
                    << "    ld    de,0\n"
                    << "    ex    de,hl\n"
                    << "    xor   a\n"
                    << "    sbc   hl,de\n"
                    << "    ld    a,'-'\n"
                    << "print_i16p:\n"
                    << "    call    " PrintCh_FUNC "\n"
                    << "print_i16n:\n"
                    << "    ld    a,h\n"
                    << "    or    a\n"
                    << "    jr    nz,print_i16q\n"
                    << "    ld    a,l\n"
                    << "    cp    10\n"
                    << "    jr    c,print_i16r\n"
                    << "print_i16q:\n"
                    << "    call  " Div10_FUNC "\n"
                    << "    push  af\n"
                    << "    call  print_i16n\n"
                    << "    pop   af\n"
                    << "print_i16r:\n"
                    << "    add   a,'0'\n"
                    << "    jp    " PrintCh_FUNC "\n"
                    << "\n"
                    ;
  }

  if (USED(PrintNewline)) {
    int index = AST::g_stringConstants["\r\n"];  
    *m_outputStream << "; print newline\n"
                    << PrintNewline_FUNC ":\n"
                    << "    ld    hl,str_" << index << "\n"    
                    << "    ld    bc,2\n"
                    << "    call  print_str\n"
                    << "    ld    (ix+column-vars),0\n"
                    << "    ret\n"
                    << "\n"
                    ;
  }                

  if (USED(PrintStr)) {
    *m_outputStream << "; print string at HL with BC chars\n"
                    << PrintStr_FUNC ":\n"
                    << "    ld    a,(hl)\n"
                    << "    call  " PrintCh_FUNC "\n"
                    << "    dec   bc\n"
                    << "    inc   hl\n"
                    << "    ld    a,c\n"
                    << "    or    b\n"
                    << "    jr    nz,print_str\n"
                    << "    ret\n"
                    << "\n"
                    ;
  }

  if (USED(PrintTab)) {
    *m_outputStream << "; print tab with expnsion\n"
                    << "print_tab:\n"
                    << "    ld    a,(ix+column-vars)\n"
                    << "    ld    b,(ix+tabwid-vars)\n"
                    << "print_tab1:\n"
                    << "    cp    b\n"
                    << "    jr    c,print_tab2\n"
                    << "    sub   b\n"
                    << "    jr    print_tab1\n"
                    << "print_tab2:\n"
                    << "    ld    b,a\n"
                    << "    ld    a,(ix+tabwid-vars)\n"
                    << "    sub   b\n"
                    << "    ld    b,a\n"
                    << "    ld    a,' '\n"
                    << "print_tab3:\n"
                    << "    call  " PrintCh_FUNC "\n"
                    << "    djnz  print_tab3\n"
                    << "    ret\n"
                    << "\n"
                    ;
  }

  if (USED(PrintCh)) {
    *m_outputStream << "; print char in A\n"
                    <<  PrintCh_FUNC ":\n"
                    << "    inc   (ix+column-vars)\n"
                    << "print_chn:\n"
                    << "    push  bc\n"
                    << "    push  hl\n"
                    << "    ld    c,a\n"
                    << "    call  conout\n"
                    << "    pop   hl\n"
                    << "    pop   bc\n"
                    << "    ret\n"
                    << "\n";
  }

  if (USED(Div10)) {
    *m_outputStream << "; Divide HL by 10.\n"
                    << "; HL quotient, A = remainder\n" 
                    << Div10_FUNC ":\n"
                    << "    ld    bc,0x0d0a\n"
                    << "    xor   a\n"
                    << "    add   hl,hl\n"
                    << "    rla\n"
                    << "    add   hl,hl\n"
                    << "    rla\n"
                    << "    add   hl,hl\n"
                    << "    rla\n"
                    << "div_10_1:\n"
                    << "    add   hl,hl\n"
                    << "    rla\n"
                    << "    cp    c\n"
                    << "    jr    c,div_10_2\n"
                    << "    sub   c\n"
                    << "    inc   l\n"
                    << "div_10_2:\n"
                    << "    djnz  div_10_1\n"
                    << "    ret\n"
                    << "\n"
                    ;
  }

  if (AST::g_stringConstants.size() > 0) {
    *m_outputStream << "; String constants\n"
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
      prefix = " ";
    }
    *m_outputStream << "\n";
  }

  *m_outputStream << "; Vars\n"
                  << "vars:\n"
                  ;
  if (USED(PrintCh)) {
    *m_outputStream << "conout: jp    0  ; replaced with address of BIOS conout\n"
                    << "tabwid: db    " << g_languageProfile->GetTabWidth() << " ; tab width\n"
                    << "column: db    0  ; current tab column\n"
                    << "\n"
                    ;    
  }

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
  TopOutput() << "    jp    end\n";
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::StringConstant & expr)
{
  int index = AST::g_stringConstants[expr.GetValue()];
  TopOutput(false) << "    ld    hl,str_" << index << "\n"
                   << "    ld    bc," << expr.GetValue().length() << "\n"
                   << "    call  print_str\n";

  m_funcsUsed.insert(PrintStr_FUNC);                

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  TopOutput(false) << "    ld    hl," << expr.GetValue() << "\n"
                   << "    call  " PrintI16s_FUNC "\n"
                   ;

  m_funcsUsed.insert(PrintI16s_FUNC);

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::Print & expr)
{
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }

  if ((expr.m_list.size() > 0) && !expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon()) {
    TopOutput(false) << "    call  " << PrintNewline_FUNC << "\n";
    m_funcsUsed.insert(PrintNewline_FUNC);
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput(false) << "    call  print_tab\n";
  m_funcsUsed.insert(PrintTab_FUNC);
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

  TopOutput(false) << "    jp    line_" << line << "\n"; 
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
      us.Output() << "    ld    hl," << rhs << "\n"
                  << "    ld    (" << avar.m_aname << "),hl\n" << endl;
    }
    else if (expr.m_rhs->IsVarRef()) {
      us.Output() << "    ld    hl,(" << rhs << ")\n"
                  << "    ld    (" << avar.m_aname << "),hl\n" << endl;
    }
  }

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

int Z80_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;

  m_funcsUsed.insert(funcName);

  TopOutput(false) << "    ld      hl,(" << avar.m_aname << ")\n"
                   << "    call    " << funcName << "\n"
                   ;  
  return 0;
}
