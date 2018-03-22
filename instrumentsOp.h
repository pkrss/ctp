#if !defined(__MY_INSTRUMENTS_OP_H__)
#define __MY_INSTRUMENTS_OP_H__

#include <map>
#include <string>
#include <shared_mutex>

class CInstrumentsOp{

public:
  void resetAll();

  std::string getAllStks(const char *stk);

 static CInstrumentsOp* getInstance(){
        static CInstrumentsOp m;
        return &m;
    }

private:
  std::shared_mutex mainInstrumentsMapLocker;
  std::map<std::string, std::string> mainInstrumentsMap;
};

#endif