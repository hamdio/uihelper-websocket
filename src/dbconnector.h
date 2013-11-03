/*
 * dbConnector.h
 *
 *  Created on: 29 Aug 2013
 *      Author: DTHOZBAYBURTLU
 */

#ifndef DBCONNECTOR_H_
#define DBCONNECTOR_H_

#include <string>
#include <json/json.h>

#include "sqlite3.h"

using namespace std;

class DBConnector {

	//singleton instance
	static DBConnector* instance;
	DBConnector();

	//internal attributes
	bool isDbOpened;
	sqlite3 *db;

	//internal methods
	bool openDb(string* file);
	bool closeDb();

	//sqlite cb for select
	static int sqlitecb(void *data, int argc, char **argv, char **azColName);

public:
	static DBConnector* getInstance();

	bool loadDb(string* file);
	bool query(string* query, Json::Value* result);

	virtual ~DBConnector();
};

#endif /* DBCONNECTOR_H_ */
