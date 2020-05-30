#include <typeinfo>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

AST::NodeList AST::g_program;

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

VarRef::VarRef(VarType type, const std::string & id)
  : Expr(type)
  , m_id(id)
{
}

std::string VarRef::GetName() const
{
  return m_id;
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

