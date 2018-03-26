#include "ctpSave.h"

#include "hqMd.h"
#include "hqTrader.h"

#include <list>

#include "../../third/jsoncpp/json.hpp"
#include "../../character.h"

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

			char* name = (char*)gbk_to_utf8(item->InstrumentName);

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

template<class _Ty>
void json_2_v(_Ty& s, const json& j){
	*(&s) = j;
}
void json_2_s(char* s, const json& j){ strcpy(s, ((std::string)j).c_str()); }
template<>
void json_2_v(TThostFtdcTraderIDType& s, const json& j){ json_2_s((char*)&s[0], j); }
template<>
void json_2_v(TThostFtdcInvestorIDType& s, const json& j){ json_2_s((char*)&s[0], j); }
template<>
void json_2_v(TThostFtdcBrokerIDType& s, const json& j){ json_2_s((char*)&s[0], j); }
template<>
void json_2_v(TThostFtdcBrokerAbbrType& s, const json& j){ json_2_s((char*)&s[0], j); }
template<>
void json_2_v(TThostFtdcExchangeInstIDType& s, const json& j){ json_2_s((char*)&s[0], j); }
template<>
void json_2_v(float& s, const json& j){ *(&s) = j; }
void json_2_v(double& s, const json& j){ *(&s) = j; }
void json_2_v(long& s, const json& j){ *(&s) = j; }
void json_2_v(unsigned char& s, const json& j){ *(&s) = (unsigned char)j; }
void json_2_v(char& s, const json& j){ *(&s) = (char)(unsigned char)j; }

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

		CThostFtdcInstrumentField item2;
		for (json::iterator b = j.begin(), e=j.end(); b != e; ++b) {
			json& r = *b;

			CThostFtdcInstrumentField *item = &item2;
			memset(item, 0, sizeof(CThostFtdcInstrumentField));

			json_2_v(item->InstrumentID, r["id"]);

			char* name = (char*)utf8_to_gbk(((std::string)r["name"]).c_str());
			json_2_v(item->InstrumentName, name);
			json_2_v(item->ExchangeID, r["exchangeID"]);
			json_2_v(item->ProductID, r["productID"]);
			json_2_v(item->ProductClass, r["productClass"]);
			json_2_v(item->DeliveryYear, r["deliveryYear"]);
			json_2_v(item->DeliveryMonth, r["deliveryMonth"]);
			json_2_v(item->MaxMarketOrderVolume, r["maxMarketOrderVolume"]);
			json_2_v(item->MinMarketOrderVolume, r["minMarketOrderVolume"]);
			json_2_v(item->MaxLimitOrderVolume, r["maxLimitOrderVolume"]);
			json_2_v(item->MinLimitOrderVolume, r["minLimitOrderVolume"]);
			json_2_v(item->VolumeMultiple, r["volumeMultiple"]);
			json_2_v(item->PriceTick, r["priceTick"]);
			json_2_v(item->CreateDate, r["createDate"]);
			json_2_v(item->OpenDate, r["openDate"]);
			json_2_v(item->ExpireDate, r["expireDate"]);
			json_2_v(item->StartDelivDate, r["startDelivDate"]);
			json_2_v(item->EndDelivDate, r["endDelivDate"]);
			json_2_v(item->InstLifePhase, r["instLifePhase"]);
			json_2_v(item->IsTrading, r["isTrading"]);
			json_2_v(item->PositionType, r["positionType"]);
			json_2_v(item->PositionDateType, r["positionDateType"]);
			json_2_v(item->LongMarginRatio, r["longMarginRatio"]);
			json_2_v(item->ShortMarginRatio, r["shortMarginRatio"]);
			json_2_v(item->MaxMarginSideAlgorithm, r["maxMarginSideAlgorithm"]);
			json_2_v(item->UnderlyingInstrID, r["underlyingInstrID"]);
			json_2_v(item->StrikePrice, r["strikePrice"]);
			json_2_v(item->OptionsType, r["optionsType"]);
			json_2_v(item->UnderlyingMultiple, r["underlyingMultiple"]);
			json_2_v(item->CombinationType, r["combinationType"]);

			free(name);

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