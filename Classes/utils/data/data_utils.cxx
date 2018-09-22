// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "data_utils.hxx"
#include "xxhash/xxhash.h"

void DataUtils::initDatabase() {
  // Get database instance.
  const auto db = getDatabase();

  // Creating the main sqlite statement to create the tables if they haven't been created yet
  sqlite3_stmt* stmt = nullptr;
  vector<const char*> statements = {__DI_SQLC_OTHER_DATA__, __DI_SQLC_SCORE_DATA__, __DI_SQLC_STORE_DATA__,
                                    __DI_SQLC_TRIAL_DATA__};

  // Run each create statement..
  for (const auto& statement : statements)
    if (sqlite3_prepare_v2(db, statement, -1, &stmt, nullptr) == SQLITE_OK)
      if (sqlite3_step(stmt) != SQLITE_DONE) __CCLOGWITHFUNCTION("Error trying to create table...");

  // Finalize the transactions, Close the database
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

void DataUtils::saveOtherData(const string& key, const int value) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this
  // way to make it more readable
  stringstream fmt;
  fmt << "INSERT OR REPLACE INTO " << __DI_SQLT_OTHER_DATA__ << "(key, value_int) VALUES(?, ?)";
  const auto statement = fmt.str();

  // Attempt to insert data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind all of the necessary variables
    sqlite3_bind_int(stmt, 1, generateHashKey(key));
    sqlite3_bind_int(stmt, 2, value);
    if (sqlite3_step(stmt) != SQLITE_DONE)
      __CCLOGWITHFUNCTION("Failed while saving data... STATEMENT: %s", sqlite3_errmsg(db));
  }

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

void DataUtils::saveOtherData(const string& key, const string value) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this
  // way to make it more readable
  stringstream fmt;
  fmt << "INSERT OR REPLACE INTO " << __DI_SQLT_OTHER_DATA__ << "(key, value_string) VALUES(?, ?)";

  // Attempt to insert data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind all of the necessary variables
    sqlite3_bind_int(stmt, 1, generateHashKey(key));
    sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE)
      __CCLOGWITHFUNCTION("Error... Attempting to save data... STATEMENT: %s", sqlite3_errmsg(db));
  }

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

int DataUtils::getOtherData(const string& key, const int default_value) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this
  // way to make it more readable
  stringstream fmt;
  fmt << "SELECT value_int FROM " << __DI_SQLT_OTHER_DATA__ << " WHERE key=?";

  // select data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind hash key to query
    sqlite3_bind_int(stmt, 1, generateHashKey(key));

    // If the key returns a row, the data is good
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      const auto result = sqlite3_column_int(stmt, 0);

      // Reset, finalize and close db
      sqlite3_reset(stmt);
      sqlite3_finalize(stmt);
      sqlite3_close(db);

      // Return result
      return result;
    }
    __CCLOGWITHFUNCTION("Error... Key Does not exist: %s", key.c_str());
  } else
    __CCLOGWITHFUNCTION("Error... : %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  // check the return value and cipher if it's not the default;
  return default_value;
}

string DataUtils::getOtherData(const string& key, const string& default_value) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this
  // way to make it more readable
  stringstream fmt;
  fmt << "SELECT value_string FROM " << __DI_SQLT_OTHER_DATA__ << " WHERE key=?";

  // select data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind hash key to query
    sqlite3_bind_int(stmt, 1, generateHashKey(key));

    // If the key returns a row, the data is good
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      auto result = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));

      // Reset, finalize and close db
      sqlite3_reset(stmt);
      sqlite3_finalize(stmt);
      sqlite3_close(db);

      // Return result
      return result;
    }
    __CCLOGWITHFUNCTION("Error... Key Does not exist: %s", key.c_str());
  } else
    __CCLOGWITHFUNCTION("Error... : %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  // check the return value and cipher if it's not the default;
  return default_value;
}

void DataUtils::saveScoreData(ValueMap values) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this way to make it more readable
  stringstream fmt;
  fmt << "INSERT OR REPLACE INTO " << __DI_SQLT_SCORE_DATA__;
  fmt << "(level, trial, score, stars, lluma, color, music, pattern) VALUES(?, ?, ?, ?, ?, ?, ?, ?)";

  // Attempt to insert data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind all of the necessary variables
    sqlite3_bind_int(stmt, 1, values[__LEVEL__].asInt());
    sqlite3_bind_int(stmt, 2, values[__TRIAL__].asInt());
    sqlite3_bind_int(stmt, 3, values[__SCORE__].asInt());
    sqlite3_bind_int(stmt, 4, values[__STARS__].asInt());
    sqlite3_bind_text(stmt, 5, values[__LLUMA__].asString().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, values[__COLOR__].asString().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, values[__MUSIC__].asString().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, values[__PATTERN__].asString().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
      __CCLOGWITHFUNCTION("Error... Attempting to save data... STATEMENT: %s", sqlite3_errmsg(db));
  } else
    __CCLOGWITHFUNCTION("Error... STATEMENT: %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

void DataUtils::saveTrialData(ValueMap values) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this way to make it more readable
  stringstream fmt;
  fmt << "INSERT OR REPLACE INTO " << __DI_SQLT_TRIAL_DATA__
      << "(trial, stars, unlocked, last) VALUES(?, ?, ?, ?)";

  // Attempt to insert data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind all of the necessary variables
    sqlite3_bind_int(stmt, 1, values[__TRIAL__].asInt());
    sqlite3_bind_int(stmt, 2, values[__STARS__].asInt());
    sqlite3_bind_int(stmt, 3, values[__UNLOCKED__].asInt());
    sqlite3_bind_int(stmt, 4, values[__LAST__].asInt());

    if (sqlite3_step(stmt) != SQLITE_DONE)
      __CCLOGWITHFUNCTION("Error... Attempting to save data... STATEMENT: %s", sqlite3_errmsg(db));
  } else
    __CCLOGWITHFUNCTION("Error... STATEMENT: %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

void DataUtils::saveStoreData(const string& item) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this
  // way to make it more readable
  stringstream fmt;
  fmt << "INSERT OR REPLACE INTO " << __DI_SQLT_STORE_DATA__ << "(item) VALUES(?)";

  // Attempt to insert data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind all of the necessary variables
    sqlite3_bind_text(stmt, 1, item.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE)
      __CCLOGWITHFUNCTION("Error... Attempting to save data... STATEMENT: %s", sqlite3_errmsg(db));
  }

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

string DataUtils::getStoreData(const string& item, const string& default_value) {
  // Get database instance.
  const auto db = getDatabase();

  // Create the local statement that will be used to insert data. Doing it this way to make it more readable
  stringstream fmt;
  fmt << "SELECT * FROM " << __DI_SQLT_STORE_DATA__ << " WHERE item=?";

  // select data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    // Bind hash key to query
    sqlite3_bind_text(stmt, 1, item.c_str(), -1, SQLITE_STATIC);

    // If the key returns a row, the data is good
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      // Generate all the ciphers for the player and return the data;
      const auto result = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));

      // Reset, finalize and close db
      sqlite3_reset(stmt);
      sqlite3_finalize(stmt);
      sqlite3_close(db);

      // Return result
      return result;
    }

    __CCLOGWITHFUNCTION("Error... Key Does not exist: %s", item.c_str());
  } else
    __CCLOGWITHFUNCTION("Error... : %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  // check the return value and cipher if it's not the default;
  return default_value;
}

vector<string> DataUtils::getAllStoreData() {
  // Get database instance.
  const auto db = getDatabase();
  vector<string> data;

  // Create the local statement that will be used to insert data. Doing it this way to make it more readable
  stringstream fmt;
  fmt << "SELECT * FROM " << __DI_SQLT_STORE_DATA__;

  // select data
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, fmt.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // Generate all the ciphers for the player and return the data;
      data.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
  } else
    __CCLOGWITHFUNCTION("Error... : %s", sqlite3_errmsg(db));

  // Reset, finalize and close db
  sqlite3_reset(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return data;
}

sqlite3* DataUtils::getDatabase() {
  // Use FileUtils to get the path of the database
  const auto& path = FileUtils::getInstance()->getWritablePath() + __DI_DB_NAME__;
  sqlite3* db;

  // Attempt to open this database on the device
  if (sqlite3_open(path.c_str(), &db) == SQLITE_OK) {
    sqlite3_key(db, to_string(getDatabaseKey()).c_str(), int(strlen(to_string(getDatabaseKey()).c_str())));
    return db;
  }

  // This will only happen if the user has somehow tampered with the database.
  throw runtime_error("Error: Cannot open or create database...");
}

int DataUtils::getDatabaseKey() {
  const string part1 = "a728df263ab5d6fdb3d8";
  const string part2 = "c6730c6eafafe58839bb";
  const string part3 = "054c4fba1cfd2b6df083";
  const string part4 = "be76b7ab05dd4198d23b";
  return generateHashKey(part1 + part3 + part2 + part4);
}

int DataUtils::generateHashKey(const string& key) {
  // WARNING, CHANGING THIS BASICALLY BRAKES EVERYTHING.
  return abs(int(XXH32((key + to_string(__DI_HSALT_03__)).c_str(), int(key.length()),
                       __DI_HSALT_01__ + __DI_HSALT_02__)));
}
