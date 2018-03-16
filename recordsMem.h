#if !defined(__MY_RECORDS_MEM_H__)
#define __MY_RECORDS_MEM_H__

#include <shared_mutex>
#include <list>
#include <functional>


template<class _Ty>
class RecordsMem{
public:    
	typedef std::list<_Ty> DATALIST;
private:
  DATALIST dataList;
  std::shared_mutex                       m;

  RecordsMem()
  {      
  }
public:
    static RecordsMem* getInstance(){
        static RecordsMem m;
        return &m;
    }
  void resetAll(const DATALIST& p){
      m.lock();
      dataList.clear();
      dataList.assign(p);
      m.unlock();
  }

  void runAllItem(std::function<void(const DATALIST&)> f){
      m.lock_shared();
      f(dataList);
      m.unlock_shared();
  }
};

#endif