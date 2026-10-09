#ifndef IRUTIL_H_
#define IRUTIL_H_

#include "ast.h"

#include <cctype>
#include <cstdlib>
#include <map>
#include <string>

inline bool IsNumberLiteral(const std::string & text)
{
  if (text.empty())
    return false;
  char * end = nullptr;
  std::strtod(text.c_str(), &end);
  return end != text.c_str() && end != nullptr && *end == '\0';
}

inline const char * CTypeName(VarType type)
{
  switch (type) {
    case VarType::eInt16: return "int16_t";
    case VarType::eInt32: return "int32_t";
    case VarType::eSingle: return "float";
    case VarType::eDouble: return "double";
    case VarType::eString: return "char *";
    default: return "int";
  }
}

inline int TypeWidth(VarType type)
{
  switch (type) {
    case VarType::eInt16: return 2;
    case VarType::eInt32: return 4;
    case VarType::eSingle: return 4;
    case VarType::eDouble: return 8;
    default: return 2;
  }
}

inline bool IsFloatType(VarType type)
{
  return type == VarType::eSingle || type == VarType::eDouble;
}

// BASIC names become identifiers. Temps and string-constant labels pass through.
inline std::string Mangle(const std::string & name)
{
  if (name.rfind("temp_", 0) == 0 || name.rfind("str_", 0) == 0)
    return name;
  std::string out = "v_";
  for (unsigned char ch : name) {
    if (std::isalnum(ch))
      out += static_cast<char>(ch);
    else if (ch == '$')
      out += "_s";
    else if (ch == '!')
      out += "_f";
    else if (ch == '#')
      out += "_d";
    else if (ch == '%')
      out += "_i";
    else
      out += '_';
  }
  return out;
}

inline std::string EscapeC(const std::string & text)
{
  std::string out;
  for (unsigned char ch : text) {
    if (ch == '\\' || ch == '"') {
      out += '\\';
      out += static_cast<char>(ch);
    }
    else if (ch == '\n')
      out += "\\n";
    else if (ch == '\r')
      out += "\\r";
    else if (ch == '\t')
      out += "\\t";
    else if (ch < 32 || ch > 126) {
      char buf[8];
      snprintf(buf, sizeof buf, "\\%03o", ch);
      out += buf;
    }
    else
      out += static_cast<char>(ch);
  }
  return out;
}

inline bool IsArrayName(const std::string & name)
{
  return AST::g_userFunctions.count(name) == 0 && AST::g_arrayBounds.count(name) != 0;
}

inline int ArrayLength(const std::string & name)
{
  auto it = AST::g_arrayBounds.find(name);
  if (it == AST::g_arrayBounds.end() || it->second.empty())
    return 11;
  int count = 1;
  for (int upper : it->second)
    count *= (upper + 1);
  return count;
}

inline void ApplyDefaultArrayBounds()
{
  for (auto & entry : AST::g_subscriptArity) {
    if (AST::g_userFunctions.count(entry.first) != 0)
      continue;
    if (AST::g_arrayBounds.count(entry.first) == 0)
      AST::g_arrayBounds[entry.first] = std::vector<int>(entry.second, 10);
  }
}

#endif
