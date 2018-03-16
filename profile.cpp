#include "profile.h"

#include "third/jsoncpp/json.hpp"
#include "utils.h"
#include <fstream>

// for convenience
using json = nlohmann::json;

static Profile* instance = 0;

Profile::Profile() {
	pJson = 0;
}
Profile::~Profile() {
	if (pJson) {
		delete (json *)pJson;
		pJson = 0;
	}
}

void Profile::Destroy() {
	if (!instance)
		return;
	delete instance;
	instance = 0;
}


Profile* Profile::getInstance(){
	if (!instance)
		instance = new Profile();
	return instance;
}

void Profile::init(const char* profile) {
	std::ifstream i(profile);
	json * j = new json();
	i >> *j;
	this->pJson = (void*)j;
}

void ReplaceAll(std::string& strSource, const std::string& strOld, const std::string& strNew)
{
	std::size_t nPos = 0;
	while ((nPos = strSource.find(strOld, nPos)) != strSource.npos)
	{
		strSource.replace(nPos, strOld.length(), strNew);
		nPos += strNew.length();
	}
}

//std::vector<std::string> split(const std::string &text, char sep) {
//	std::vector<std::string> tokens;
//	std::size_t start = 0, end = 0;
//	while ((end = text.find(sep, start)) != std::string::npos) {
//		tokens.push_back(text.substr(start, end - start));
//		start = end + 1;
//	}
//	tokens.push_back(text.substr(start));
//	return tokens;
//}

std::string Profile::getString(const char* key){

	char* s = getenv(key);
	if (s && *s) {
		return s;
	}

	// std::vector<std::string> tokens = split(key, '.');
	std::list<std::string> tokens = SplitString(key, ".");

	json* j = ((json *)pJson);
	for (std::list<std::string>::iterator b = tokens.begin(), e = tokens.end(); b != e; ++b) {
		j = &(*j)[*b];
		if (j->is_null())
			return "";
	}

	return *j;

	//if (s.find(".") >= 0) {
	//	s = "/" + s;
	//	ReplaceAll(s, ".", "/");
	//}
	//return (*(json *)pJson)[s.c_str()];
}

const char* Profile::getStringCache(const char* key,const char* defaultValue) {
	M2M::const_iterator f = m2m.find(key);
	if (f == m2m.end()) {
		m2m[key] = getString(key);
		f = m2m.find(key);
	}
	
	if (f != m2m.end()) {
		return (*f).second.c_str();
	}

	return defaultValue;
}