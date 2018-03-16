#if !defined(__SAVEDATA__HELPER_H__)
#define __SAVEDATA__HELPER_H__

#include <list>
#include <memory>
#include <mutex>  

template<class Ty>
class CSaveDataHelper
{
public:	
	typedef std::list<Ty> DATALIST;
	typedef void(*FUNSAVEDATA)(const DATALIST&);
private:

	DATALIST dataList;

	std::mutex dataMutex;

	FUNSAVEDATA saveChangeCallback;
  public:

	  CSaveDataHelper() {
		  saveChangeCallback = 0;
	  }

	  void addItem(Ty *pItem) {
		  dataMutex.lock();
		  dataList.push_back(typename DATALIST::value_type(*pItem));
		  dataMutex.unlock();
	  }
	  void saveAll(){
		  DATALIST dataList;

		  dataMutex.lock();
		  if (this->dataList.empty()) {
			  dataMutex.unlock();
			  return;
		  }
		  dataList.swap(this->dataList);
		  dataMutex.unlock();

		  if (!this->saveChangeCallback)
			  return;

		  (*this->saveChangeCallback)(dataList);
	  }
	  void setSaveChangeCallback(FUNSAVEDATA saveChangeCallback){
		  this->saveChangeCallback = saveChangeCallback;
	  }
};

#endif