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

void ctpSave::SetReadDataFun(ReadDataFun readDataFun) {
	this->readDataFun = readDataFun;
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

void ctpSave::saveInstruments(const std::list<CThostFtdcInstrumentField>& instaruments) {

	if (!this->saveDataFun || instaruments.empty())
		return;

	// typedef std::map<std::string, std::list<CThostFtdcInstrumentField*> > E2IMAP;
	// E2IMAP exchange2Instruments;

	// for (int i = 0; i < count; ++i) {
	// 	CThostFtdcInstrumentField* item = instaruments[i];

	// 	std::string exchangeId = item->ExchangeID;
	// 	E2IMAP::iterator b = exchange2Instruments.find(exchangeId);
	// 	if (b == exchange2Instruments.end()) {
	// 		exchange2Instruments.insert(std::make_pair(exchangeId, std::list<CThostFtdcInstrumentField*>()));
	// 		b = exchange2Instruments.find(item->ExchangeID);
	// 	}

	// 	b->second.push_back(item);
	// }

	// for (E2IMAP::const_iterator b = exchange2Instruments.begin(), e = exchange2Instruments.end(); b != e; ++b) {

		json j = json::array();

	// 	for (std::list<CThostFtdcInstrumentField*>::const_iterator b2 = b->second.begin(), e2 = b->second.end(); b2 != e2; ++b2) {
	// 		CThostFtdcInstrumentField* item = *b2;

		for (auto b = instaruments.begin(), e = instaruments.end(); b != e;++b)
		{
			const CThostFtdcInstrumentField* item = &(*b);

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

		// this->saveDataFun(("futuresInstruments-" + b->first).c_str(), 0, content.c_str());
		this->saveDataFun("futuresInstruments", 0, content.c_str());
	// }
}

std::shared_ptr<std::vector<CThostFtdcInstrumentField>> ctpSave::readInstruments(){
	std::shared_ptr<std::vector<CThostFtdcInstrumentField>> ret;
	if (!this->readDataFun || !outCount)
		return ret;

	const char *content = 0;

	int count = 0;

	do{
		content = this->readDataFun("futuresInstruments", 0);

		if(!content || !*content)
			break;

		json j = json::parse(content);

		ret.reset(new std::vector<CThostFtdcInstrumentField>());
		ret->reserve(j.count());

		for (json::iterator b = j.begin(), e=j.end(); b != e; ++b) {
			json& r = *j;

			CThostFtdcInstrumentField *item = malloc(sizeof(CThostFtdcInstrumentField));
			memset(item, 0, sizeof(CThostFtdcInstrumentField));

			strcpy(item->InstrumentID, r["id"]);

			char* name = utf8_to_gbk(r["name"]);
			strcpy(item->InstrumentName, name);
			strcpy(item->ExchangeID, r["exchangeID"]);
			strcpy(item->ProductID, r["productID"]);
			item->ProductClass = r["productClass"];
			item->DeliveryYear = r["deliveryYear"];
			item->DeliveryMonth = r["deliveryMonth"];
			item->MaxMarketOrderVolume = r["maxMarketOrderVolume"];
			item->MinMarketOrderVolume = r["minMarketOrderVolume"];
			item->MaxLimitOrderVolume = r["maxLimitOrderVolume"];
			item->MinLimitOrderVolume = r["minLimitOrderVolume"];
			item->VolumeMultiple = r["volumeMultiple"];
			item->PriceTick = r["priceTick"];
			strcpy(item->CreateDate, r["createDate"]);
			strcpy(item->OpenDate, r["openDate"]);
			strcpy(item->ExpireDate, r["expireDate"]);
			strcpy(item->StartDelivDate, r["startDelivDate"]);
			strcpy(item->EndDelivDate, r["endDelivDate"]);
			item->InstLifePhase = r["instLifePhase"];
			item->IsTrading = r["isTrading"];
			item->PositionType = r["positionType"];
			item->PositionDateType = r["positionDateType"];
			item->LongMarginRatio = r["longMarginRatio"];
			item->ShortMarginRatio = r["shortMarginRatio"];
			item->MaxMarginSideAlgorithm = r["maxMarginSideAlgorithm"];
			item->UnderlyingInstrID = r["underlyingInstrID"];
			item->StrikePrice = r["strikePrice"];
			item->OptionsType = r["optionsType"];
			item->UnderlyingMultiple = r["underlyingMultiple"];
			item->CombinationType = r["combinationType"];

			free(name);

			ret->push_back(item);
		}

	} while (false);

	if(content){
		free(content);
		content = 0;
	}

	return ret;
}

void ctpSave::saveInstrumentsStatus(const std::list<CThostFtdcInstrumentField>& instaruments) {

	if (!this->saveDataFun || instaruments.empty())
		return;

	typedef std::map<std::string, std::list<CThostFtdcInstrumentStatusField*> > E2IMAP;
	E2IMAP exchange2Instruments;

	for (auto b = instaruments.begin(), e = instaruments.end(); b != e;++b)
	{
		CThostFtdcInstrumentStatusField* item = &(*b);

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