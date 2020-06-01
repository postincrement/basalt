#include <typeinfo>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

AST::NodeList AST::g_program;
AST::VarList AST::g_globalVars;

/////////////////////////////////////////

int AST::Node::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int AST::Node::Print(CodeGenerator & gen)
{ 
  return gen.Print(*this);
}

void AST::NodeList::Append(Node * node)
{
  if (node != nullptr) {
    m_list.push_back(std::unique_ptr<Node>(node));
  }
}

void AST::NodeList::Append(NodeList * nodeList)
{
  if (nodeList != nullptr) {
    for (auto & r : nodeList->m_list) {
      Append(r.release());
    }
  }
}

/////////////////////////////////////////

SourceLine::SourceLine(int lineNumber, const std::string & line)
  : m_lineNumber(lineNumber)
  , m_line(line)
{
  size_t len = m_line.length();
  while ((len > 0) && (isspace(m_line[len-1]))) {
    --len;
  }
  m_line = m_line.substr(0, len);
}

int SourceLine::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

////////////////////////////////////////////////////////////////////////////

LineNumber::LineNumber(const std::string & ref)
  : m_ref(ref)
{}

int LineNumber::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

/////////////////////////////////////////

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

int Print::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PrintComma::Print(CodeGenerator & gen)
{
  return gen.Print(*this);
}

int PrintSemiColon::Print(CodeGenerator & gen)
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

int Assign::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

/////////////////////////////////////////

int Addition::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int Subtraction::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int Multiplication::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int Division::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int Negation::Generate(CodeGenerator & gen)
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

int VarRef::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int VarRef::Print(CodeGenerator & gen)
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
int AST::StringConstant::Generate(CodeGenerator & gen)
{ return gen.Generate(*this); }

template<>
int AST::StringConstant::Print(CodeGenerator & gen)
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
int AST::Int16Constant::Generate(CodeGenerator & gen)
{ return gen.Generate(*this); }

template<>
int AST::Int16Constant::Print(CodeGenerator & gen)
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::Int32Constant::Create(const std::string & str)
{
  return new Int32Constant(atoi(str.c_str()));
}

template<>
int AST::Int32Constant::Generate(CodeGenerator & gen)
{ return gen.Generate(*this); }

template<>
int AST::Int32Constant::Print(CodeGenerator & gen)
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::SingleConstant::Create(const std::string & str)
{
  return new SingleConstant(atof(str.c_str()));
}

template<>
int AST::SingleConstant::Generate(CodeGenerator & gen)
{ return gen.Generate(*this); }

template<>
int AST::SingleConstant::Print(CodeGenerator & gen)
{ return gen.Print(*this); }

/////////////////////////////////////////

template<>
AST::Expr * AST::DoubleConstant::Create(const std::string & str)
{
  return new DoubleConstant(atof(str.c_str()));
}
template<>
int AST::DoubleConstant::Generate(CodeGenerator & gen)
{ return gen.Generate(*this); }

template<>
int AST::DoubleConstant::Print(CodeGenerator & gen)
{ return gen.Print(*this); }

