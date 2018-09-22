// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "tmx_engine.hxx"
#include "utils/archive/archive_utils.hxx"

// Singleton Declaration
atomic<TmxEngine*> TmxEngine::instance_;
mutex TmxEngine::instance_mutex_;

TmxEngine::TmxEngine() = default;

TmxEngine::~TmxEngine() = default;

TmxEngine* TmxEngine::getInstance() {
  auto init = instance_.load(memory_order_relaxed);
  atomic_thread_fence(memory_order_acquire);

  if (init == nullptr) {
    // Invoke lock guards.
    lock_guard<mutex> lock(instance_mutex_);
    init = instance_.load(memory_order_relaxed);

    // Initialize a new instance if we are still null
    if (init == nullptr) {
      init = new (nothrow) TmxEngine();

      // Release guards
      atomic_thread_fence(memory_order_release);

      // Store new FirebaseEngine Instance
      instance_.store(init, memory_order_relaxed);
    }
  }

  // Return
  return init;
}

void TmxEngine::destroyInstance() { CC_SAFE_DELETE(instance_); }

void TmxEngine::cache(TMXTiledMap* map, const Modes mode) {
  // Check the cache to see if we have already cached a tmx object for this map before
  if (tmx_object_cache_.find(map->getName()) not_eq tmx_object_cache_.end()) return;

  // If the cache limit has been reached, We should clear half the cache from the top
  if (tmx_object_cache_.size() > __TMX_OBJECT_CACHE_LIMIT__ - 1) {
    auto itr = tmx_object_cache_.begin();
    advance(itr, __TMX_OBJECT_CACHE_LIMIT__ / 2);
    tmx_object_cache_.erase(tmx_object_cache_.begin(), itr);
  }

  if (tmx_config_cache_.size() > __TMX_CONFIG_CACHE_LIMIT__ - 1) {
    auto itr = tmx_config_cache_.begin();
    advance(itr, __TMX_CONFIG_CACHE_LIMIT__ / 2);
    tmx_config_cache_.erase(tmx_config_cache_.begin(), itr);
  }

  // Check to see if we have already cached this info before search a new one
  if (tmx_config_cache_.find(mode) == tmx_config_cache_.end()) {
    if (mode == CONTROLSM)
      tmx_config_cache_.emplace(CONTROLSM, ArchiveUtils::loadValueVector(__TMX_CONTROLS_CONFIG__));
    else if (mode == ESACPE)
      tmx_config_cache_.emplace(ESACPE, ArchiveUtils::loadValueVector(__TMX_ESCAPE_CONFIG__));
    else if (mode == ENERGY)
      tmx_config_cache_.emplace(ENERGY, ArchiveUtils::loadValueVector(__TMX_ENERGY_CONFIG__));
    else if (mode == SEARCH)
      tmx_config_cache_.emplace(SEARCH, ArchiveUtils::loadValueVector(__TMX_SEARCH_CONFIG__));
    else if (mode == SURVIVE)
      tmx_config_cache_.emplace(SURVIVE, ArchiveUtils::loadValueVector(__TMX_SURVIVE_CONFIG__));
    else if (mode == PORTALS)
      tmx_config_cache_.emplace(PORTALS, ArchiveUtils::loadValueVector(__TMX_PORTALS_CONFIG__));
    else if (mode == TILES)
      tmx_config_cache_.emplace(TILES, ArchiveUtils::loadValueVector(__TMX_TILES_CONFIG__));
  }

  // Cache TmxObject
  tmx_object_cache_.emplace(map->getName(), new TmxObject(*map, tmx_config_cache_.at(mode)));
}

TmxObject* TmxEngine::getTmxObject(const string& key) {
  // Sanity check that the TmxObject exists in the cache.
  CCASSERT(tmx_object_cache_.find(key) not_eq tmx_object_cache_.end(),
           string("TmxEngine: No TmxObject found for name" + key).c_str());
  return tmx_object_cache_.at(key);
}
