#if !defined(__HQ_INSTRUMENT_FIELD_H__)
#define __HQ_INSTRUMENT_FIELD_H__

#include "../../third/ctp/ThostFtdcUserApiStruct.h"
#include <shared_mutex>

template<class _Ty>
class CHqRecordsMem{
private:
  int ppRecordsCount;
  _Ty **ppRecords;
  std::shared_mutex                       mutex;
public:
    CHqRecordsMem(){
        ppRecords = 0;
        ppRecordsCount = 0;
    }
  void resetAll(_Ty **ppRecords, int ppRecordsCount){
      mutex.lock();
      if(this->ppRecords){
          for (int i = 0; i < this->ppRecordsCount;++i)
				free(this->ppRecords[i]);
			free(this->ppRecords);
      }
      this->ppRecords = ppRecords;
      this->ppRecordsCount = ppRecordsCount;
      mutex.unlock();
  }
};

#endif