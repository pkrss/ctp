
#include "pgsql.h"
#include "Profile.h"

#include <string>

#include <stdio.h>  
#include <stdlib.h>  
#include "../../third/libpq/h/libpq-fe.h" 


static PgSql* pgsql = 0;

PgSql* PgSql::getInstance() {
	if (!pgsql)
		pgsql = new PgSql();
	return pgsql;
}

PgSql::PgSql() {
	conninfo = 0;
	conn = 0;
	seqId = 0;
}

void PgSql::Destroy() {
	if (!pgsql)
		return;

	pgsql->destroy();

	delete pgsql;
	pgsql = 0;
}

int PgSql::init() {
	conninfo = Profile::getInstance()->getStringCache("db.conn");

	return connectDb();
}

int PgSql::connectDb() {
	if (!conninfo) {
		fprintf(stderr, "Connection to database failed: conninfo is null!\n");
		return -2;
	}

	printf("Begin connection to database\n");

	/* Make a connection to the database */
	conn = PQconnectdb(conninfo);

	/* Check to see that the backend connection was successfully made */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		destroy();
		return -1;
	}

	printf("End connection to database: ok!\n");
	return 0;
}

void PgSql::destroy()
{
	if (conn) {
		PQfinish((PGconn*)conn);
		conn = 0;
	}

	//exit(1);
}


void PgSql::saveData(const char* name, long id, const char* content) {
	
	if (!checkConn()) {
		printf("ERROR: saveData can not run, because db is not connected!\n");
		return;
	}

	printf("Prepare save data name=%s id=%ld ...\n", name, id);

	char sid[32];
	ltoa(id, sid, 10);

	const char** sqlParams = (const char**)malloc(sizeof(const char*) * 3);
	sqlParams[0] = name;
	sqlParams[1] = sid;

	execSql("DELETE FROM myzc_string_record WHERE key_type=$1 AND key_id=$2", sqlParams, 2);

	sqlParams[2] = content;
	execSql("INSERT INTO myzc_string_record (key_type, key_id, content) VALUES($1, $2, $3)", sqlParams, 3);

	printf("End save data name=%s id=%ld ...\n", name, id);

	free(sqlParams);
}

bool PgSql::checkConn() {

	std::lock_guard<std::mutex> lock(dbMutex);

	if (!conn || (PQstatus(conn) != CONNECTION_OK))
	{
		printf("checkConn failed\n");
		if (0 != connectDb())
			return false;
	}

	return true;
}

bool PgSql::simpleDataExist(const char* type, const char* id) {
	if (!id)
		id = "";

	const char* sql = "SELECT COUNT(*) FROM myzc_string_record WHERE key_type=:$1 AND key_id=:$2";
	const char** val = (const char**)malloc(sizeof(const char*) * 2);
	val[0] = type;
	val[1] = id;
	std::unique_ptr<RESULT> res = execSql(sql, val, 2);
	free(val);
	return getLongResult(res);
}


std::unique_ptr<PgSql::RESULT> PgSql::getResultAndCloseConnect(PGresult* res) {
	RESULT* ret = 0;

	do {
		if (!res)
			break;

		int nFields = PQnfields(res);
		if (nFields == 0)
			break;

		std::vector<std::string> fieldNames;
		fieldNames.resize(nFields);

		for (int i = 0; i < nFields; ++i)
			fieldNames[i] = PQfname(res, i);

		int rows = PQntuples(res);
		if (rows == 0)
			break;

		ret = new RESULT();
		std::map<std::string, std::string> rowVals;
		for (int i = 0; i < rows; ++i) {
			rowVals.clear();
			for (int j = 0; j < nFields; ++j) {
				char* val = PQgetvalue(res, 0, 0);
				rowVals.insert(std::map<std::string, std::string>::value_type(fieldNames[j], std::string(val)));
			}
			ret->push_back(rowVals);
		}
	} while (false);

	if (res != 0) {
		PQclear(res);
		res = 0;
	}

	return std::unique_ptr<PgSql::RESULT>(ret);
}

long PgSql::getLongResultAndCloseConnect(PGresult* res) {
	if (!res)
		return -2;

	return getLongResult(getResultAndCloseConnect(res));
}

long PgSql::getLongResult(const std::unique_ptr<PgSql::RESULT>& rst) {
	if (!rst.get() || rst.get()->empty() || rst.get()->front().empty())
		return -2;

	std::string s = rst.get()->front().begin()->second;
	return atol(s.c_str());
}

int PgSql::insert(const char* sql, const char*** sqlParams, int sqlParamsRows, int sqlParamsCount) {
	PGresult    *res = 0;

	bool dbOk = checkConn();

	dbMutex.lock();

	do {
		if (!dbOk)
			break;

		if (sqlParamsRows > 0 && sqlParamsCount > 0) {
			std::string stmtName = "stmt";
			stmtName.resize(32);
			sprintf((char*)stmtName.c_str(), "stmt_%d", ++seqId);

			res = PQprepare(conn, stmtName.c_str(), sql, sqlParamsCount, 0);
			if (PQstatus(conn) != CONNECTION_OK)
			{
				fprintf(stderr, "PQprepare [%s] failed: %s/n", sql, PQresultErrorMessage(res));
				break;
			}

			for (int i = 0; i < sqlParamsRows; ++i) {
				res = PQexecPrepared(conn, stmtName.c_str(), sqlParamsCount, sqlParams[i], 0, 0, 0);
				if (PQstatus(conn) != CONNECTION_OK)
				{
					fprintf(stderr, "PQexecPrepared [%s] failed: %s/n", sql, PQresultErrorMessage(res));
					// break;
				}
			}
		}
		else {
			res = PQexec(conn, "sql");
			if (PQstatus(conn) != CONNECTION_OK)
			{
				fprintf(stderr, "PQexec [%s] failed: %s/n", sql, PQresultErrorMessage(res));
				break;
			}
		}

		sql = "SELECT LASTVAL()";
		res = PQexec(conn, sql);
		if (PQstatus(conn) != CONNECTION_OK)
		{
			fprintf(stderr, "PQexec [%s] failed: %s/n", sql, PQresultErrorMessage(res));
			break;
		}
	} while (false);

	dbMutex.unlock();

	return getLongResultAndCloseConnect(res);
}

std::unique_ptr<PgSql::RESULT> PgSql::execSql(const char* sql, const char** sqlParams, int sqlParamsCount){
	PGresult    *res = 0;

	bool dbOk = checkConn();

	dbMutex.lock();

	do {
		if (!dbOk)
			break;

		if (sqlParamsCount > 0)
			res = PQexecParams(conn, sql, sqlParamsCount, 0, sqlParams, 0, 0, 0);
		else
			res = PQexec(conn, sql);
	} while (false);

	if (PQstatus(conn) != CONNECTION_OK)
	{
		fprintf(stderr, "PQexec [%s] failed: %s/n", sql, PQresultErrorMessage(res));
	}

	dbMutex.unlock();

	return getResultAndCloseConnect(res);
}

//
//int pq_main(int argc, char **argv)
//{
//	const char  *conninfo;
//	PGconn      *conn;
//	PGresult    *res;
//	int     nFields;
//	int     i,
//		j;
//
//	/*
//	* If the user supplies a parameter on the command line, use it as the
//	* conninfo string; otherwise default to setting dbname=postgres and using
//	* environment variables or defaults for all other connection parameters.
//	*/
//	if (argc > 1)
//		conninfo = argv[1];
//	else
//		conninfo = "dbname = postgres";
//
//	/* Make a connection to the database */
//	conn = PQconnectdb(conninfo);
//
//	/* Check to see that the backend connection was successfully made */
//	if (PQstatus(conn) != CONNECTION_OK)
//	{
//		fprintf(stderr, "Connection to database failed: %s",
//			PQerrorMessage(conn));
//		exit_nicely(conn);
//	}
//
//	/*
//	* Our test case here involves using a cursor, for which we must be inside
//	* a transaction block. We could do the whole thing with a single
//	* PQexec() of "select * from pg_database", but that's too trivial to make
//	* a good example.
//	*/
//
//	/* Start a transaction block */
//	res = PQexec(conn, "BEGIN");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK)
//	{
//		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
//		PQclear(res);
//		exit_nicely(conn);
//	}
//
//	/*
//	* Should PQclear PGresult whenever it is no longer needed to avoid memory
//	* leaks
//	*/
//	PQclear(res);
//
//	/*
//	* Fetch rows from pg_database, the system catalog of databases
//	*/
//	res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
//	if (PQresultStatus(res) != PGRES_COMMAND_OK)
//	{
//		fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
//		PQclear(res);
//		exit_nicely(conn);
//	}
//	PQclear(res);
//
//	res = PQexec(conn, "FETCH ALL in myportal");
//	if (PQresultStatus(res) != PGRES_TUPLES_OK)
//	{
//		fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
//		PQclear(res);
//		exit_nicely(conn);
//	}
//
//	/* first, print out the attribute names */
//	nFields = PQnfields(res);
//	for (i = 0; i < nFields; i++)
//		printf("%-15s", PQfname(res, i));
//	printf("\n");
//
//	/* next, print out the rows */
//	for (i = 0; i < PQntuples(res); i++)
//	{
//		for (j = 0; j < nFields; j++)
//			printf("%-15s", PQgetvalue(res, i, j));
//		printf("\n");
//	}
//
//	PQclear(res);
//
//	/* close the portal ... we don't bother to check for errors ... */
//	res = PQexec(conn, "CLOSE myportal");
//	PQclear(res);
//
//	/* end the transaction */
//	res = PQexec(conn, "END");
//	PQclear(res);
//
//	/* close the connection to the database and cleanup */
//	PQfinish(conn);
//
//	return 0;
//}