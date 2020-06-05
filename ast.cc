#include <typeinfo>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

AST::Program AST::g_program;
AST::VarList AST::g_globalVars;

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
                      int lineNumber,
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

String::String(const std::string * str)
{
  if (str != nullptr)
    m_val = *str;
}

/////////////////////////////////////////

Assign::Assign(AST::VarRef * lhs, Expr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

int Assign::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

/////////////////////////////////////////

bool BinaryOperation::Validate(std::string & msg)
{
  VarType ltype = m_lhs->GetType();
  VarType rtype = m_rhs->GetType();

  if (ltype != rtype) {
    if (ltype == VarType::eString) {
      msg = "error: rhs must be string type";
      return false;
    }
    if (rtype == VarType::eString) {
      msg = "error: lhs must be string type";
      return false;
    }
    if (rtype == VarType::eDouble || ltype == VarType::eDouble)
      m_type = VarType::eDouble;
    else if (rtype == VarType::eSingle || ltype == VarType::eSingle)
      m_type = VarType::eSingle;
    else 
      m_type = g_languageProfile->GetIntegerType();
  }

  return true;
}


int Addition::Generate(CodeGenerator & gen) const
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

/////////////////////////////////////////

VarRef::VarRef(VarType type, const std::string & varName)
  : Expr(type)
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
}

int VarRef::Generate(CodeGenerator & gen) const
{
  return gen.Generate(*this);
}

int VarRef::Print(CodeGenerator & gen) const
{
  return gen.Print(*this);
}

/////////////////////////////////////////

template<>
AST::Expr * AST::StringConstant::Create(const std::string & str)
{
  return new StringConstant(str.c_str());
}

template<>
int AST::StringConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::StringConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::Int16Constant::Create(const std::string & str)
{
  int16_t v = atoi(str.c_str());
  cout << "'" << str << "' = " << v << endl; 
  return new Int16Constant(atoi(str.c_str()));
}

template<>
int AST::Int16Constant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::Int16Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::Int32Constant::Create(const std::string & str)
{
  return new Int32Constant(atoi(str.c_str()));
}

template<>
int AST::Int32Constant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::Int32Constant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::SingleConstant::Create(const std::string & str)
{
  return new SingleConstant(atof(str.c_str()));
}

template<>
int AST::SingleConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::SingleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::DoubleConstant::Create(const std::string & str)
{
  return new DoubleConstant(atof(str.c_str()));
}
template<>
int AST::DoubleConstant::Generate(CodeGenerator & gen) const
{ return gen.Generate(*this); }

template<>
int AST::DoubleConstant::Print(CodeGenerator & gen) const
{ return gen.Print(*this); }

