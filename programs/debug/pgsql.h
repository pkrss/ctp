#ifndef __MY_PGSQL__H__
#define __MY_PGSQL__H__


typedef struct pg_conn PGconn;
typedef struct pg_result PGresult;

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <mutex>

class PgSql {
public:
	typedef std::vector<std::map<std::string, std::string> > RESULT;
private:
	const char* conninfo;
	PGconn* conn;
	volatile int seqId;
	std::mutex dbMutex;

	PgSql();

	int connectDb();

	std::unique_ptr<RESULT> getResultAndCloseConnect(PGresult* res);
	long getLongResultAndCloseConnect(PGresult* res);
	long getLongResult(const std::unique_ptr<PgSql::RESULT>& rst);
public:

	static PgSql* getInstance();
	static void Destroy();

	int init();
	void destroy();

	void saveData(const char* name, long id, const char* content);
	
	bool checkConn();
	bool simpleDataExist(const char* type, const char* id);

	int insert(const char* sql, const char*** sqlParams = 0, int sqlParamsRows = 0, int sqlParamsCount = 0);
	
	std::unique_ptr<RESULT> execSql(const char* sql, const char** sqlParams = 0, int sqlParamsCount = 0);
};

#endif