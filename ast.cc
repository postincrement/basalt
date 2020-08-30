#include <typeinfo>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

Program AST::g_program;
VarList AST::g_globalVars;
LineNumberInfo AST::g_lineNumberInfo;
JumpDestinationList AST::g_jumpDestinationInfo;
StringConstantList AST::g_stringConstants;
unsigned AST::g_stringConstantIndex = 0;

#define BASIC_STRING_SUFFIX  "$"
#define BASIC_INT_SUFFIX     "%"
#define BASIC_SINGLE_SUFFIX  "!"
#define BASIC_DOUBLE_SUFFIX  "#"

const char * g_basicVarSuffixes[] = {
  "", 
  BASIC_INT_SUFFIX, BASIC_INT_SUFFIX, 
  BASIC_SINGLE_SUFFIX, BASIC_DOUBLE_SUFFIX, 
  BASIC_STRING_SUFFIX
};

/////////////////////////////////////////

int Node::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int Node::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

int Node::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

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
{ return gen.Generate(*this); }

////////////////////////////////////////////////////////////////////////////

int End::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int System::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int Rem::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

////////////////////////////////////////////////////////////////////////////

Expr::Expr(VarType type)
  : m_type(type)
{ }

VarType Expr::GetType() const
{ return m_type; }

/////////////////////////////////////////

Print::Print(unsigned lineNumber, ExprList * list)
  : Statement(lineNumber)
{ Append(list); }

int Print::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int PrintComma::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

int PrintSemiColon::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

int NumericExpr::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

StringAssign::StringAssign(const StringVarRef * lhs, const StringExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{ }

int StringAssign::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

bool StringAssign::Validate()
{ return false; }

/////////////////////////////////////////

int StringAddition::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

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

int StringVarRef::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

int StringVarRef::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

StringConstant::StringConstant(const std::string & str)
  : m_value(str)
{
  if (g_stringConstants.count(str) != 0) {
    m_index = g_stringConstants[str];
  }
  else {
    m_index = g_stringConstantIndex++;
    g_stringConstants[str] = m_index;
  }
}

int StringConstant::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

int StringConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

NumericAssign::NumericAssign(const NumericVarRef * lhs, const NumericExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{ }

int NumericAssign::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

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
  m_rhs = new NumericCast(ltype, m_rhs);

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
    m_lhs = new NumericCast(etype, m_lhs);
  }
  else if (rtype != etype) {
    m_rhs = new NumericCast(etype, m_rhs);
  }

  m_type = etype;

  return true;
}

int NumericAddition::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Subtraction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Multiplication::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Division::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Negation::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Power::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericCast::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int IntFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int SqrFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int LenFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int TabFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int LeftFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int MidFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int RightFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int ChrFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int StrFunction::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericEquality::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericNotEquality::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericGreaterThan::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericGreaterThanEqual::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericLessThan::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericLessThanEqual::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
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

int NumericVarRef::Evaluate(CodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericVarRef::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

template<>
NumericExpr * Int16Constant::Create(const std::string & str)
{ return new Int16Constant(atoi(str.c_str())); }

template<>
int Int16Constant::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

template<>
int Int16Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * Int32Constant::Create(const std::string & str)
{ return new Int32Constant(atoi(str.c_str())); }

template<>
int Int32Constant::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

template<>
int Int32Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * SingleConstant::Create(const std::string & str)
{ return new SingleConstant(atof(str.c_str())); }

template<>
int SingleConstant::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

template<>
int SingleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * DoubleConstant::Create(const std::string & str)
{ return new DoubleConstant(atof(str.c_str())); }

template<>
int DoubleConstant::Evaluate(CodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

template<>
int DoubleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

JumpStatement::JumpStatement(unsigned lineNumber, const std::string & ref)
  : Statement(lineNumber)
  , m_ref(ref)
{
  auto & info = g_jumpDestinationInfo[ref];
  info.m_count++;
  info.m_usedLine.insert(lineNumber);
}

int GotoStatement::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

int GosubStatement::Generate(CodeGenerator & gen) const
{  return gen.Generate(*this); }

int ReturnStatement::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

/////////////////////////////////////////

IfStatement::IfStatement(unsigned lineNumber,
                         const NumericExpr * cond, 
                         const IfConditional * trueStatements,
                         const StatementList * falseStatements)
  : Statement(lineNumber)
  , m_cond(cond)
  , m_trueStatements(trueStatements)
  , m_falseStatements(falseStatements)
{
  if (!m_trueStatements->m_lineNumber.empty()) {
    auto & info = g_jumpDestinationInfo[m_trueStatements->m_lineNumber];
    info.m_count++;
    info.m_usedLine.insert(lineNumber);
  }
}

int IfStatement::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

/////////////////////////////////////////

ForStatement::ForStatement(unsigned lineNumber,
              const NumericVarRef * var, 
              const NumericExpr * fromVal,
              const NumericExpr * toVal,
              const NumericExpr * stepVal)
  : Statement(lineNumber)
  , m_var(var)
  , m_fromVal(fromVal)
  , m_toVal(toVal)
  , m_stepVal(stepVal)
{ 
  m_type = (VarType)std::max<int>(
      (int)m_var->GetType(),
      (int)m_fromVal->GetType()
  );
  m_type = (VarType)std::max<int>((int)m_type, 
      (int)m_toVal->GetType()
  );
  m_type = (VarType)std::max<int>((int)m_type, 
      (m_stepVal ? (int)m_stepVal->GetType() : 0)
  );
}             

int ForStatement::Generate(CodeGenerator & gen) const
{
  gen.SetFORUsed();
  return gen.Generate(*this);
}

NextStatement::NextStatement(unsigned lineNumber, const NumericVarRef * var)
  : Statement(lineNumber)
  , m_var(var)
{ }

int NextStatement::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

