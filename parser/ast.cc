#include <typeinfo>

using namespace std;

#include "parser/ast.h"
#include "codegen.h"

using namespace AST;

#define BASIC_STRING_SUFFIX  "$"
#define BASIC_INT_SUFFIX     "%"
#define BASIC_SINGLE_SUFFIX  "!"
#define BASIC_DOUBLE_SUFFIX  "#"


// must be indexed by VarType
static VarTypeInfoRec g_varTypeInfo[] ={
    { 0                                },   // none
    { "int16_t",  BASIC_INT_SUFFIX     },   // eInt16
    { "int32_t",  BASIC_INT_SUFFIX     },   // eInt32
    { "float",    BASIC_SINGLE_SUFFIX  },   // eSingle
    { "double",   BASIC_DOUBLE_SUFFIX  },   // eDouble
    { "string",   BASIC_STRING_SUFFIX  },   // eString
};

AST::Parser * AST::g_parser = nullptr;

/////////////////////////////////////////

VarTypeInfoRec & AST::GetVarTypeInfo(VarType type)
{
  return g_varTypeInfo[(int)type];
}

/////////////////////////////////////////

int Node::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

int Node::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

int Node::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
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

int SourceLine::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

////////////////////////////////////////////////////////////////////////////

int End::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

int System::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

int Rem::Generate(PseudoCodeGenerator & gen) const
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
{ m_list = list; }

int Print::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

int PrintComma::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

int PrintSemiColon::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

int NumericExpr::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

int StringExpr::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

StringAssign::StringAssign(const StringVarRef * lhs, const StringExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{ }

int StringAssign::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

bool StringAssign::Validate()
{ return false; }

/////////////////////////////////////////

int StringAddition::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
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

  int varNameLen = g_parser->m_languageProfile->GetVarNameLen();
  m_name = name.substr(0, varNameLen) + BASIC_STRING_SUFFIX;
}

int StringVarRef::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

int StringVarRef::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

StringConstant::StringConstant(const std::string & str)
  : m_value(str)
{
  m_index = g_parser->AddStringConstant(str);
}

int StringConstant::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

int StringConstant::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

int AssignStatement::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

/////////////////////////////////////////

NumericAssign::NumericAssign(const NumericVarRef * lhs, const NumericExpr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{ }

int NumericAssign::Generate(PseudoCodeGenerator & gen) const
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
    msg = "error: rhs cannot be string type";
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

  VarType etype = g_parser->m_languageProfile->GetIntegerType();

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

int NumericAddition::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Subtraction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Multiplication::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Division::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Negation::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int Power::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericCast::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int IntFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int SqrFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int AbsFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int RndFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int LenFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int TabFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int LeftFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int MidFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int RightFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int ChrFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int StrFunction::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericEquality::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericNotEquality::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericGreaterThan::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericGreaterThanEqual::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericLessThan::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericLessThanEqual::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
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
      strType = g_parser->m_languageProfile->GetIntegerType();
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

    suffix = GetVarTypeInfo(type).m_suffix;
  }

  m_originalName = name + suffix;

  int varNameLen = g_parser->m_languageProfile->GetVarNameLen();
  m_name = name.substr(0, varNameLen) + suffix;
  m_type = type;

  if ((int)m_type == 0) {
    cerr << varName << " has type 0" << endl; 
  }
}

int NumericVarRef::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{
  return gen.Evaluate(*this, result);
}

int NumericVarRef::Print(PseudoCodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

int NumericConstant::Evaluate(PseudoCodeGenerator & gen, std::string & result) const
{ return gen.Evaluate(*this, result); }

/////////////////////////////////////////

template<>
NumericExpr * Int16Constant::Create(const std::string & str)
{ return new Int16Constant(atoi(str.c_str())); }

template<>
int Int16Constant::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * Int32Constant::Create(const std::string & str)
{ return new Int32Constant(atoi(str.c_str())); }

template<>
int Int32Constant::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * SingleConstant::Create(const std::string & str)
{ return new SingleConstant(atof(str.c_str())); }

template<>
int SingleConstant::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
NumericExpr * DoubleConstant::Create(const std::string & str)
{ return new DoubleConstant(atof(str.c_str())); }

template<>
int DoubleConstant::Print(PseudoCodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

JumpStatement::JumpStatement(unsigned lineNumber, const std::string & ref)
  : Statement(lineNumber)
  , m_ref(ref)
{
  g_parser->AddJumpTo(lineNumber, ref);
}

int GotoStatement::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

int GosubStatement::Generate(PseudoCodeGenerator & gen) const
{  return gen.Generate(*this); }

int ReturnStatement::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

/////////////////////////////////////////

OnGotoStatement::OnGotoStatement(unsigned lineNumber, const OnRefList & onRefs)
  : Statement(lineNumber)
  , m_onRefs(onRefs)
{
  for (auto & r : m_onRefs)
    g_parser->AddJumpTo(lineNumber, r);
}

int OnGotoStatement::Generate(PseudoCodeGenerator & gen) const
{ return gen.Generate(*this); }

/////////////////////////////////////////

IfStatement::IfStatement(unsigned lineNumber,
                         const NumericExpr * cond, 
                         const IfConditional * trueStatements,
                         const IfConditional * falseStatements)
  : Statement(lineNumber)
  , m_cond(cond)
  , m_trueStatements(trueStatements)
  , m_falseStatements(falseStatements)
{
  if (!m_trueStatements->m_lineNumber.empty()) {
    g_parser->AddJumpTo(lineNumber, m_trueStatements->m_lineNumber);
  }
  if (m_falseStatements && !m_falseStatements->m_lineNumber.empty()) {
    g_parser->AddJumpTo(lineNumber, m_falseStatements->m_lineNumber);
  }
}

int IfStatement::Generate(PseudoCodeGenerator & gen) const
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

int ForStatement::Generate(PseudoCodeGenerator & gen) const
{
  return gen.Generate(*this);
}

NextStatement::NextStatement(unsigned lineNumber, const NumericVarRef * var)
  : Statement(lineNumber)
  , m_var(var)
{ }

int NextStatement::Generate(PseudoCodeGenerator & gen) const
{
  return gen.Generate(*this);
}

////////////////////////////////////////////////////////////////

Parser::Parser()
{
  g_parser = this;
}

int Parser::AddStringConstant(const std::string & str)
{
  int index;
  if (m_stringConstants.count(str) != 0) {
    index = m_stringConstants[str];
  }
  else {
    index = m_stringConstantIndex++;
    m_stringConstants[str] = index;
  }

  return index;
}

void Parser::AddJumpTo(int sourceLineNumber, const std::string & basicLineRef)
{
  auto & info = m_jumpDestinationInfo[basicLineRef];
  info.m_count++;
  info.m_usedLine.insert(sourceLineNumber);
}

AST::VarInfo * Parser::GetVar(const std::string & name)
{
  auto r = m_globalVars.find(name);
  if (r == m_globalVars.end())
    return nullptr;
  return &r->second;
}

std::string Parser::DefineVar(const std::string & name,
                       const std::string & originalName,
                       VarType varType,
                       int leftRef,
                       int rightRef)
{
  std::string synonym;

  AST::VarInfo * var = GetVar(name);
  if (var == nullptr) {
    AST::VarInfo info;
    info.m_type         = varType;
    info.m_originalName = originalName; 
    if (leftRef >= 0)
      info.m_lhsLine = leftRef;
    if (rightRef >= 0)   
      info.m_rhsLine  = rightRef;
    m_globalVars[name] = info;
  }
  else {
    if (var->m_originalName != originalName) {
      synonym = var->m_originalName;
    }
    if ((leftRef >= 0) && (var->m_lhsLine == 0))
      var->m_lhsLine = leftRef;
    if ((rightRef >= 0) && (var->m_rhsLine == 0))
      var->m_rhsLine = rightRef;
  }

  return synonym;
}

bool Parser::StartLine(const std::string * lineRef)
{
  m_basicLineNumber = "";
  if (lineRef != nullptr) {
    m_basicLineNumber = *lineRef;
    if (m_lineNumberInfo.count(m_basicLineNumber) != 0) {
      return false;
    }
    m_lineNumberInfo[m_basicLineNumber] = m_lexLineNumber;
  }

  return true;
}
