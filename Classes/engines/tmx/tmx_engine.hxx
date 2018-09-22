/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __TMX_ENGINE_HXX__
#define __TMX_ENGINE_HXX__

/**
 * Cache limit for TmxObjects
 */
#define __TMX_OBJECT_CACHE_LIMIT__ 10
#define __TMX_CONFIG_CACHE_LIMIT__ 3

/**
 * Config files to construct TmxObjects
 */
#define __TMX_CONTROLS_CONFIG__ "configs/tmx_controls.plist"
#define __TMX_ESCAPE_CONFIG__ "configs/tmx_tutorial.plist"
#define __TMX_ENERGY_CONFIG__ "configs/tmx_tutorial.plist"
#define __TMX_SEARCH_CONFIG__ "configs/tmx_tutorial.plist"
#define __TMX_SURVIVE_CONFIG__ "configs/tmx_tutorial.plist"
#define __TMX_PORTALS_CONFIG__ "configs/tmx_tutorial.plist"
#define __TMX_TILES_CONFIG__ "configs/tmx_tutorial.plist"

/**
 * Includes
 */
#include "globals.hxx"
#include "objects/tmx/tmx_object.hxx"

/**
 * Forward Declarations
 */
class BaseLluma;

class TmxEngine {
  /**
   * Constructor.
   */
  explicit TmxEngine();

  /**
   * Destructor.
   */
  virtual ~TmxEngine();

  /**
   * Friend Class
   */
  friend BaseLluma;

 public:
  /**
   * Gets singleton instance of this class. If the instance is null, it will initialize it.
   *
   * @returns ~ The singleton instance of the TmxEngine.
   *
   * @note ~ This was implemented using acquire and release fences. Since the instance is wraped
   * in a C++11 atomic type, we can manipulate it using relaxed atomic operations.
   */
  static TmxEngine* getInstance();

  /**
   * Destroys the instance of this class.
   */
  static void destroyInstance();

  /**
   * Caches a TMX object fot the `map`.
   *
   * @param map  ~ Loaded TMX map.
   * @param mode ~ Mode needed to load the correct info file for the tmx object.
   */
  void cache(TMXTiledMap* map, Modes mode);

  /**
   * Gets the TmxObject that has been cached by with the key
   *
   * @param key ~ The key of the TmxObject to search.
   *
   * @retruns ~ @link TmxObject
   */
  TmxObject* getTmxObject(const string& key);

 private:
  // Map hold tmx objects that have already been processed.
  unordered_map<string, TmxObject*> tmx_object_cache_;

  // Map holding info files. We cache these since it is likely that the user will be playing the
  // same modes for quite some time.
  unordered_map<Modes, ValueVector> tmx_config_cache_;

  // Singleton instance of this class.
  static atomic<TmxEngine*> instance_;

  // Mutex to make accessing the singleton thread-safe
  static mutex instance_mutex_;

  // __DISALLOW_COPY_AND_ASSIGN__
  __DISALLOW_COPY_AND_ASSIGN__(TmxEngine)
};

#endif  // __TMX_ENGINE_HXX__
