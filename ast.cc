#include <typeinfo>

using namespace std;

#include "ast.h"

using namespace AST;

AST::NodeList AST::g_program;

/////////////////////////////////////////

Node::Node()
{}

Node::~Node()
{}

/////////////////////////////////////////

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

void AST::NodeList::PrintOn(ostream & strm) const
{
  for (auto & r : m_list) {
    r->PrintOn(strm);
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

void SourceLine::PrintOn(std::ostream & strm) const
{
  strm << "# " << m_lineNumber << " \"" << m_line << "\"" << endl;
}

////////////////////////////////////////////////////////////////////////////

LineNumber::LineNumber(const std::string & ref)
  : m_ref(ref)
{}

void LineNumber::PrintOn(std::ostream & strm) const
{
//  strm << "# " << m_ref << endl;
}

/////////////////////////////////////////

AST::Expr::Expr(VarType type)
  : m_type(type)
{
}

void Expr::PrintOn(std::ostream & strm) const
{
  if (m_type == VarType::eNone)
    return;

  strm << "(";
  switch (m_type) {
    case VarType::eNone:
      break;
    case VarType::eDefault:
      strm << "default";
      break;
    case VarType::eInteger:
      strm << "integer";
      break;
    case VarType::eSingle:
      strm << "single";
      break;
    case VarType::eDouble:
      strm << "double";
      break;
    case VarType::eString:
      strm << "string";
      break;
  }
  strm << ")"; 
}
/////////////////////////////////////////

void AST::ExprList::Append(Expr * expr)
{
  if (expr != nullptr) {
    m_list.push_back(std::unique_ptr<Expr>(expr));
  }
}

void AST::ExprList::Append(ExprList * exprList)
{
  if (exprList != nullptr) {
    for (auto & r : exprList->m_list) {
      Append(r.release());
    }
  }
}

void AST::ExprList::PrintOn(ostream & strm) const
{
  for (auto & r : m_list) {
    r->PrintOn(strm);
    strm << endl;
  }
}

size_t AST::ExprList::Length() const
{
  return m_list.size();
}

/////////////////////////////////////////

AST::Print::Print(ExprList * list)
{
  Append(list);
}

void AST::Print::PrintOn(ostream & strm) const
{
  strm << "PRINT {" << endl;
  ExprList::PrintOn(strm);
  strm << "}" << endl;
}

void PrintComma::PrintOn(std::ostream & strm) const
{
  strm << "comma";
};

void PrintSemiColon::PrintOn(std::ostream & strm) const
{
  strm << "semicolon";
}

/////////////////////////////////////////

String::String(const std::string * str)
{
  if (str != nullptr)
    m_val = *str;
}

void String::PrintOn(std::ostream & strm) const
{
  strm << "string '" << m_val << "'" << endl;
}

/////////////////////////////////////////

Assign::Assign(Expr * lhs, Expr * rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

void Assign::PrintOn(std::ostream & strm) const
{
  strm << "ASSIGN ";
  if (m_lhs == nullptr)
    strm << "(null)";
  else
    m_lhs->PrintOn(strm);
    
  strm << " = ";

  if (m_rhs == nullptr)
    strm << "(null)";
  else
    m_rhs->PrintOn(strm);
  strm << endl;  
}

/////////////////////////////////////////

VarRef::VarRef(VarType type, const std::string & id)
  : Expr(type)
  , m_id(id)
{
}

void VarRef::PrintOn(std::ostream & strm) const
{
  strm << "'" << m_id << "'";
  Expr::PrintOn(strm);
}

/////////////////////////////////////////

DefaultValue::DefaultValue(const std::string & str)
  : Expr(VarType::eDefault)
  , m_value(str)
{}

void DefaultValue::DefaultValue::PrintOn(std::ostream & strm) const
{
  Expr::PrintOn(strm);
  strm << m_value;
}

StringValue::StringValue(const std::string & str)
  : Expr(VarType::eString)
  , m_value(str)
{}

void StringValue::PrintOn(std::ostream & strm) const
{
  Expr::PrintOn(strm);
  strm << m_value;
}

IntegerValue::IntegerValue(int value)
  : Expr(VarType::eInteger)
  , m_value(value)
{}

void IntegerValue::PrintOn(std::ostream & strm) const
{
  Expr::PrintOn(strm);
  strm << m_value;
}

SingleValue::SingleValue(double value)
  : Expr(VarType::eSingle)
  , m_value(value)
{}

void SingleValue::PrintOn(std::ostream & strm) const
{
  Expr::PrintOn(strm);
  strm << m_value;
}

DoubleValue::DoubleValue(double value)
  : Expr(VarType::eDouble)
  , m_value(value)
{}

void DoubleValue::PrintOn(std::ostream & strm) const
{
  Expr::PrintOn(strm);
  strm << m_value;
}
