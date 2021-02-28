#include <iostream>

#include "pass.h"

using namespace std;

Pass1::Pass1(const AST::Program & g_program)
  : m_program(g_program)
{
}

bool Pass1::Run()
{
  for (auto & line : m_program.m_list) {
    if (line == nullptr)
      continue;
    cout //<< "# " << line->GetLine() << endl
         << line->GetSourceLineNumber() << ": " << line->GetBasicLineNumber() << endl;
         #if 0
    for (auto & statement : line->m_statements->m_list) {
      if (statement != nullptr)     
        cout << "   : " << statement->m_text << endl;
    }
    cout << endl;
    #endif
  }
  return true;
}
