#include <typeinfo>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

AST::Program AST::g_program;
AST::VarList AST::g_globalVars;
AST::LineNumberInfo AST::g_lineNumberInfo;
AST::GotoList AST::g_gotoInfo;

/////////////////////////////////////////

int AST::Node::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int AST::Node::Print(CodeGenerator & gen) const
{ 
  return gen.Print(*this);
}

/////////////////////////////////////////

SourceLine::SourceLine(
                      unsigned lineNumber,
      const std::string & basicLineNumber,
      const std::string & line)
  : m_sourceLineNumber(lineNumber)
  , m_basicLineNumber(basicLineNumber)
  , m_line(line)
{
  size_t len = m_line.length();
  while ((len > 0) && (isspace(m_line[len-1]))) {
    --len;
  }
  m_line = m_line.substr(0, len);
}

int SourceLine::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

////////////////////////////////////////////////////////////////////////////

Expr::Expr(VarType type)
  : m_type(type)
{
}

VarType AST::Expr::GetType() const
{
  return m_type;
}

/////////////////////////////////////////

Print::Print(ExprList * list)
{
  Append(list);
}

int Print::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int PrintComma::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

int PrintSemiColon::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

StringAssign::StringAssign(const AST::StringVarRef * lhs, const StringExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

int StringAssign::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

bool StringAssign::Validate()
{
  return false;
}

/////////////////////////////////////////

StringVarRef::StringVarRef(const std::string & varName)
{
  m_originalName = varName;

  // extract suffix, if any
  int len = varName.length();
  std::string name = varName;

  if (len < 2) {
    cerr << "error: bad string name " << endl;
  }
  else {
    char last = varName[len-1];
    if (last != BASIC_STRING_SUFFIX[0]) {
      cerr << "error: string has bad suffix" << endl;
    }
    name = name.substr(0, len-1);
  }

  int varNameLen = g_languageProfile->GetVarNameLen();
  m_name = name.substr(0, varNameLen) + BASIC_STRING_SUFFIX;
}

int StringVarRef::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int StringVarRef::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

int StringConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int StringConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

NumericAssign::NumericAssign(const AST::NumericVarRef * lhs, const NumericExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

int NumericAssign::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

bool NumericAssign::Validate()
{
  if ((m_lhs == nullptr) || (m_rhs == nullptr))
    return false;

  VarType ltype = m_lhs->GetType();
  VarType rtype = m_rhs->GetType();

  std::string msg;

  if (ltype == rtype) {
    m_type = ltype;
    return true;
  }

  if (ltype == VarType::eString) {
    msg = "error: rhs must be string type";
    return false;
  }

  if (!m_rhs->IsConstant()) {
    m_type = ltype;
  }
  else {
    auto const rconst = dynamic_cast<const NumericConstant *>(m_rhs);
    if (ltype == VarType::eDouble) {
      double v = rconst->AsDouble();
      delete m_rhs;
      m_rhs = new DoubleConstant(v);
    }
    else if ((ltype == VarType::eSingle) && (rtype != VarType::eDouble)) {
      float v = rconst->AsSingle();
      delete m_rhs;
      m_rhs = new SingleConstant(v);
    }
    else if ((ltype == VarType::eInt32) && (rtype == VarType::eInt16)) {
      int32_t v = rconst->AsInt32();
      delete m_rhs;
      m_rhs = new Int16Constant(v);
    }
    if (ltype == m_rhs->GetType()) {
      m_type = ltype;
      return true;
    }
  }

  m_type = ltype;
  m_rhs = new AST::NumericCast(ltype, m_rhs);

  return true;
}

/////////////////////////////////////////

bool NumericBinaryOperation::Validate()
{
  if ((m_lhs == nullptr) || (m_rhs == nullptr))
    return false;

  VarType ltype = m_lhs->GetType();
  VarType rtype = m_rhs->GetType();

  std::string msg;

  if (ltype == rtype) {
    m_type = ltype;
    return true;
  }

  if (ltype == VarType::eString) {
    msg = "error: rhs must be string type";
    return false;
  }
  if (rtype == VarType::eString) {
    msg = "error: lhs must be string type";
    return false;
  }

  VarType etype = g_languageProfile->GetIntegerType();

  if ((ltype == VarType::eDouble) || (rtype == VarType::eDouble)) {
    etype = VarType::eDouble;
  }
  else if ((ltype == VarType::eSingle) || (rtype == VarType::eSingle)) {
    etype = VarType::eSingle;
  }

  if (ltype != etype) {
    m_lhs = new AST::NumericCast(etype, m_lhs);
  }
  else if (rtype != etype) {
    m_rhs = new AST::NumericCast(etype, m_rhs);
  }

  m_type = etype;

  return true;
}

int NumericAddition::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int Subtraction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int Multiplication::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int Division::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int Negation::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int Power::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int NumericCast::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int IntFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int SqrFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int LenFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int TabFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int LeftFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int MidFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int RightFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int ChrFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int StrFunction::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

/////////////////////////////////////////

NumericVarRef::NumericVarRef(VarType type, const std::string & varName)
  : NumericExpr(type)
{
  // extract suffix, if any
  int len = varName.length();
  std::string name = varName;
  std::string suffix;
  if (len > 1) {
    char last = varName[len-1];
    char * expectedSuffix = nullptr;
    VarType strType = VarType::eNone;
    if (last == BASIC_STRING_SUFFIX[0]) {
      strType = VarType::eString;
    }
    else if (last == BASIC_INT_SUFFIX[0]) {
      strType = g_languageProfile->GetIntegerType();
    }   
    else if (last == BASIC_SINGLE_SUFFIX[0]) {
      strType = VarType::eSingle;
    }   
    else if (last == BASIC_DOUBLE_SUFFIX[0]) {
      strType = VarType::eDouble;
    }
    else
      strType = VarType::eNone;

    if (strType != VarType::eNone) {
      name = varName.substr(0, len-1);
    }   

    if (type == VarType::eNone) {
      if (strType == VarType::eNone) {
        cerr << "internal error: cannot identify type of '" << varName << "'" << endl;
        exit(-1);
      }
      type = strType;
    }
    else if (strType != VarType::eNone) {
      if (strType != type) {
        cerr << "internal error: parser mismatch for type of '" << varName << "'" << endl;
        exit(-1);
      }
      type = strType;
    }

    suffix = g_basicVarSuffixes[(int)type];
  }

  m_originalName = name + suffix;

  int varNameLen = g_languageProfile->GetVarNameLen();
  m_name = name.substr(0, varNameLen) + suffix;
  m_type = type;

  if ((int)m_type == 0) {
    cerr << varName << " has type 0" << endl; 
  }
}

int NumericVarRef::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int NumericVarRef::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

template<>
AST::NumericExpr * AST::Int16Constant::Create(const std::string & str)
{ return new Int16Constant(atoi(str.c_str())); }

template<>
int AST::Int16Constant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::Int16Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::NumericExpr * AST::Int32Constant::Create(const std::string & str)
{ return new Int32Constant(atoi(str.c_str())); }

template<>
int AST::Int32Constant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::Int32Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::NumericExpr * AST::SingleConstant::Create(const std::string & str)
{ return new SingleConstant(atof(str.c_str())); }

template<>
int AST::SingleConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::SingleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::NumericExpr * AST::DoubleConstant::Create(const std::string & str)
{ return new DoubleConstant(atof(str.c_str())); }

template<>
int AST::DoubleConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::DoubleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

int AST::Goto::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }
