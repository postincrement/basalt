#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <map>
#include <memory>


namespace std {
  template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

std::string Trim(const std::string & str);
void Tokenize(std::vector<std::string> & tokens, const std::string & str, char sep);

//////////////////////////////////////////////////////////////////

class Filename : public std::string
{
  public:
    Filename();
    Filename(const std::string & str);

    Filename & operator =(const std::string & str);

    std::string GetDir() const;
    std::string GetFilename() const;
    std::string GetBasename() const;
    std::string GetExtension() const;
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

#if 0

//////////////////////////////////////////////////////////////////

typedef std::vector<unsigned> UnsignedList;

struct Variable
{
  enum Type {
    eString,
    eInt16,
    eSingle,
    eDouble,
    eStringConst,
    eUntyped
  };

  Variable(Type type, const std::string & name)
    : m_type(type)
    , m_name(name)
    {
    }

  Type m_type;
  std::string m_name;  
  std::string m_normalizedName;
};

//////////////////////////////////////////////////////////////////

#endif

enum class VarType {
  eNone,
  eInt16,
  eInt32,
  eSingle,
  eDouble,
  eString
};

struct LanguageProfile
{
  //virtual bool NormalizeVariableName( & var, int dim) = 0;
  virtual VarType GetIntegerType() = 0;
  virtual VarType GetDefaultNumericType() = 0;
  int m_normalizedVarLen;
};

struct BasicLanguageProfile : public LanguageProfile
{
  BasicLanguageProfile(int normVarLen);
  //virtual bool NormalizeVariableName(Variable & var, int dim) override;
};

struct Basic_8k_LanguageProfile : public BasicLanguageProfile
{
  Basic_8k_LanguageProfile();
  virtual VarType GetIntegerType();
  virtual VarType GetDefaultNumericType();
};

struct Basic_Extended_LanguageProfile : public BasicLanguageProfile
{
  Basic_Extended_LanguageProfile();
  virtual VarType GetIntegerType();
  virtual VarType GetDefaultNumericType();
};

struct Basic_Disk_LanguageProfile : public BasicLanguageProfile
{
  Basic_Disk_LanguageProfile();
  virtual VarType GetIntegerType();
  virtual VarType GetDefaultNumericType();
};

//////////////////////////////////////////////////////////////////

extern LanguageProfile * g_languageProfile;

#endif // COMMON_H_
