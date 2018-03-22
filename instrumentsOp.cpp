#include "instrumentsOp.h"
#include "recordsMem.h"
#include <set>
#include <list>
#include "third/ctp/ThostFtdcUserApiStruct.h"
#include <algorithm>

void CInstrumentsOp::resetAll(){
  CInstrumentsOp* self = this;
  RecordsMem<CThostFtdcInstrumentField>::getInstance()->runAllItem([&](const std::list<CThostFtdcInstrumentField>& items){
    if(items.empty())
      return;
    
    
  });
}

std::string CInstrumentsOp::getAllStks(const char *stk){
  // std::list<std::string> validStks;
  // std::list<std::string> stks = SplitString(stk, ",");
  // std::set<std::string> regedStks;

  // time_t now;
  // time(&now);  
  // struct tm * nowTm = gmtime(&now);
  // char curYYMM[32];
  // sprintf(curYYMM, "%02d%02d", nowTm->tm_year % 100, nowTm->tm_mon + 1);

  // mainInstrumentsMapLocker.lock_shared();

  // int i = 0;
  // for (std::list<std::string>::iterator b = stks.begin(), e = stks.end(); b != e; ++b)
  // {
  //     if(((*b).length()==3) && (0==(*b).substr(2,1).compare("0"))){
  //         b->resize(7);
  //         strcpy((char *)(b->c_str() + 2), curYYMM);

  //         mainInstrumentsMapLocker[*b]
  //     }

  //     auto i = regedStks.insert(*b);
  //     if(!i.second)
  //       continue;

  //     validStks.push_back(*b);
  // }


  // mainInstrumentsMapLocker.unlock_shared();

  return "";
}