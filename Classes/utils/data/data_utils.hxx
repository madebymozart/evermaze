/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __DATA_INTERFACE_HXX__
#define __DATA_INTERFACE_HXX__

/**
 * Class Constant
 */
#ifndef COCOS2D_DEBUG
#define __DI_DB_NAME__ "evmz_core.data"
#else
#define __DI_DB_NAME__ "debug_db.sqlite"
#endif

/**
 * Hash Key Salts
 */
#define __DI_HSALT_01__ 19941118
#define __DI_HSALT_02__ 19960406
#define __DI_HSALT_03__ 20140618

/**
 * SQLite Tables
 */
#define __DI_SQLT_OTHER_DATA__ "other_data"
#define __DI_SQLT_SCORE_DATA__ "score_data"
#define __DI_SQLT_STORE_DATA__ "store_data"
#define __DI_SQLT_TRIAL_DATA__ "trial_data"

/**
 * SQLite Create Statements
 */
#define __DI_SQLC_OTHER_DATA__ \
  "CREATE TABLE IF NOT EXISTS other_data(key BIGINT PRIMARY KEY, value_int BIGINT, value_string TEXT);"
#define __DI_SQLC_SCORE_DATA__                                                                               \
  "CREATE TABLE IF NOT EXISTS score_data(level BIGINT NOT NULL, trial BIGINT NOT NULL, score BIGINT, stars " \
  "BIGINT, lluma TEXT, color TEXT, music TEXT, pattern TEXT, UNIQUE(level, trial));"
#define __DI_SQLC_STORE_DATA__ "CREATE TABLE IF NOT EXISTS store_data(item BIGINT UNIQUE);"
#define __DI_SQLC_TRIAL_DATA__                                                                           \
  "CREATE TABLE IF NOT EXISTS trial_data(trial BIGINT PRIMARY KEY, stars BIGINT, unlocked BIGINT, last " \
  "BIGINT);"

/**
 * Database Keys
 */
#define __KEY_CURRENT_COLOR__ "dk_current_color"
#define __KEY_CURRENT_LLUMA__ "dk_current_lluma"
#define __KEY_CURRENT_MUSIC__ "dk_current_music"
#define __KEY_CURRENT_COINS__ "dk_current_coins"
#define __KEY_CURRENT_PATTERN__ "dk_current_pattern"
#define __KEY_REMOVE_ADS__ "dk_remove_ads"
#define __KEY_ORBS__ "dk_coins"
#define __KEY_TOTAL_ORBS__ "dk_total_coins"
#define __KEY_TOTAL_ORBS_EVER__ "dk_total_orbs_coins"
#define __KEY_TUTORIAL_COMPLETE__ "key_tutorial_complete"
#define __KEY_CURRENT_MODE__ "key_current_mode"
#define __KEY_REVERSE_SWIPE__ "key_reverse_swipe"
/**
 * Includes
 */
#include "globals.hxx"
#include "sqleet/sqleet.h"

/**
 * Class
 */
class DataUtils {
 public:
  /**
   * Initializes the database for the first time. This is run by the AppDelegate class during initialization
   * of the application, which is why AppDelegate is a friend class
   */
  static void initDatabase();

  /**
   * Saves random key/pair integer values in the database. useful for items that aren't specific to any of the
   * other tables in the database.
   *
   * @param key   Key to access a value in database.
   * @param value (int)(string) Value to store in database.
   */
  static void saveOtherData(const string& key, int value = 0);

  static void saveOtherData(const string& key, string value);

  /**
   * Retrieves data from the sqlite database based on the key from the other_data table.
   *
   * @param key           Key used to lookup data.
   * @param default_value (int)(string) What we should return if nothing is found.
   */
  static int getOtherData(const string& key, int default_value = 0);

  static string getOtherData(const string& key, const string& default_value = string());

  /**
   * Saves data to the score table. This data includes the score, level, trial, and the configuration that was
   * used to achieve said score.
   *
   * @param values Map containing all the vlaue used to obtain the score
   */
  static void saveScoreData(ValueMap values);

  /**
   * Saves data to the score table. This data includes the score, level, trial,
   * and the configuration that was used to achieve said score.
   *
   * @param values Map containing all the vlaue used to obtain the score
   */
  static void saveTrialData(ValueMap values);

  /**
   * Saves data to the store table.
   *
   * @param item item to be saved in the database
   */
  static void saveStoreData(const string& item);

  /**
   * Retrieves data from the sqlite database based on the key from the score_table.
   *
   * @param item Item in the database
   * @param default_value
   * @returns string value;
   */
  static string getStoreData(const string& item, const string& default_value = string());

  static vector<string> getAllStoreData();

 private:
  /**
   * Gets and instance of the database. This will allow the database connections be thread safe since each
   * thread will have it's own instance of the database.
   *
   * @returns SQLite3 database instance
   */
  static sqlite3* getDatabase();

  /**
   * Using the xxhash library, this will generate a key on runtime for the database. Using a unique identifier
   * for each device will make it so that no other device can use the same database.
   *
   * @returns Generated database key
   */
  static int getDatabaseKey();

  /**
   * @brief Creates a hashed version of the key
   *
   * @note using xxhash 32-bit hashing algorithm. along with the key, the db_key is also used to create a more
   * powerful hash. since the db_key for every derive is unique, all keys to data will be unique to that
   * specific device, rendering copy's of databases to other devices useless.
   *
   * @param key Key to be hashed
   *
   * @returns Integer hashed key.
   */
  static int generateHashKey(const string& key);

  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(DataUtils)
};

#endif  /// __DATA_INTERFACE_HXX__
