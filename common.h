#ifndef COMMON_H_
#define COMMON_H_

#include <string>

class Filename : public std::string
{
  public:
    Filename()
    { }

    Filename(const std::string & str)
      : std::string(str)
    { }

    Filename & operator =(const std::string & str)
    { this->std::string::operator=(str); return *this; }

    std::string GetDir() const
    { 
      size_t pos = find_last_of('/');
      if (pos == std::string::npos)
        return "";
      return substr(0, pos);
    }

    std::string GetFilename() const
    { 
      std::string fn;
      size_t pos = find_last_of('/');
      if (pos == std::string::npos)
        fn = *this;
      else
        fn = substr(pos+1);
      return fn;
    }

    std::string GetBasename() const
    { 
      std::string base = GetFilename();      
      size_t pos = base.find_last_of('.');      
      if (pos != std::string::npos)
        base = base.substr(0, pos);
      return base;
    }

    std::string GetExtension() const
    { 
      std::string ext = GetFilename();      
      size_t pos = ext.find_last_of('.');      
      if (pos == std::string::npos)
        return "";
      return ext.substr(pos);
    }
};

#endif // COMMON_H_
