/*
 * dbConnector.cpp
 *
 *  Created on: 29 Aug 2013
 *      Author: DTHOZBAYBURTLU
 */

#include <stdio.h>
#include "dbconnector.h"

DBConnector* DBConnector::instance = NULL;

DBConnector::DBConnector() {
	// TODO Auto-generated constructor stub
	isDbOpened = false;
	db = NULL;

}

DBConnector::~DBConnector() {
	// TODO Auto-generated destructor stub
	if (db != NULL)
		closeDb();
}

bool DBConnector::loadDb(string* file) {
	if (db != NULL)
		closeDb();

	return openDb(file);
}

bool DBConnector::query(string* query, Json::Value* result) {

	bool bRetVal = false;
	char *zErrMsg = 0;

	/* Execute SQL statement */
	if (sqlite3_exec(db, query->c_str(), &this->sqlitecb, (void*) result,
			&zErrMsg) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		printf("Operation done successfully\n");
		bRetVal = true;
	}

	return bRetVal;
}

bool DBConnector::closeDb() {
	bool bRetVal = false;
	if (isDbOpened && db!= NULL)
		bRetVal = (sqlite3_close(db) == SQLITE_OK);

	return bRetVal;
}

bool DBConnector::openDb(string* file) {
	/* Open database */
	int r = sqlite3_open(file->c_str(), &db);
	if (r == SQLITE_OK) {
		printf("Successfully opened %s \n",file->c_str());
		isDbOpened = true;
	} else {
		fprintf(stderr, "Cannot open [err:%d]: %s\n", r, file->c_str());
		isDbOpened = false;
	}

	return isDbOpened;
}

int DBConnector::sqlitecb(void *data, int argc, char **argv, char **azColName) {

	Json::Value* result = static_cast<Json::Value*>(data);
	if (result == 0){
		fprintf(stderr, "Data type error\n");
		return -1;
	}

	Json::Value jValue;
	for (int i = 0; i < argc; i++) {
		jValue[azColName[i]] = argv[i] ? argv[i] : "NULL";
	}

	(*result)["results"].append(jValue);

	return 0;
}

DBConnector* DBConnector::getInstance() {
	if (instance == NULL)
		instance = new DBConnector();
	return instance;
}

