#if !defined(__PROFILE_H__)
#define __PROFILE_H__

#include <map>

class Profile{
private:
    Profile();
	~Profile();
private:
	void* pJson;
	typedef std::map<std::string, std::string> M2M;
	M2M m2m;
public:
    static Profile* getInstance();
	static void Destroy();

    void init(const char* profile);

	std::string getString(const char* key);

	const char* getStringCache(const char* key);
};

#endif