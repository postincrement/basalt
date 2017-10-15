#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <map>


//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////

template <class Abstract, typename ... TArgs>
class Factory
{
  public:
    Factory()
    { }

    struct AbstractWorker
    {
      virtual Abstract * Create(TArgs ... args) = 0; 
    };

    template <class Concrete>
    struct Worker : public AbstractWorker
    {
      Worker()
      { }

      virtual Abstract * Create(TArgs ... args) override
      { return new Concrete(args...); }
    };

    template<class Concrete>
    void Register(const std::string & key)
    {
      m_workers[key] = new Worker<Concrete>();
    }

    std::vector<std::string> GetList() const
    {
      std::vector<std::string> types;
      for (auto & r : m_workers)
        types.push_back(r.first);

      return types;
    }

    Abstract * CreateInstance(const std::string & key, TArgs ... args)
    {
      typename WorkerListType::iterator r = m_workers.find(key);
      if (r == m_workers.end())
        return nullptr;
      AbstractWorker * worker = r->second;  
      return worker->Create(args ...);  
    }

    bool Contains(const std::string & key) const
    {
      return m_workers.count(key) != 0;
    }

    typedef std::map<std::string, AbstractWorker *> WorkerListType;    
    WorkerListType m_workers;
};

//////////////////////////////////////////////////////////////////

struct Variable
{
  enum Type {
    eString,
    eInt16,
    eSingle,
    eDouble,
    eUntyped
  };

  Variable(Type type, const std::string & name)
    : m_type(type)
    , m_name(name)
  { }

  Type m_type;
  std::string m_name;  
  std::string m_normalizedName;
};

//////////////////////////////////////////////////////////////////

struct LanguageProfile
{
  virtual bool NormalizeVariableName(Variable & var) = 0;
  virtual Variable::Type GetDefaultNumericType() = 0;
  int m_normalizedVarLen;
};

struct BasicLanguageProfile : public LanguageProfile
{
  BasicLanguageProfile(int normVarLen);
  virtual bool NormalizeVariableName(Variable & var) override;
};

struct Basic_8k_LanguageProfile : public BasicLanguageProfile
{
  Basic_8k_LanguageProfile();
  virtual Variable::Type GetDefaultNumericType();
};

struct Basic_Extended_LanguageProfile : public BasicLanguageProfile
{
  Basic_Extended_LanguageProfile();
  virtual Variable::Type GetDefaultNumericType();
};

struct Basic_Disk_LanguageProfile : public BasicLanguageProfile
{
  Basic_Disk_LanguageProfile();
  virtual Variable::Type GetDefaultNumericType();
};

//////////////////////////////////////////////////////////////////

extern LanguageProfile * g_languageProfile;

#endif // COMMON_H_
