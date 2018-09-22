/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __LANGUAGE_ENGINE_HXX__
#define __LANGUAGE_ENGINE_HXX__

/**
 * Defines
 */
#define __LANGUAGES__ "languages/"
#define __WORD_CACHE_LIMIT__ 150

/**
 * Includes
 */
#include <unordered_set>
#include "globals.hxx"

class LanguageEngine {
  /**
   * Constructor.
   */
  explicit LanguageEngine();

  /**
   * Destructor.
   */
  virtual ~LanguageEngine();

 public:
  /**
   * Gets singleton instance of this class. If the instance is null, it will initialize it.
   *
   * @returns ~ The singleton instance of the LanguageEngine.
   *
   * @note ~ This was implemented using acquire and release fences. Since the instance is wraped
   * in a C++11 atomic type, we can manipulate it using relaxed atomic operations.
   */
  static LanguageEngine* getInstance();

  /**
   * Destroys the inistance of this class.
   */
  static void destroyInstance();

  /**
   * Caches a words file containing a ValueMap of keys to strings.
   *
   * @param name ~ Name of word cache file.
   */
  void cacheWords(const string& name);

  /**
   * Gets a string from the `current_words_cache_`.
   *
   * @param key ~ key of the string we looking for.
   *
   * @retruns ~ String for key.
   *
   * @note ~ If the cache does not contain this key, An empty string will be retruned.
   */
  string getStringForKey(const string& key);

  /**
   * Gets the correct font path to load depending on the `language_code_`
   * or `override_language_code_`
   *
   * @return ~ The correct font path
   */
  string getFontPath() const;

 protected:
 private:
  // Current language code of the device
  string language_code_{};

  // Current word cache to load strings from. When a language changes, this get invalidated
  ValueMap current_words_cache_{};

  // Vector containing file names of word packs that have already been cached
  unordered_set<string> currenty_files_cache_{};

  // Singleton instance of this class.
  static atomic<LanguageEngine*> instance_;

  // Mutex to make accessing the singleton thread-safe
  static mutex instance_mutex_;

  // __DISALLOW_COPY_AND_ASSIGN__
  __DISALLOW_COPY_AND_ASSIGN__(LanguageEngine)
};

#endif  // __LANGUAGE_ENGINE_HXX__
