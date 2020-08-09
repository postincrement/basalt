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

#define STRM_STR(x, v) do { std::stringstream strm; strm << v; x = strm.str(); } while(0)
#define STRM_STR_DECL(x, v) std::string x; do { std::stringstream strm; strm << v; x = strm.str(); } while(0)

#include <iostream>
#include <iomanip>

#define   HEXFORMAT2(val) std::hex << std::setw(2) << std::setfill('0') << ((unsigned int)(val) & 0xff) << std::dec
#define   HEXFORMAT4(val) std::hex << std::setw(4) << std::setfill('0') << ((unsigned int)(val) & 0xffff) << std::dec
#define   HEXFORMAT8(val) std::hex << std::setw(8) << std::setfill('0') << ((unsigned int)(val)) << std::dec

#define   HEXFORMAT0x2(val) "0x" << HEXFORMAT2(val)
#define   HEXFORMAT0x4(val) "0x" << HEXFORMAT4(val)
#define   HEXFORMAT0x8(val) "0x" << HEXFORMAT8(val)

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

#endif // COMMON_H_
