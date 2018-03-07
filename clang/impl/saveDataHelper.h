#if !defined(__SAVEDATA__HELPER_H__)
#define __SAVEDATA__HELPER_H__

#include <list>
#include <memory>
#include <mutex>  

template<class Ty>
class CSaveDataHelper
{
  public:
	  typedef std::list<std::unique_ptr<Ty> > DATALIST;
	  typedef void(*FUNSAVEDATA)(Ty**, int);

	  CSaveDataHelper() {
		  saveChangeCallback = 0;
	  }

	  void addItem(Ty *pItem) {
		  Ty* r = new Ty();
		  *r = *pItem;

		  dataMutex.lock();
		  dataList.push_back(typename DATALIST::value_type(r));
		  dataMutex.unlock();
	  }
	  void saveAll(){
		  DATALIST dataList;

		  dataMutex.lock();
		  if (this->dataList.size() == 0) {
			  dataMutex.unlock();
			  return;
		  }
		  dataList.swap(this->dataList);
		  dataMutex.unlock();

		  if (!this->saveChangeCallback)
			  return;

		  int cnt = dataList.size();
		  Ty** datas = (Ty**)malloc(sizeof(Ty*) * cnt);

		  int i = 0;
		  for (typename DATALIST::const_iterator b = dataList.begin(), e = dataList.end(); b != e; ++b, ++i) {
			  datas[i] = b->get();
		  }

		  (*this->saveChangeCallback)(datas, cnt);
		  free(datas);
	  }
	  void setSaveChangeCallback(FUNSAVEDATA saveChangeCallback){
		  this->saveChangeCallback = saveChangeCallback;
	  }
private:

	DATALIST dataList;

	std::mutex dataMutex;

	FUNSAVEDATA saveChangeCallback;
};

#endif