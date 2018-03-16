#include "ctpSave.h"

// #include "../../impl/ctpApiMd.h"
// #include "../../impl/ctpApiTrader.h"
#include "character.h"
#include "third/jsoncpp/json.hpp"
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

		const char* name = gbk_to_utf8(item->ExchangeName);

		json r = json::object();
		r["id"] = item->ExchangeID;
		r["name"] = name;
		r["property"] = item->ExchangeProperty;
		j += r;

		free((void*)name);
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

			const char* name = gbk_to_utf8(item->InstrumentName);

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

			free((void*)name);

			j += r;
		}

		std::string content = j.dump();

		// this->saveDataFun(("futuresInstruments-" + b->first).c_str(), 0, content.c_str());
		this->saveDataFun("futuresInstruments", 0, content.c_str());
	// }
}

class JsonOp{
	private:
	  json& j;
	public:
	JsonOp(json& j2):j(j2){

	}

	// template<class _Ty>
	// const JsonOp& get(_Ty& left, const char* rightFieldKey){
	// 	left = (_Ty)j[rightFieldKey];
	// 	return *this;
	// }

	// template<class _Ty>
	JsonOp& get(int& left, const char* rightFieldKey){
		left = (int)j[rightFieldKey];
		return *this;
	}

	JsonOp& get(double& left, const char* rightFieldKey){
		left = (double)j[rightFieldKey];
		return *this;
	}

	// template<>
	JsonOp& get(char& left, const char* rightFieldKey){
		left = (unsigned char)j[rightFieldKey];
		return *this;
	}

	// template<>
	JsonOp& get(char* left, const char* rightFieldKey){
		std::string s = j[rightFieldKey];
		strcpy(left, s.c_str());
		return *this;
	}
};

std::shared_ptr<std::vector<CThostFtdcInstrumentField>> ctpSave::readInstruments(){
	std::shared_ptr<std::vector<CThostFtdcInstrumentField>> ret;
	if (!this->readDataFun)
		return ret;

	const char *content = 0;

	do{
		content = this->readDataFun("futuresInstruments", 0);

		if(!content || !*content)
			break;

		json j = json::parse(content);

		ret.reset(new std::vector<CThostFtdcInstrumentField>());
		ret->reserve(j.size());

		char name2[64];
		for (json::iterator b = j.begin(), e=j.end(); b != e; ++b) {

			JsonOp r(*b);

			CThostFtdcInstrumentField item2;
			CThostFtdcInstrumentField* item = &item2;
			memset(item, 0, sizeof(CThostFtdcInstrumentField));

			r.get(item->InstrumentID, "id");			
			r.get(name2, "name");

			const char* name = utf8_to_gbk((const char*)name2);
			strcpy(item->InstrumentName, name);

			r.get(item->ExchangeID, "exchangeID")
				.get(item->ProductID, "productID")
				.get(item->ProductClass,"productClass")
				.get(item->DeliveryYear, "deliveryYear")
				.get(item->DeliveryMonth, "deliveryMonth")
				.get(item->MaxMarketOrderVolume, "maxMarketOrderVolume")
				.get(item->MinMarketOrderVolume, "minMarketOrderVolume")
				.get(item->MaxLimitOrderVolume, "maxLimitOrderVolume")
				.get(item->MinLimitOrderVolume, "minLimitOrderVolume")
				.get(item->VolumeMultiple, "volumeMultiple")
				.get(item->PriceTick, "priceTick")
				.get(item->CreateDate, "createDate")
				.get(item->OpenDate, "openDate")
				.get(item->ExpireDate, "expireDate")
				.get(item->StartDelivDate, "startDelivDate")
				.get(item->EndDelivDate, "endDelivDate")
				.get(item->InstLifePhase, "instLifePhase")
				.get(item->IsTrading, "isTrading")
				.get(item->PositionType, "positionType")
				.get(item->PositionDateType, "positionDateType")
				.get(item->LongMarginRatio, "longMarginRatio")
				.get(item->ShortMarginRatio, "shortMarginRatio")
				.get(item->MaxMarginSideAlgorithm, "maxMarginSideAlgorithm")
				.get(item->UnderlyingInstrID, "underlyingInstrID")
				.get(item->StrikePrice, "strikePrice")
				.get(item->OptionsType, "optionsType")
				.get(item->UnderlyingMultiple, "underlyingMultiple")
				.get(item->CombinationType, "combinationType");

			free((char*)name);

			ret->push_back(item2);
		}

	} while (false);

	if(content){
		free((void*)content);
		content = 0;
	}

	return ret;
}

void ctpSave::saveInstrumentsStatus(const std::list<CThostFtdcInstrumentStatusField>& instaruments) {

	if (!this->saveDataFun || instaruments.empty())
		return;

	typedef std::map<std::string, std::list<CThostFtdcInstrumentStatusField*> > E2IMAP;
	E2IMAP exchange2Instruments;

	for (auto b = instaruments.begin(), e = instaruments.end(); b != e;++b)
	{
		CThostFtdcInstrumentStatusField* item = (CThostFtdcInstrumentStatusField*)&(*b);

		std::string exchangeId = item->ExchangeID;
		E2IMAP::iterator b2 = exchange2Instruments.find(exchangeId);
		if (b2 == exchange2Instruments.end()) {
			exchange2Instruments.insert(std::make_pair(exchangeId, std::list<CThostFtdcInstrumentStatusField*>()));
			b2 = exchange2Instruments.find(item->ExchangeID);
		}

		b2->second.push_back(item);
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