#include <typeinfo>

using namespace std;

#include "ast.h"

using namespace AST;

AST::ExprList AST::g_program;

/////////////////////////////////////////

AST::Expr::~Expr()
{
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
  }
}

size_t AST::ExprList::Length() const
{
  return m_list.size();
}


/////////////////////////////////////////

AST::LineNumber::LineNumber(const std::string & str)
  : m_lineNumber(str)
{
}

void AST::LineNumber::PrintOn(ostream & strm) const
{
  strm << "linenumber '" << m_lineNumber << "'" << endl;
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
  strm << "comma" << endl;
};

void PrintSemiColon::PrintOn(std::ostream & strm) const
{
  strm << "semicolon" << endl;
}

/////////////////////////////////////////

String::String(const std::string * str)
{
  m_val = *str;
}

void String::PrintOn(std::ostream & strm) const
{
  strm << "string '" << m_val << "'" << endl;
}
