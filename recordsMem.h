#if !defined(__MY_RECORDS_MEM_H__)
#define __MY_RECORDS_MEM_H__

#include <shared_mutex>
#include <vector>

template<class _Ty>
class RecordsMem{
public:    
	typedef std::list<Ty> DATALIST;
private:
  DATALIST dataList;
  std::shared_mutex                       mutex;

  CHqRecordsMem()
  {      
  }
public:
    static CHqRecordsMem* getInstance(){
        static CHqRecordsMem m;
        return &m;
    }
  void resetAll(const DATALIST& p){
      mutex.lock();
      dataList.clear();
      dataList.assign(p)
      mutex.unlock();
  }

  void runAllItem(std::function<const DATALIST&> f){
      mutex.lock_shared();
      f(dataList);
      mutex.unlock_shared();
  }
};

#endif