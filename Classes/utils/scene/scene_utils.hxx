/**  Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __SCENE_UTILS_HXX__
#define __SCENE_UTILS_HXX__

/**
 * Default Transition Speed
 */
#define __DEFAULT_TRANSITION_SPEED__ 0.7f

/**
 * Includes
 */
#include "globals.hxx"
#include "layers/background/background_layer.hxx"

/**
 * @note This class work as a broker to allow the BaseScene to create the scenes once the transition is
 * acually done running
 */
class SceneUtils {
 public:
  /**
   * Replaces a scene with a define one. This will take in a
   *
   * @param scene  ~ The scene to load.
   */
  static void replaceScene(const Scenes& scene);

  /**
   * Replaces a scene with a define one. This will take in a
   *
   * @param scene  ~ The scene to load.
   * @param bg     ~ The background pointer.
   */
  static void replaceScene(const Scenes& scene, BackgroundLayer* bg);

  /**
   * Replaces a scene with a define one. This will take in a
   *
   * @param scene  ~ The scene to load.
   * @param bg     ~ The background pointer.
   * @param params ~ params to be passed to the scene
   */
  static void replaceScene(const Scenes& scene, BackgroundLayer* bg, const ValueMap& params);

  /**
   * Replaces a scene with a define one. This will take in a
   *
   * @param scene  ~ The scene to load.
   * @param bg     ~ The background pointer.
   * @param speed  ~ Speed in which transition should happen. Default is 0.7f
   */
  static void replaceScene(const Scenes& scene, BackgroundLayer* bg, float speed);

  /**
   * Replaces a scene with a define one. This will take in a
   *
   * @param scene  ~ The scene to load
   * @param bg     ~ The background pointer
   * @param params ~ params to be passed to the scene
   * @param speed  ~ Speed in which transition should happen. Default is 0.7f
   */
  static void replaceScene(const Scenes& scene, BackgroundLayer* bg, const ValueMap& params, float speed);

 private:
  /**
   * Create a scene base on the scene parameter and passes the params and background layer.
   *
   * @param scene  ~ The scene to load.
   * @param params ~ params to be passed to the scene.
   * @param bg     ~ The background pointer.
   *
   * @returns ~ New scene pointer
   */
  static Scene* createScene(const Scenes& scene, BackgroundLayer* bg, ValueMap params);

  /**
   * Function to merge to ValueMap.
   *
   * @param ref     ~ Reference to the original map.
   * @param merging ~ map to merge into ref map.
   *
   * @returns ~ reference map with merging maps data.
   *
   * @note ~ We pass back the reference map that has the merged data. This mainly allows
   * one-liners for making a new scene.
   */
  static ValueMap merge(ValueMap& ref, const ValueMap& merging);

  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(SceneUtils)
};

#endif  // __SCENE_UTILS_HXX__
