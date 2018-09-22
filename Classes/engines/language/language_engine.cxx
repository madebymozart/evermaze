// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "language_engine.hxx"
#include "utils/archive/archive_utils.hxx"

// Singleton Declaration
atomic<LanguageEngine*> LanguageEngine::instance_;
mutex LanguageEngine::instance_mutex_;

LanguageEngine::LanguageEngine()
    : language_code_(UserDefault::getInstance()->getStringForKey(
          __SETTING_LANGUAGE_CODE__, string(Application::getInstance()->getCurrentLanguageCode()) + "/")) {}

LanguageEngine::~LanguageEngine() {
  language_code_.clear();
  current_words_cache_.clear();
}

LanguageEngine* LanguageEngine::getInstance() {
  auto init = instance_.load(memory_order_relaxed);
  atomic_thread_fence(memory_order_acquire);

  if (init == nullptr) {
    // Invoke lock guards.
    lock_guard<mutex> lock(instance_mutex_);
    init = instance_.load(memory_order_relaxed);

    // Initialize a new instance if we are still null
    if (init == nullptr) {
      init = new (nothrow) LanguageEngine();

      // Release guards
      atomic_thread_fence(memory_order_release);

      // Store new FirebaseEngine Instance
      instance_.store(init, memory_order_relaxed);
    }
  }

  // Return
  return init;
}

void LanguageEngine::destroyInstance() { CC_SAFE_DELETE(instance_); }

void LanguageEngine::cacheWords(const string& name) {
  // Check if we already cached this word pack.
  if (currenty_files_cache_.find(name) not_eq currenty_files_cache_.end()) return;

  // If the cache limit has been reached, We should clear half the cache from the top
  if (current_words_cache_.size() > __WORD_CACHE_LIMIT__) {
    auto itr = current_words_cache_.begin();
    advance(itr, __WORD_CACHE_LIMIT__ / 2);
    current_words_cache_.erase(current_words_cache_.begin(), itr);
  }

  // If the language override is set, then we use that to get the current language
  const auto folder = string(__LANGUAGES__);
  const auto words = ArchiveUtils::loadValueMap(folder + language_code_ + name);

  // all words into the cache
  current_words_cache_.insert(words.begin(), words.end());
  currenty_files_cache_.emplace(name);
}

string LanguageEngine::getStringForKey(const string& key) {
  if (current_words_cache_.find(key) not_eq current_words_cache_.end())
    return current_words_cache_.at(key).asString();

  CCLOG("LanguageEngine: No string found for key: %s", key.c_str());
  return string();
}

string LanguageEngine::getFontPath() const {
  // For the specific cases, give the exact path.
  switch (Globals::sshash(language_code_.c_str())) {
    case Globals::sshash("jp/"):
    case Globals::sshash("ch/"):
    case Globals::sshash("it/"):
    case Globals::sshash("fr/"):
      return string("fonts/" + language_code_);
    default:
      return string("fonts/en/");
  }
}
