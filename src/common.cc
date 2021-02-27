#include <string>
using namespace std;

#include "common.h"

Filename::Filename()
{ }

Filename::Filename(const std::string & str)
  : std::string(str)
{ }

Filename & Filename::operator =(const std::string & str)
{ this->std::string::operator=(str); return *this; }

std::string Filename::GetDir() const
{ 
  size_t pos = find_last_of('/');
  if (pos == std::string::npos)
    return "";
  return substr(0, pos);
}

std::string Filename::GetFilename() const
{ 
  std::string fn;
  size_t pos = find_last_of('/');
  if (pos == std::string::npos)
    fn = *this;
  else
    fn = substr(pos+1);
  return fn;
}

std::string Filename::GetBasename() const
{ 
  std::string base = GetFilename();      
  size_t pos = base.find_last_of('.');      
  if (pos != std::string::npos)
    base = base.substr(0, pos);
  return base;
}

std::string Filename::GetExtension() const
{ 
  std::string ext = GetFilename();      
  size_t pos = ext.find_last_of('.');      
  if (pos == std::string::npos)
    return "";
  return ext.substr(pos);
}

std::string Trim(const std::string & str_)
{
  if (str_.length() < 1)
    return str_;

  std::string str(str_);  

  const char * start = str.c_str();
  const char * ptr = start;

  // trim left
  while (isspace(*ptr))
    ++ptr;

  str = str.substr(ptr - start);

  // trim right
  if (str.length() > 0) {
    start = str.c_str();
    ptr = start + str.length() - 1;
    while ((ptr > start) && isspace(*ptr))
      --ptr;
    str = str.substr(0, ptr - start + 1);
  }

  return str;
}

void Tokenize(std::vector<std::string> & tokens, const std::string & str, char sep)
{
  const char * ptr = str.c_str();
  while (*ptr != '\0') {
    const char * start = ptr;
    while ((*ptr != '\0') && (*ptr != sep))
      ++ptr;
    tokens.push_back(std::string(start, ptr - start));
    if (*ptr != '\0')
      ++ptr;
  }
}
