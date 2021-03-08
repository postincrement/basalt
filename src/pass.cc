#include <iostream>

#include "pass.h"

using namespace std;

Pass1::Pass1(const AST::Parser & parser)
  : m_parser(parser)
{
}

bool Pass1::Run()
{
  cout << "Vars\n"
       << "----\n";
  for (auto & var : m_parser.m_globalVars) {
    cout << "  " << var.first << endl;
  }
  cout << endl;

  cout << "Goto/Gosub targets\n"
       << "------------------\n";
  for (auto & target : m_parser.m_jumpDestinationInfo) {
    cout << "  " << target.first << endl;
  }
  cout << endl;

  cout << "String Constants\n"
       << "----------------\n";
  for (auto & var : m_parser.m_stringConstants) {
    cout << "  \"" << var.first << "\"" << endl;
  }

  cout << endl;
  for (auto & line : m_parser.m_program.m_list) {
    if (line == nullptr)
      continue;
    if (m_parser.m_jumpDestinationInfo.count(line->GetBasicLineNumber()) > 0)  
      cout << line->GetBasicLineNumber() << endl;
    for (auto & statement : line->m_statements->m_list) {
      cout << "   " << statement->m_text << endl;
    }
  }
  return true;
}
