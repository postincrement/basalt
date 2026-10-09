#include "../basalt.h"

#include "z80_codegen.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdio>
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
#define PrintStrc_FUNC      "print_strc"
#define Div10_FUNC          "div10"
#define Equals16_FUNC       "equal_i16"
#define NotEquals16_FUNC    "nequal_i16"


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
      ".asm", TEMP_PREFIX, "", "", 4, 0
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
  for (auto & entry : AST::g_subscriptArity) {
    if (AST::g_userFunctions.count(entry.first) != 0)
      continue;
    if (AST::g_arrayBounds.count(entry.first) == 0)
      AST::g_arrayBounds[entry.first] = std::vector<int>(entry.second, 10);
  }

  // map names of all variables to assember-friendly ids
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

  if (m_useFloat)
    m_funcsUsed.insert(PrintCh_FUNC);

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
                  << "    ld    hl,(bdos+1)\t; set SP to just below BDOS\n"
                  << "    ld    sp,hl\n"
                  << "    ld    hl,(wboot+1)\t; get BIOS conout vector\n"
                  << "    ld    de,9\n"
                  << "    add   hl,de\n"
                  << "    ld    (conout+1),hl\n"
                  << "    ld    ix,temp\n"
                  << "    ld    iy,vars\n"
                  ;

  *m_outputStream << code.str();

  *m_outputStream << "end:\n"
                  << "    rst   0  ; warm boot\n"
                  << "\n"
                  ;

  *m_outputStream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "; Functions\n"
                  << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "\n"
                  ;

  if (USED(PrintI16s)) {
    *m_outputStream << "; print int16 in HL with trailing space\n"
                    << PrintI16s_FUNC ":\n"
                    << "    call  " PrintI16_FUNC "\n"
                    << "    ld    a,' '\n"
                    << "; fall through\n";
  }
  if (USED(PrintCh)) {
    *m_outputStream << "; print char in A\n"
                    <<  PrintCh_FUNC ":\n"
                    << "    inc   (ix+column-temp)\n"
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
                    << "    jr    " PrintCh_FUNC "\n"
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
                    << "    ld    (ix+column-temp),0\n"
                    << "    ret\n"
                    << "\n"
                    ;
  }                

  if (USED(PrintStr)) {
    *m_outputStream << "; print string at HL with C chars\n"
                    << PrintStrc_FUNC ":\n"
                    << "    ld    b,0\n"
                    << "; print string at HL with BC chars\n"
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
    *m_outputStream << "; print tab with expansion\n"
                    << "print_tab:\n"
                    << "    ld    a,(ix+column-temp)\n"
                    << "    ld    b,(ix+tabwid-temp)\n"
                    << "print_tab1:\n"
                    << "    cp    b\n"
                    << "    jr    c,print_tab2\n"
                    << "    sub   b\n"
                    << "    jr    print_tab1\n"
                    << "print_tab2:\n"
                    << "    ld    b,a\n"
                    << "    ld    a,(ix+tabwid-temp)\n"
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

  if (USED(NotEquals16) || USED(Equals16)) {
    *m_outputStream << "; Compare HL and DE: -1 = not, 0 equal\n"
                    << NotEquals16_FUNC ":\n"
                    << "    or    a\n"
                    << "    sbc   hl,de\n"
                    << "    ld    a,h\n"
                    << "    or    l\n"
                    << "    ret   z\n"
                    << NotEquals16_FUNC "2:\n"
                    << "    ld    hl,-1\n"
                    << "    ret\n"
                    << "\n";

    *m_outputStream << "; Compare HL and DE: 0 = not, -1 equal\n" 
                    << Equals16_FUNC ":\n"
                    << "    or    a\n"
                    << "    sbc   hl,de\n"
                    << "    ld    a,h\n"
                    << "    or    l\n"
                    << "    jr    z," << NotEquals16_FUNC "2\n"
                    << "    ld    hl,0\n"
                    << "    ret\n"
                    << "\n";
  }

  if (m_funcsUsed.count("streq")) {
    *m_outputStream << "streq:\n"
                    << "    ld    a,h\n"
                    << "    or    l\n"
                    << "    jr    nz,streq_l\n"
                    << "    ld    hl,streq_empty\n"
                    << "streq_l:\n"
                    << "    ld    a,d\n"
                    << "    or    e\n"
                    << "    jr    nz,streq_r\n"
                    << "    ld    de,streq_empty\n"
                    << "streq_r:\n"
                    << "    ld    a,(de)\n"
                    << "    cp    (hl)\n"
                    << "    jr    nz,streq_no\n"
                    << "    or    a\n"
                    << "    jr    z,streq_yes\n"
                    << "    inc   hl\n"
                    << "    inc   de\n"
                    << "    jr    streq_r\n"
                    << "streq_yes:\n"
                    << "    ld    hl,-1\n"
                    << "    ret\n"
                    << "streq_no:\n"
                    << "    ld    hl,0\n"
                    << "    ret\n"
                    << "streq_empty:\n"
                    << "    db    0\n\n";
  }

  if (m_funcsUsed.count("input") || m_funcsUsed.count("print_zstr")) {
    *m_outputStream <<
      "print_zstr:\n"
      "    ld    a,(hl)\n"
      "    or    a\n"
      "    ret   z\n"
      "    push  hl\n"
      "    call  print_ch\n"
      "    pop   hl\n"
      "    inc   hl\n"
      "    jr    print_zstr\n"
      "\n";
  }

  if (m_funcsUsed.count("input")) {
    *m_outputStream <<
      "in_pull:\n"
      "    ld    hl,(in_ptr)\n"
      "    ld    a,h\n"
      "    or    l\n"
      "    jr    z,in_read\n"
      "    ld    a,(hl)\n"
      "    or    a\n"
      "    ret   nz\n"
      "in_read:\n"
      "    ld    hl,in_buf\n"
      "    ld    b,78\n"
      "in_rc:\n"
      "    push  bc\n"
      "    push  hl\n"
      "    call  in_get\n"
      "    pop   hl\n"
      "    pop   bc\n"
      "    cp    13\n"
      "    jr    z,in_cr\n"
      "    cp    10\n"
      "    jr    z,in_rend\n"
      "    cp    26\n"
      "    jr    z,in_rend\n"
      "    ld    (hl),a\n"
      "    inc   hl\n"
      "    djnz  in_rc\n"
      "    jr    in_rend\n"
      "in_cr:\n"
      "    push  hl\n"
      "    call  in_get\n"
      "    pop   hl\n"
      "    cp    10\n"
      "    jr    z,in_rend\n"
      "    ld    (in_unget),a\n"
      "in_rend:\n"
      "    ld    (hl),0\n"
      "    ld    hl,in_buf\n"
      "    ld    (in_ptr),hl\n"
      "    ret\n"
      "in_sp:\n"
      "    ld    hl,(in_ptr)\n"
      "in_sp2:\n"
      "    ld    a,(hl)\n"
      "    cp    32\n"
      "    jr    nz,in_sp3\n"
      "    inc   hl\n"
      "    jr    in_sp2\n"
      "in_sp3:\n"
      "    ld    (in_ptr),hl\n"
      "    ret\n"
      "in_parse:\n"
      "    ld    hl,0\n"
      "in_pl:\n"
      "    push  hl\n"
      "    ld    hl,(in_ptr)\n"
      "    ld    a,(hl)\n"
      "    pop   hl\n"
      "    cp    '0'\n"
      "    ret   c\n"
      "    cp    '9'+1\n"
      "    ret   nc\n"
      "    sub   '0'\n"
      "    push  af\n"
      "    add   hl,hl\n"
      "    ld    d,h\n"
      "    ld    e,l\n"
      "    add   hl,hl\n"
      "    add   hl,hl\n"
      "    add   hl,de\n"
      "    pop   af\n"
      "    ld    e,a\n"
      "    ld    d,0\n"
      "    add   hl,de\n"
      "    push  hl\n"
      "    ld    hl,(in_ptr)\n"
      "    inc   hl\n"
      "    ld    (in_ptr),hl\n"
      "    pop   hl\n"
      "    jr    in_pl\n"
      "in_skip:\n"
      "    ld    hl,(in_ptr)\n"
      "in_sk2:\n"
      "    ld    a,(hl)\n"
      "    or    a\n"
      "    jr    z,in_sk3\n"
      "    cp    ','\n"
      "    jr    z,in_skc\n"
      "    inc   hl\n"
      "    jr    in_sk2\n"
      "in_skc:\n"
      "    inc   hl\n"
      "in_sk3:\n"
      "    ld    (in_ptr),hl\n"
      "    ret\n"
      "input_num:\n"
      "    call  in_pull\n"
      "    call  in_sp\n"
      "    ld    hl,(in_ptr)\n"
      "    ld    a,(hl)\n"
      "    cp    '-'\n"
      "    jr    nz,in_npos\n"
      "    inc   hl\n"
      "    ld    (in_ptr),hl\n"
      "    call  in_parse\n"
      "    xor   a\n"
      "    sub   l\n"
      "    ld    l,a\n"
      "    ld    a,0\n"
      "    sbc   a,h\n"
      "    ld    h,a\n"
      "    jr    in_nd\n"
      "in_npos:\n"
      "    call  in_parse\n"
      "in_nd:\n"
      "    push  hl\n"
      "    call  in_skip\n"
      "    pop   hl\n"
      "    xor   a\n"
      "    ld    (sf_i2f_mode),a\n"
      "    call  i2f\n"
      "    ret\n"
      "input_str:\n"
      "    call  in_pull\n"
      "    ld    hl,(in_ptr)\n"
      "in_sc:\n"
      "    ld    a,(hl)\n"
      "    or    a\n"
      "    jr    z,in_se\n"
      "    cp    ','\n"
      "    jr    z,in_se\n"
      "    ld    (de),a\n"
      "    inc   hl\n"
      "    inc   de\n"
      "    jr    in_sc\n"
      "in_se:\n"
      "    xor   a\n"
      "    ld    (de),a\n"
      "    ld    (in_ptr),hl\n"
      "    ld    a,(hl)\n"
      "    cp    ','\n"
      "    ret   nz\n"
      "    inc   hl\n"
      "    ld    (in_ptr),hl\n"
      "    ret\n"
      "line_in:\n"
      "    call  in_pull\n"
      "    ld    hl,(in_ptr)\n"
      "lin_c:\n"
      "    ld    a,(hl)\n"
      "    ld    (de),a\n"
      "    or    a\n"
      "    jr    z,lin_e\n"
      "    inc   hl\n"
      "    inc   de\n"
      "    jr    lin_c\n"
      "lin_e:\n"
      "    ld    (in_ptr),hl\n"
      "    ret\n"
      "in_get:\n"
      "    ld    a,(in_unget)\n"
      "    or    a\n"
      "    jr    z,in_getb\n"
      "    push  af\n"
      "    xor   a\n"
      "    ld    (in_unget),a\n"
      "    pop   af\n"
      "    ret\n"
      "in_getb:\n"
      "    ld    c,1\n"
      "    call  5\n"
      "    ret\n"
      "in_ptr: dw 0\n"
      "in_unget: db 0\n"
      "in_buf: ds 80\n"
      "\n";
    m_funcsUsed.insert(PrintCh_FUNC);
    m_useFloat = true;
  }

  if (m_useFloat) {
    std::string path = __FILE__;
    auto slash = path.find_last_of('/');
    path = path.substr(0, slash + 1) + "softf.asm";
    std::ifstream softf(path);
    if (!softf) {
      cerr << "error: cannot read " << path << "\n";
      return false;
    }
    *m_outputStream << "\n";
    *m_outputStream << softf.rdbuf();
    *m_outputStream << "\n";
  }

  if (AST::g_stringConstants.size() > 0) {
    *m_outputStream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                    << "; String constants\n"
                    << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                    << "\n"
                    ;
    for (auto & r : AST::g_stringConstants) {
      *m_outputStream << "str_" << r.second << ":";
      const std::string & str = r.first;
      if (str.empty()) {
        *m_outputStream << "\tdb\t0\n";
        continue;
      }
      int i = 0;
      std::string prefix = "\t";
      if (!isprint(static_cast<unsigned char>(str[i]))) {
        prefix += "db\t";
        while (i < str.length()) {
          *m_outputStream << prefix << "0x" << setw(2) << setfill('0') << hex << (int)(unsigned char)str[i] << dec;
          prefix = ", ";
          ++i;
        }
        *m_outputStream << ", 0\n";
      }
      else {
        int start = i;
        while ((i < str.length()) && (isprint(static_cast<unsigned char>(str[i]))))
          ++i;
        *m_outputStream << prefix << "dm\t\"" << str.substr(start, i-start) << "\"\n";
        *m_outputStream << "\tdb\t0\n";
      }
    }
    *m_outputStream << "\n";
  }

  *m_outputStream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "; Temp storage\n"
                  << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "\n"
                  << "temp:\n"
                  ;
  if (USED(PrintCh)) {
    *m_outputStream << "conout: jp    0  ; replaced with address of BIOS conout\n"
                    << "tabwid: db    " << g_languageProfile->GetTabWidth() << " ; tab width\n"
                    << "column: db    0  ; current tab column\n"
                    << "\n"
                    ;    
  }

  *m_outputStream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "; Variables\n"
                  << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
                  << "\n"
                  << "vars:\n";
  for (auto & r : m_globalVars) {
    AsmTypeInfoRec & info = g_varTypeInfo[(int)r.second.m_type];
    *m_outputStream << r.second.m_aname << ":\t";
    auto bounds = AST::g_arrayBounds.find(r.first);
    bool isArray = bounds != AST::g_arrayBounds.end() && AST::g_userFunctions.count(r.first) == 0;
    if (isArray) {
      int count = 1;
      for (int upper : bounds->second)
        count *= (upper + 1);
      int width = 2;
      if (r.second.m_type == VarType::eSingle)
        width = 4;
      else if (r.second.m_type == VarType::eDouble)
        width = 8;
      *m_outputStream << "ds\t" << (count * width);
    }
    else if (r.second.m_type == VarType::eSingle)
      *m_outputStream << "db\t0,0,0,0";
    else if (r.second.m_type == VarType::eDouble)
      *m_outputStream << "db\t0,0,0,0,0,0,0,0";
    else if (r.second.m_type == VarType::eString)
      *m_outputStream << "dw\t0";
    else
      *m_outputStream << info.m_asmtype << "\t" << info.m_initializer;
    *m_outputStream << "\t; " << r.first << "\n";
    if (!isArray && r.second.m_type == VarType::eString)
      *m_outputStream << r.second.m_aname << "_b:\tds\t64\n";
  }
  for (auto & slot : m_floatSlots)
    *m_outputStream << slot << ":\tdb\t0,0,0,0\n";
  for (auto & fc : m_floatConsts) {
    uint32_t bits = fc.first;
    *m_outputStream << fc.second << ":\tdb\t"
                    << (bits & 0xff) << ","
                    << ((bits >> 8) & 0xff) << ","
                    << ((bits >> 16) & 0xff) << ","
                    << ((bits >> 24) & 0xff) << "\n";
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

int Z80_CodeGenerator::Generate(const AST::Rem & expr)
{
  return 0;
}

///////////////////////////////////////////////////////////////////

void Z80_CodeGenerator::LoadRegPair(const std::string & regPair, const std::string & val)
{
  if (val.empty()) {
    m_regs.erase(regPair);
    m_regs.erase(std::string(regPair[0], 1));
    m_regs.erase(std::string(regPair[1], 1));
  }
  //if (m_regs[regPair] != val) {
    m_regs[regPair] = val;
    m_regs.erase(std::string(regPair[0], 1));
    m_regs.erase(std::string(regPair[1], 1));
    TopOutput() << "ld    " << regPair << "," << val << "\n";
  //}
}

void Z80_CodeGenerator::LoadReg(char reg, const std::string & val)
{
  std::string regPair;
  switch (reg) {
    case 'b':
    case 'c':
      regPair = "bc";
      break;
    case 'd':
    case 'e':
      regPair = "de";
      break;
    case 'h':
    case 'l':
      regPair = "hl";
      break;
  }
  std::string regStr(reg, 1);
  if (val.empty()) {
    m_regs.erase(regPair);
    m_regs.erase(regStr);
  }
  //if (m_regs[regStr] != val) {
    m_regs.erase(regPair);
    m_regs[regStr] = val;
    TopOutput() << "ld    " << reg << "," << val << "\n";
  //}
}

int Z80_CodeGenerator::Print(const AST::StringConstant & expr)
{
  int index = AST::g_stringConstants[expr.GetValue()];

  STRM_STR_DECL(hl, "str_" << index);
  LoadHL(hl);

  int len = expr.GetValue().length();
  STRM_STR_DECL(lenStr, len);

  if (len < 0x100) {
    LoadC(lenStr);
    TopOutput() << "call  " PrintStrc_FUNC "\n"
                     ;
  }
  else {
    LoadBC(lenStr);
    TopOutput() << "call  " PrintStr_FUNC "\n"
                     ;
  }

  m_funcsUsed.insert(PrintStr_FUNC);                

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  STRM_STR_DECL(val, expr.GetValue());
  LoadHL(val);
  TopOutput() << "call  " PrintI16s_FUNC "\n"
                   ;

  m_funcsUsed.insert(PrintI16s_FUNC);

  return 0;
}

int Z80_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  LoadFacc(expr);
  TopOutput() << "call  print_f\n";
  m_useFloat = true;
  m_funcsUsed.insert(PrintCh_FUNC);
  return 0;
}

int Z80_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  char buf[80];
  snprintf(buf, sizeof(buf), "% .13lg ", expr.AsDouble());
  std::string text(buf);
  if (AST::g_stringConstants.count(text) == 0)
    AST::g_stringConstants[text] = AST::g_stringConstantIndex++;
  int index = AST::g_stringConstants[text];
  TopOutput() << "ld    hl,str_" << index << "\n";
  TopOutput() << "ld    c," << text.size() << "\n";
  TopOutput() << "call  " PrintStrc_FUNC "\n";
  m_funcsUsed.insert(PrintStr_FUNC);
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::Print & printExpr)
{
  AST::ExprList & expr = *printExpr.m_list;
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }

  if ((expr.m_list.size() > 0) && !expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon()) {
    TopOutput() << "call  " << PrintNewline_FUNC << "\n";
    m_funcsUsed.insert(PrintNewline_FUNC);
  }
  
  return 0;
}

int Z80_CodeGenerator::Print(const AST::NumericExpr & expr) 
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(expr);
    TopOutput() << "call  print_f\n";
    m_useFloat = true;
    m_funcsUsed.insert(PrintCh_FUNC);
    return 0;
  }

  std::string str;    
  expr.Evaluate(*this, str);

  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  TopOutput() << "call  " << funcName << "\n"
                   ;  
  return 0;  
}

int Z80_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];

  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    TopOutput() << "ld    hl," << avar.m_aname << "\n";
    TopOutput() << "call  fload\n";
    TopOutput() << "call  print_f\n";
    m_useFloat = true;
    m_funcsUsed.insert(PrintCh_FUNC);
    return 0;
  }

  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  STRM_STR_DECL(val, "(" << avar.m_aname << ")");
  LoadHL(val);
  TopOutput() << "call  " << funcName << "\n"
                   ;  
  return 0;  
}

int Z80_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput() << "call  print_tab\n";
  m_funcsUsed.insert(PrintTab_FUNC);
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::GotoStatement & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0) {
    cerr << "internal error: cannot resolve goto destination '" << expr.GetRef() << "'" << endl;
    return 1;
  }

  TopOutput() << "jp    line_" << line << "\n"; 
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::GosubStatement & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0) {
    cerr << "internal error: cannot resolve gosub destination '" << expr.GetRef() << "'" << endl;
    return 1;
  }
  std::string cont = GetGlobalTempName();
  TopOutput() << "ld    hl," << cont << "\n";
  TopOutput() << "push  hl\n";
  TopOutput() << "jp    line_" << line << "\n";
  TopOutput(false) << cont << ":\n";
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::ReturnStatement & expr)
{
  TopOutput() << "ret\n";
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::System & expr)
{
  TopOutput() << "jp    end\n";
  return 0;
}

///////////////////////////////////////////////////////////////////

void Z80_CodeGenerator::AssignExprToRegPair(const std::string & regPair, const AST::Expr & expr)
{  
  std::string exprVal;
  expr.Evaluate(*this, exprVal);
  switch (expr.GetType()) {
    case VarType::eInt16:
      if (expr.IsConstant()) {
        LoadRegPair(regPair, exprVal);
      }
      else if (expr.IsVarRef()) {
        STRM_STR_DECL(val, "(" << exprVal << ")");
        LoadHL(val);
        if (regPair != "hl") {
          TopOutput() << "ex    de,hl\n";
        }
      }
      break;
    default:
      cerr << "error: numeric type not supported for assign" << endl;
      exit(-1);  
  }
}

int Z80_CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_currentStatementLine = expr.m_lineNumber;
  return expr.m_expr->Generate(*this);
}

int Z80_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), avar))
    return -1;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
    return -1;
  }

  if (expr.m_lhs->GetType() == VarType::eSingle || expr.m_lhs->GetType() == VarType::eDouble) {
    LoadFacc(*expr.m_rhs);
    if (auto * subscript = dynamic_cast<const AST::NumericSubscript *>(expr.m_lhs)) {
      bool allConst = true;
      for (auto * index : subscript->m_indexes) {
        if (dynamic_cast<const AST::NumericConstant *>(index) == nullptr)
          allConst = false;
      }
      if (!allConst) {
        us.Output() << "    call  fpush\n";
        EmitElementAddress(*subscript);
        us.Output() << "    push  hl\ncall  fpop\nld    hl,farg\ncall  fload\npop   hl\n";
      }
      else
        EmitElementAddress(*subscript);
    }
    else
      us.Output() << "    ld    hl," << avar.m_aname << "\n";
    us.Output() << "    call  fstore\n";
    m_useFloat = true;
    return 0;
  }

  AssignExprToHL(*expr.m_rhs);
  us.Output() << "    ld    (" << avar.m_aname << "),hl\n";

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
    CompilerError(Error_UndeclaredVariable, 0, "unknown variable \"" << expr.GetName() << "\"");
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];
  result = avar.m_aname;  
  return 0;
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::EvaluateBinaryOperands(const AST::NumericBinaryOperation & expr, bool commutative)
{
  if ((expr.m_lhs == nullptr) || (expr.m_rhs == nullptr))
    return -1;

  if (expr.m_rhs->IsConstant()) {
    AssignExprToHL(*expr.m_lhs);
    AssignExprToRegPair("de", *expr.m_rhs);
  }
  else if (expr.m_lhs->IsConstant()) {
    AssignExprToHL(*expr.m_rhs);
    AssignExprToRegPair("de", *expr.m_lhs);
    TopOutput() << "ex    de,hl\n";
  }
  else {
    AssignExprToHL(*expr.m_rhs);
    TopOutput() << "push  hl\n";
    AssignExprToHL(*expr.m_lhs);
    TopOutput() << "pop   de\n";
  }

  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result)
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(*expr.m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*expr.m_rhs);
    TopOutput() << "call  fpop\n";
    TopOutput() << "call  fadd\n";
    m_useFloat = true;
    return 0;
  }

  EvaluateBinaryOperands(expr, true);
  if (expr.GetType() == VarType::eInt16)
    TopOutput() << "add   hl,de\n";
  else {
    cerr << "error: numeric type not supported for binary op +" << endl;
    exit(-1);
  }
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result)
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(*expr.m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*expr.m_rhs);
    TopOutput() << "call  fpop\n";
    TopOutput() << "call  fsub\n";
    m_useFloat = true;
    return 0;
  }

  EvaluateBinaryOperands(expr, false);
  if (expr.GetType() == VarType::eInt16) {
    TopOutput() << "or    a\n";
    TopOutput() << "sbc   hl,de\n";
  }
  else {
    cerr << "error: numeric type not supported for binary op -" << endl;
    exit(-1);
  }
  return 0;
}


int Z80_CodeGenerator::NumericComparisonOperator(const AST::NumericBinaryOperation & expr, const std::string & funcBase)
{
  VarType operand = expr.m_lhs ? expr.m_lhs->GetType() : VarType::eInt16;
  if (operand == VarType::eSingle || operand == VarType::eDouble) {
    EmitFloatRelation(expr, funcBase);
    return 0;
  }

  EvaluateBinaryOperands(expr, false);

  switch (expr.GetType()) {
    case VarType::eInt16:
      {
        std::string typeFuncName = funcBase + "_i16";
        m_funcsUsed.insert(typeFuncName);
        TopOutput() << "call  " << typeFuncName << "\n";
      }
      break;
    default:
      cerr << "error: numeric type not supported for binary op " << funcBase << endl;
      exit(-1);  
  }

  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "equal");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "nequal");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "gt");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "gte");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "lt");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "lte");
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr)
    return -1;
  if (expr.m_trueStatements == nullptr)
    return -1;

  if (expr.m_cond->GetType() == VarType::eSingle || expr.m_cond->GetType() == VarType::eDouble) {
    LoadFacc(*expr.m_cond);
    std::string nz = GetGlobalTempName();
    std::string tf = GetGlobalTempName();
    TopOutput() << "ld    a,(facc+3)\nand   0x7f\njp    nz," << nz << "\n";
    TopOutput() << "ld    a,(facc+2)\nor    a\njp    nz," << nz << "\n";
    TopOutput() << "ld    a,(facc+1)\nor    a\njp    nz," << nz << "\n";
    TopOutput() << "ld    a,(facc+0)\nor    a\njp    nz," << nz << "\n";
    TopOutput() << "ld    hl,0\njp    " << tf << "\n";
    TopOutput(false) << nz << ":\nld    hl,-1\n";
    TopOutput(false) << tf << ":\n";
  }
  else
    AssignExprToHL(*expr.m_cond);
  TopOutput(false) << "    ld    a,h\n"
                   << "    or    l\n";

  std::string temp2;
  std::string gotoLine = expr.m_trueStatements->m_lineNumber;
  if (!gotoLine.empty()) {
    int line = ResolveGotoDestination(gotoLine);
    if (line < 0) {
      cerr << "internal error: cannot resolve goto destination '" << gotoLine << "'" << endl;
      return 1;
    }
    TopOutput() << "jp    nz,line_" << line << "\n";
  }
  else {
    std::string temp1 = GetGlobalTempName();  
    temp2 = GetGlobalTempName();  
    TopOutput() << "jp    z," << temp1 << "  ; branch if false\n";
    Push();
    expr.m_trueStatements->m_statements->Generate(*this);
    Pop();
    TopOutput() << "jp    " << temp2 << "\n";
    TopOutput(false) << temp1 << ":\n";
  }
  if (expr.m_falseStatements) {
    Push();
    expr.m_falseStatements->Generate(*this);
    Pop();
  }
  if (gotoLine.empty()) {
    TopOutput(false) << temp2 << ":\n";
  }
  return 0;
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::ForStatement & expr)
{
  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_var->GetName(), avar))
    return -1;

  std::string forRef = GetGlobalTempName();
  ForBlock forBlock;
  forBlock.m_ref   = forRef;
  forBlock.m_type  = expr.m_var->GetType();
  forBlock.m_index = avar.m_aname;
  forBlock.m_step = expr.m_stepVal;
  forBlock.m_to = expr.m_toVal;

  if (forBlock.m_type == VarType::eSingle || forBlock.m_type == VarType::eDouble) {
    forBlock.m_check = GetGlobalTempName();
    forBlock.m_toSlot = "sft_" + std::to_string(++m_floatSlotId);
    forBlock.m_stepSlot = "sfs_" + std::to_string(m_floatSlotId);
    m_floatSlots.push_back(forBlock.m_toSlot);
    m_floatSlots.push_back(forBlock.m_stepSlot);
    LoadFacc(*expr.m_fromVal);
    TopOutput() << "ld    hl," << avar.m_aname << "\ncall  fstore\n";
    LoadFacc(*expr.m_toVal);
    TopOutput() << "ld    hl," << forBlock.m_toSlot << "\ncall  fstore\n";
    if (expr.m_stepVal != nullptr)
      LoadFacc(*expr.m_stepVal);
    else {
      AST::SingleConstant one(1.0f);
      LoadFacc(one);
    }
    TopOutput() << "ld    hl," << forBlock.m_stepSlot << "\ncall  fstore\n";
    TopOutput() << "jp    " << forBlock.m_check << "\n";
    m_useFloat = true;
  }
  else {
    AssignExprToHL(*expr.m_fromVal);
    TopOutput(true) << "ld    (" << avar.m_aname << "),hl\n";
  }

  m_forQueue.push_back(forBlock);
  TopOutput(false) << forRef << ":\n";
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::NextStatement & expr)
{
  if (m_forQueue.size() == 0) {
    cerr << "Mismatched next\n";
    exit(-1);
  }

  ForBlock forBlock = m_forQueue.back();
  m_forQueue.pop_back();

  if (forBlock.m_type == VarType::eSingle || forBlock.m_type == VarType::eDouble) {
    std::string neg = GetGlobalTempName();
    std::string done = GetGlobalTempName();
    TopOutput() << "ld    hl," << forBlock.m_index << "\ncall  fload\ncall  fpush\n";
    TopOutput() << "ld    hl," << forBlock.m_stepSlot << "\ncall  fload\ncall  fpop\ncall  fadd\n";
    TopOutput() << "ld    hl," << forBlock.m_index << "\ncall  fstore\n";
    TopOutput(false) << forBlock.m_check << ":\n";
    TopOutput() << "ld    hl," << forBlock.m_stepSlot << "\ncall  fload\n";
    TopOutput() << "ld    a,(facc+3)\nrlca\njp    c," << neg << "\n";
    TopOutput() << "ld    hl," << forBlock.m_index << "\ncall  fload\ncall  fpush\n";
    TopOutput() << "ld    hl," << forBlock.m_toSlot << "\ncall  fload\ncall  fpop\ncall  fcmp\n";
    TopOutput() << "ld    a,h\nor    l\njp    z," << forBlock.m_ref << "\n";
    TopOutput() << "bit   7,h\njp    nz," << forBlock.m_ref << "\n";
    TopOutput() << "jp    " << done << "\n";
    TopOutput(false) << neg << ":\n";
    TopOutput() << "ld    hl," << forBlock.m_index << "\ncall  fload\ncall  fpush\n";
    TopOutput() << "ld    hl," << forBlock.m_toSlot << "\ncall  fload\ncall  fpop\ncall  fcmp\n";
    TopOutput() << "bit   7,h\njp    z," << forBlock.m_ref << "\n";
    TopOutput(false) << done << ":\n";
    m_useFloat = true;
    return 0;
  }

  LoadHL("(" + forBlock.m_index + ")");
  TopOutput() << "push  hl\n";
  if (forBlock.m_step == nullptr) {
    TopOutput() << "inc   hl\n";
  }
  else {
    AssignExprToRegPair("de", *forBlock.m_step);
    TopOutput() << "add   hl,de\n";
  }
  TopOutput() << "ld    (" + forBlock.m_index + "),hl\n";
  TopOutput() << "pop   hl\n";
  AssignExprToRegPair("de", *forBlock.m_to);
  TopOutput() << "or    a\n";
  TopOutput() << "sbc   hl,de\n";
  TopOutput() << "jp    c," << forBlock.m_ref << "\n";
  TopOutput(false) << "\n";
  return 0;
}

std::string Z80_CodeGenerator::FloatConst(float value)
{
  uint32_t bits = 0;
  static_assert(sizeof(float) == 4, "float must be 4 bytes");
  memcpy(&bits, &value, 4);
  auto found = m_floatConsts.find(bits);
  if (found != m_floatConsts.end())
    return found->second;
  std::string name = "fc_" + std::to_string(m_floatConsts.size());
  m_floatConsts[bits] = name;
  m_useFloat = true;
  return name;
}

void Z80_CodeGenerator::LoadFacc(const AST::Expr & expr)
{
  m_useFloat = true;
  if (auto * add = dynamic_cast<const AST::NumericAddition *>(&expr)) {
    LoadFacc(*add->m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*add->m_rhs);
    TopOutput() << "call  fpop\ncall  fadd\n";
    return;
  }
  if (auto * sub = dynamic_cast<const AST::Subtraction *>(&expr)) {
    LoadFacc(*sub->m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*sub->m_rhs);
    TopOutput() << "call  fpop\ncall  fsub\n";
    return;
  }
  if (auto * neg = dynamic_cast<const AST::Negation *>(&expr)) {
    LoadFacc(*neg->m_expr);
    TopOutput() << "call  fneg\n";
    return;
  }
  if (auto * mul = dynamic_cast<const AST::Multiplication *>(&expr)) {
    LoadFacc(*mul->m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*mul->m_rhs);
    TopOutput() << "call  fpop\ncall  fmul\n";
    return;
  }
  if (auto * div = dynamic_cast<const AST::Division *>(&expr)) {
    LoadFacc(*div->m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*div->m_rhs);
    TopOutput() << "call  fpop\ncall  fdiv\n";
    return;
  }
  if (auto * pow = dynamic_cast<const AST::Power *>(&expr)) {
    AST::SingleConstant one(1.0f);
    LoadFacc(one);
    TopOutput() << "ld    hl,sf_hold2\ncall  fstore\n";
    LoadFacc(*pow->m_rhs);
    TopOutput() << "call  f2i\n";
    std::string loop = GetGlobalTempName();
    std::string done = GetGlobalTempName();
    TopOutput(false) << loop << ":\n";
    TopOutput() << "ld    a,h\nor    l\njp    z," << done << "\n";
    TopOutput() << "dec   hl\npush  hl\n";
    TopOutput() << "ld    hl,sf_hold2\ncall  fload\ncall  fpush\n";
    LoadFacc(*pow->m_lhs);
    TopOutput() << "call  fpop\ncall  fmul\n";
    TopOutput() << "ld    hl,sf_hold2\ncall  fstore\n";
    TopOutput() << "pop   hl\njp    " << loop << "\n";
    TopOutput(false) << done << ":\n";
    TopOutput() << "ld    hl,sf_hold2\ncall  fload\n";
    return;
  }
  if (auto * rnd = dynamic_cast<const AST::RndFunction *>(&expr)) {
    if (rnd->m_arg != nullptr)
      LoadFacc(*rnd->m_arg);
    else {
      AST::SingleConstant one(1.0f);
      LoadFacc(one);
    }
    TopOutput() << "call  rnd\n";
    return;
  }
  if (auto * sub = dynamic_cast<const AST::NumericSubscript *>(&expr)) {
    if (AST::g_userFunctions.count(sub->GetName()) != 0) {
      EmitUserFunction(*sub, true);
      return;
    }
    EmitElementAddress(*sub);
    TopOutput() << "call  fload\n";
    return;
  }
  if (auto * var = dynamic_cast<const AST::NumericVarRef *>(&expr)) {
    AsmVarDef avar;
    if (!LookupGlobalVar(var->GetName(), avar))
      return;
    TopOutput() << "ld    hl," << avar.m_aname << "\ncall  fload\n";
    return;
  }
  if (auto * num = dynamic_cast<const AST::NumericConstant *>(&expr)) {
    std::string lab = FloatConst(num->AsSingle());
    TopOutput() << "ld    hl," << lab << "\ncall  fload\n";
    return;
  }
  cerr << "error: cannot load floating expression\n";
  exit(-1);
}

int Z80_CodeGenerator::Evaluate(const AST::Negation & expr, std::string & result)
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(expr);
    return 0;
  }
  std::string inner;
  if (expr.m_expr == nullptr || expr.m_expr->Evaluate(*this, inner) != 0)
    return -1;
  TopOutput() << "ex    de,hl\nld    hl,0\nor    a\nsbc   hl,de\n";
  return 0;
}

void Z80_CodeGenerator::EmitFloatRelation(const AST::NumericBinaryOperation & expr, const std::string & kind)
{
  LoadFacc(*expr.m_lhs);
  TopOutput() << "call  fpush\n";
  LoadFacc(*expr.m_rhs);
  TopOutput() << "call  fpop\ncall  fcmp\n";
  std::string yes = GetGlobalTempName();
  std::string done = GetGlobalTempName();
  if (kind == "equal") {
    TopOutput() << "ld    a,h\nor    l\njp    nz," << yes << "\n";
    TopOutput() << "ld    hl,-1\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,0\n";
  }
  else if (kind == "nequal") {
    TopOutput() << "ld    a,h\nor    l\njp    z," << yes << "\n";
    TopOutput() << "ld    hl,-1\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,0\n";
  }
  else if (kind == "gt") {
    TopOutput() << "bit   7,h\njp    nz," << yes << "\n";
    TopOutput() << "ld    a,h\nor    l\njp    z," << yes << "\n";
    TopOutput() << "ld    hl,-1\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,0\n";
  }
  else if (kind == "gte") {
    TopOutput() << "bit   7,h\njp    nz," << yes << "\n";
    TopOutput() << "ld    hl,-1\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,0\n";
  }
  else if (kind == "lt") {
    TopOutput() << "bit   7,h\njp    z," << yes << "\n";
    TopOutput() << "ld    hl,-1\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,0\n";
  }
  else {
    TopOutput() << "bit   7,h\njp    nz," << yes << "\n";
    TopOutput() << "ld    a,h\nor    l\njp    z," << yes << "\n";
    TopOutput() << "ld    hl,0\njp    " << done << "\n";
    TopOutput(false) << yes << ":\nld    hl,-1\n";
  }
  TopOutput(false) << done << ":\n";
  m_useFloat = true;
}

static void EmitBitwise(Z80_CodeGenerator & gen, const AST::NumericBinaryOperation & expr, const char * op)
{
  std::string ignored;
  expr.m_lhs->Evaluate(gen, ignored);
  gen.TopOutput() << "push  hl\n";
  expr.m_rhs->Evaluate(gen, ignored);
  gen.TopOutput() << "pop   de\n";
  gen.TopOutput() << "ld    a,h\n" << op << "   d\nld    h,a\n";
  gen.TopOutput() << "ld    a,l\n" << op << "   e\nld    l,a\n";
}

int Z80_CodeGenerator::Evaluate(const AST::LogicalAnd & expr, std::string & result)
{
  EmitBitwise(*this, expr, "and");
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::LogicalOr & expr, std::string & result)
{
  EmitBitwise(*this, expr, "or");
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::Multiplication & expr, std::string & result)
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(*expr.m_lhs);
    TopOutput() << "call  fpush\n";
    LoadFacc(*expr.m_rhs);
    TopOutput() << "call  fpop\n";
    TopOutput() << "call  fmul\n";
    m_useFloat = true;
    return 0;
  }
  cerr << "error: integer multiply is not implemented\n";
  exit(-1);
}

int Z80_CodeGenerator::Generate(const AST::WidthStatement & expr)
{
  m_funcsUsed.insert(PrintCh_FUNC);
  TopOutput() << "ld    a," << expr.m_width << "\n";
  TopOutput() << "ld    (ix+tabwid-temp),a\n";
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::ClearStatement & expr)
{
  for (auto & r : m_globalVars) {
    const std::string & name = r.second.m_aname;
    TopOutput() << "ld    hl,0\n";
    TopOutput() << "ld    (" << name << "),hl\n";
    if (r.second.m_type == VarType::eSingle || r.second.m_type == VarType::eDouble)
      TopOutput() << "ld    (" << name << "+2),hl\n";
    if (r.second.m_type == VarType::eDouble) {
      TopOutput() << "ld    (" << name << "+4),hl\n";
      TopOutput() << "ld    (" << name << "+6),hl\n";
    }
  }
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::OnGotoStatement & expr)
{
  if (expr.m_index == nullptr)
    return -1;
  for (size_t i = 0; i < expr.m_lines.size(); ++i) {
    int line = ResolveGotoDestination(expr.m_lines[i]);
    if (line < 0)
      return 1;
    LoadFacc(*expr.m_index);
    TopOutput() << "call  fpush\n";
    AST::SingleConstant which(static_cast<float>(i + 1));
    LoadFacc(which);
    TopOutput() << "call  fpop\ncall  fcmp\n";
    TopOutput() << "ld    a,h\nor    l\n";
    TopOutput() << "jp    z,line_" << line << "\n";
  }
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::StringAssign & expr)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), avar))
    return -1;
  auto * constant = dynamic_cast<const AST::StringConstant *>(expr.m_rhs);
  if (constant == nullptr) {
    cerr << "error: only string constant assignment is implemented\n";
    exit(-1);
  }
  TopOutput() << "ld    hl,str_" << AST::g_stringConstants[constant->GetValue()] << "\n";
  TopOutput() << "ld    (" << avar.m_aname << "),hl\n";
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::StringCompare & expr, std::string & result)
{
  auto loadPtr = [this](const AST::StringExpr * side) {
    if (auto * constant = dynamic_cast<const AST::StringConstant *>(side)) {
      TopOutput() << "ld    hl,str_" << AST::g_stringConstants[constant->GetValue()] << "\n";
      return;
    }
    if (auto * var = dynamic_cast<const AST::StringVarRef *>(side)) {
      AsmVarDef avar;
      if (!LookupGlobalVar(var->GetName(), avar))
        return;
      TopOutput() << "ld    hl,(" << avar.m_aname << ")\n";
      return;
    }
    cerr << "error: unsupported string compare operand\n";
    exit(-1);
  };
  loadPtr(expr.m_rhs);
  TopOutput() << "push  hl\n";
  loadPtr(expr.m_lhs);
  TopOutput() << "pop   de\ncall  streq\n";
  if (!expr.m_equal) {
    std::string flip = GetGlobalTempName();
    TopOutput() << "ld    a,h\nor    l\njp    z," << flip << "\n";
    TopOutput() << "ld    hl,0\njp    " << flip << "z\n";
    TopOutput(false) << flip << ":\nld    hl,-1\n";
    TopOutput(false) << flip << "z:\n";
  }
  m_funcsUsed.insert("streq");
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::Division & expr, std::string & result)
{
  if (expr.GetType() == VarType::eSingle || expr.GetType() == VarType::eDouble) {
    LoadFacc(expr);
    return 0;
  }
  cerr << "error: integer divide is not implemented\n";
  exit(-1);
}

int Z80_CodeGenerator::Evaluate(const AST::Power & expr, std::string & result)
{
  LoadFacc(expr);
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr)
    return -1;
  LoadFacc(*expr.GetArg1());
  TopOutput() << "call  f2i\n";
  m_useFloat = true;
  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::RndFunction & expr, std::string & result)
{
  LoadFacc(expr);
  return 0;
}

void Z80_CodeGenerator::EmitElementAddress(const AST::NumericSubscript & expr)
{
  AsmVarDef avar;
  if (!LookupGlobalVar(expr.GetName(), avar))
    return;
  int width = 2;
  if (avar.m_type == VarType::eSingle)
    width = 4;
  else if (avar.m_type == VarType::eDouble)
    width = 8;
  bool allConst = true;
  int offset = 0;
  int stride = 1;
  const auto & bounds = AST::g_arrayBounds[expr.GetName()];
  for (size_t d = 0; d < expr.m_indexes.size(); ++d) {
    auto * constant = dynamic_cast<const AST::NumericConstant *>(expr.m_indexes[d]);
    if (constant == nullptr) {
      allConst = false;
      break;
    }
    offset = offset * (d == 0 ? 1 : (bounds[d - 1] + 1)) + constant->AsInt32();
    (void)stride;
  }
  if (allConst && expr.m_indexes.size() == 1) {
    TopOutput() << "ld    hl," << avar.m_aname << "+" << (expr.m_indexes[0]
      ? dynamic_cast<const AST::NumericConstant *>(expr.m_indexes[0])->AsInt32() * width : 0) << "\n";
    return;
  }
  if (allConst) {
    TopOutput() << "ld    hl," << avar.m_aname << "+" << (offset * width) << "\n";
    return;
  }
  TopOutput() << "ld    hl,0\n";
  for (size_t d = 0; d < expr.m_indexes.size(); ++d) {
    if (d > 0) {
      int dim = bounds[d - 1] + 1;
      TopOutput() << "ld    de," << dim << "\n";
      TopOutput() << "call  mul_hlde\n";
    }
    TopOutput() << "push  hl\n";
    LoadFacc(*expr.m_indexes[d]);
    TopOutput() << "call  f2i\n";
    TopOutput() << "pop   de\nadd   hl,de\n";
  }
  for (int i = 1; i < width; i *= 2)
    TopOutput() << "add   hl,hl\n";
  TopOutput() << "ld    de," << avar.m_aname << "\nadd   hl,de\n";
}

void Z80_CodeGenerator::EmitUserFunction(const AST::NumericSubscript & expr, bool wantFloat)
{
  auto found = AST::g_userFunctions.find(expr.GetName());
  if (found == AST::g_userFunctions.end() || found->second.m_body == nullptr)
    return;
  const AST::UserFunction & fn = found->second;
  std::vector<std::string> slots;
  for (auto * index : expr.m_indexes) {
    std::string slot = "sarg_" + std::to_string(m_floatSlotId++);
    m_floatSlots.push_back(slot);
    LoadFacc(*index);
    TopOutput() << "ld    hl," << slot << "\ncall  fstore\n";
    slots.push_back(slot);
  }
  std::vector<std::string> params;
  for (size_t i = 0; i < fn.m_params.size() && i < slots.size(); ++i) {
    AsmVarDef avar;
    if (!LookupGlobalVar(fn.m_params[i], avar))
      return;
    params.push_back(avar.m_aname);
    TopOutput() << "ld    hl," << avar.m_aname << "\ncall  fload\ncall  fpush\n";
    TopOutput() << "ld    hl," << slots[i] << "\ncall  fload\n";
    TopOutput() << "ld    hl," << avar.m_aname << "\ncall  fstore\n";
  }
  bool bodyInt = fn.m_body->GetType() == VarType::eInt16 || fn.m_body->GetType() == VarType::eInt32;
  if (bodyInt && !wantFloat) {
    std::string ignored;
    fn.m_body->Evaluate(*this, ignored);
    TopOutput() << "push  hl\n";
  }
  else {
    LoadFacc(*fn.m_body);
    TopOutput() << "ld    hl,sf_hold\ncall  fstore\n";
  }
  for (auto it = params.rbegin(); it != params.rend(); ++it) {
    TopOutput() << "call  fpop\nld    hl,farg\ncall  fload\n";
    TopOutput() << "ld    hl," << *it << "\ncall  fstore\n";
  }
  if (bodyInt && !wantFloat)
    TopOutput() << "pop   hl\n";
  else
    TopOutput() << "ld    hl,sf_hold\ncall  fload\n";
  m_useFloat = true;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericSubscript & expr, std::string & result)
{
  if (AST::g_userFunctions.count(expr.GetName()) != 0) {
    bool bodyInt = false;
    auto & fn = AST::g_userFunctions[expr.GetName()];
    if (fn.m_body != nullptr)
      bodyInt = fn.m_body->GetType() == VarType::eInt16 || fn.m_body->GetType() == VarType::eInt32;
    EmitUserFunction(expr, !bodyInt);
    return 0;
  }
  EmitElementAddress(expr);
  TopOutput() << "call  fload\n";
  m_useFloat = true;
  return 0;
}

int Z80_CodeGenerator::Print(const AST::NumericSubscript & expr)
{
  if (AST::g_userFunctions.count(expr.GetName()) != 0) {
    auto & fn = AST::g_userFunctions[expr.GetName()];
    bool bodyInt = fn.m_body != nullptr &&
      (fn.m_body->GetType() == VarType::eInt16 || fn.m_body->GetType() == VarType::eInt32);
    std::string ignored;
    Evaluate(expr, ignored);
    if (bodyInt) {
      m_funcsUsed.insert(PrintI16s_FUNC);
      TopOutput() << "call  " << PrintI16s_FUNC << "\n";
    }
    else {
      m_funcsUsed.insert(PrintCh_FUNC);
      TopOutput() << "call  print_f\n";
    }
    return 0;
  }
  EmitElementAddress(expr);
  TopOutput() << "call  fload\ncall  print_f\n";
  m_useFloat = true;
  m_funcsUsed.insert(PrintCh_FUNC);
  return 0;
}

int Z80_CodeGenerator::Print(const AST::StringVarRef & expr)
{
  AsmVarDef avar;
  if (!LookupGlobalVar(expr.GetName(), avar))
    return -1;
  TopOutput() << "ld    hl,(" << avar.m_aname << ")\n";
  TopOutput() << "ld    a,h\nor    l\n";
  std::string skip = GetGlobalTempName();
  TopOutput() << "jp    z," << skip << "\n";
  TopOutput() << "call  print_zstr\n";
  TopOutput(false) << skip << ":\n";
  m_funcsUsed.insert("print_zstr");
  m_funcsUsed.insert(PrintCh_FUNC);
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::DimStatement & expr)
{
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::DefStatement & expr)
{
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::InputStatement & expr)
{
  m_funcsUsed.insert("input");
  m_funcsUsed.insert(PrintCh_FUNC);
  m_useFloat = true;
  if (!expr.m_prompt.empty()) {
    if (AST::g_stringConstants.count(expr.m_prompt) == 0)
      AST::g_stringConstants[expr.m_prompt] = AST::g_stringConstantIndex++;
    TopOutput() << "ld    hl,str_" << AST::g_stringConstants[expr.m_prompt] << "\n";
    TopOutput() << "call  print_zstr\n";
    m_funcsUsed.insert("print_zstr");
  }
  if (expr.m_vars == nullptr)
    return 0;
  for (auto & item : expr.m_vars->m_list) {
    auto * str = dynamic_cast<AST::StringVarRef *>(item.get());
    auto * numeric = dynamic_cast<AST::NumericVarRef *>(item.get());
    if (expr.m_lineInput && str != nullptr) {
      AsmVarDef avar;
      if (!LookupGlobalVar(str->GetName(), avar))
        return -1;
      TopOutput() << "ld    de," << avar.m_aname << "_b\ncall  line_in\n";
      TopOutput() << "ld    hl," << avar.m_aname << "_b\nld    (" << avar.m_aname << "),hl\n";
      continue;
    }
    if (str != nullptr) {
      AsmVarDef avar;
      if (!LookupGlobalVar(str->GetName(), avar))
        return -1;
      TopOutput() << "ld    de," << avar.m_aname << "_b\ncall  input_str\n";
      TopOutput() << "ld    hl," << avar.m_aname << "_b\nld    (" << avar.m_aname << "),hl\n";
      continue;
    }
    if (numeric != nullptr) {
      AsmVarDef avar;
      if (!LookupGlobalVar(numeric->GetName(), avar))
        return -1;
      TopOutput() << "call  input_num\n";
      TopOutput() << "ld    hl," << avar.m_aname << "\ncall  fstore\n";
    }
  }
  return 0;
}
