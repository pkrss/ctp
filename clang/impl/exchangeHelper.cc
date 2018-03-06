#include "exchangeHelper.h"

CExchangeHelper::CExchangeHelper() {
	saveChangeCallback = 0;
}

void CExchangeHelper::addExchange(CThostFtdcExchangeField *pExchange) {
	CThostFtdcExchangeField* r = new CThostFtdcExchangeField();
	memcpy(r, pExchange, sizeof(CThostFtdcExchangeField));

	exchangeMutex.lock();
	exchangeList.push_back(EXCHANGELIST::value_type(r));
	exchangeMutex.unlock();
}

void CExchangeHelper::saveExchanges() {
	EXCHANGELIST exchangeList;

	exchangeMutex.lock();
	if (this->exchangeList.size() == 0) {
		exchangeMutex.unlock();
		return;
	}
	exchangeList.swap(this->exchangeList);
	exchangeMutex.unlock();

	if (!this->saveChangeCallback)
		return;

	int cnt = exchangeList.size();
	CThostFtdcExchangeField** exchanges = (CThostFtdcExchangeField**)malloc(sizeof(CThostFtdcExchangeField*) * cnt);

	int i = 0;
	for (EXCHANGELIST::const_iterator b = exchangeList.begin(), e = exchangeList.end(); b != e; ++b, ++i) {
		exchanges[i] = b->get();
	}
	
	(*this->saveChangeCallback)(exchanges, cnt);
	free(exchanges);
}

void CExchangeHelper::setSaveChangeCallback(FUNSAVEEXCHANGE saveChangeCallback) {
	this->saveChangeCallback = saveChangeCallback;
}
