#include "ctpSave.h"

#include "../../impl/ctpApiMd.h"
#include "../../impl/ctpApiTrader.h"

#include <list>

#include "../../third/jsoncpp/json.hpp"
// for convenience
using json = nlohmann::json;

void ctpSave::SetSaveDataFun(SaveDataFun saveDataFun) {
	this->saveDataFun = saveDataFun;
}

void ctpSave::saveExchanges(CThostFtdcExchangeField** exchanges, int count) {

	if (!this->saveDataFun || !exchanges || !count)
		return;

	json j = json::array();
	for (int i = 0; i < count; ++i) {
		CThostFtdcExchangeField* item = exchanges[i];

		char* name = ExtGbkToUtf8(item->ExchangeName);

		json r = json::object();
		r["id"] = item->ExchangeID;
		r["name"] = name;
		r["property"] = item->ExchangeProperty;
		j += r;

		free(name);
	}
	std::string content = j.dump();

	this->saveDataFun("futuresExchanges", 0, content.c_str());
}

void ctpSave::saveInstruments(CThostFtdcInstrumentField** instaruments, int count) {

	if (!this->saveDataFun || !instaruments || !count)
		return;

	typedef std::map<std::string, std::list<CThostFtdcInstrumentField*> > E2IMAP;
	E2IMAP exchange2Instruments;

	for (int i = 0; i < count; ++i) {
		CThostFtdcInstrumentField* item = instaruments[i];

		std::string exchangeId = item->ExchangeID;
		E2IMAP::iterator b = exchange2Instruments.find(exchangeId);
		if (b == exchange2Instruments.end()) {
			exchange2Instruments.insert(std::make_pair(exchangeId, std::list<CThostFtdcInstrumentField*>()));
			b = exchange2Instruments.find(item->ExchangeID);
		}

		b->second.push_back(item);
	}

	for (E2IMAP::const_iterator b = exchange2Instruments.begin(), e = exchange2Instruments.end(); b != e; ++b) {

		json j = json::array();

		for (std::list<CThostFtdcInstrumentField*>::const_iterator b2 = b->second.begin(), e2 = b->second.end(); b2 != e2; ++b2) {
			CThostFtdcInstrumentField* item = *b2;


			char* name = ExtGbkToUtf8(item->InstrumentName);

			json r = json::object();
			r["id"] = item->InstrumentID;
			r["name"] = name;
			r["exchangeID"] = item->ExchangeID;
			r["productID"] = item->ProductID;
			r["productClass"] = item->ProductClass;
			r["deliveryYear"] = item->DeliveryYear;
			r["deliveryMonth"] = item->DeliveryMonth;
			r["maxMarketOrderVolume"] = item->MaxMarketOrderVolume;
			r["minMarketOrderVolume"] = item->MinMarketOrderVolume;
			r["maxLimitOrderVolume"] = item->MaxLimitOrderVolume;
			r["minLimitOrderVolume"] = item->MinLimitOrderVolume;
			r["volumeMultiple"] = item->VolumeMultiple;
			r["priceTick"] = item->PriceTick;
			r["createDate"] = item->CreateDate;
			r["openDate"] = item->OpenDate;
			r["expireDate"] = item->ExpireDate;
			r["startDelivDate"] = item->StartDelivDate;
			r["endDelivDate"] = item->EndDelivDate;
			r["instLifePhase"] = item->InstLifePhase;
			r["isTrading"] = item->IsTrading;
			r["positionType"] = item->PositionType;
			r["positionDateType"] = item->PositionDateType;
			r["longMarginRatio"] = item->LongMarginRatio;
			r["shortMarginRatio"] = item->ShortMarginRatio;
			r["maxMarginSideAlgorithm"] = item->MaxMarginSideAlgorithm;
			r["underlyingInstrID"] = item->UnderlyingInstrID;
			r["strikePrice"] = item->StrikePrice;
			r["optionsType"] = item->OptionsType;
			r["underlyingMultiple"] = item->UnderlyingMultiple;
			r["combinationType"] = item->CombinationType;

			free(name);

			j += r;
		}

		std::string content = j.dump();

		this->saveDataFun(("futuresInstruments-" + b->first).c_str(), 0, content.c_str());
	}
}


void ctpSave::saveInstrumentsStatus(CThostFtdcInstrumentStatusField** instarumentsStatus, int count) {

	if (!this->saveDataFun || !instarumentsStatus || !count)
		return;

	typedef std::map<std::string, std::list<CThostFtdcInstrumentStatusField*> > E2IMAP;
	E2IMAP exchange2Instruments;

	for (int i = 0; i < count; ++i) {
		CThostFtdcInstrumentStatusField* item = instarumentsStatus[i];

		std::string exchangeId = item->ExchangeID;
		E2IMAP::iterator b = exchange2Instruments.find(exchangeId);
		if (b == exchange2Instruments.end()) {
			exchange2Instruments.insert(std::make_pair(exchangeId, std::list<CThostFtdcInstrumentStatusField*>()));
			b = exchange2Instruments.find(item->ExchangeID);
		}

		b->second.push_back(item);
	}

	for (E2IMAP::const_iterator b = exchange2Instruments.begin(), e = exchange2Instruments.end(); b != e; ++b) {

		json j = json::array();

		for (std::list<CThostFtdcInstrumentStatusField*>::const_iterator b2 = b->second.begin(), e2 = b->second.end(); b2 != e2; ++b2) {
			CThostFtdcInstrumentStatusField* item = *b2;
			
			json r = json::object();
			r["id"] = item->InstrumentID;
			r["status"] = item->InstrumentStatus;
			r["exchangeID"] = item->ExchangeID;
			r["exchangeInstID"] = item->ExchangeInstID;
			r["settlementGroupID"] = item->SettlementGroupID;
			r["tradingSegmentSN"] = item->TradingSegmentSN;
			r["enterTime"] = item->EnterTime;
			r["enterReason"] = item->EnterReason;

			j += r;
		}

		std::string content = j.dump();

		this->saveDataFun(("futuresInstrumentsStatus-" + b->first).c_str(), 0, content.c_str());
	}
}