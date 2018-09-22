/** Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

/**
 * Include Guard
 */
#ifndef __BASE_LAYER_HXX__
#define __BASE_LAYER_HXX__

/**
 * Includes
 */
#include "globals.hxx"
#include "layers/background/background_layer.hxx"

/**
 * Include Managers
 */
#include "managers/menu/menu_manager.hxx"
#include "managers/sprite/sprite_manager.hxx"
#include "managers/text/text_manager.hxx"

/**
 * Include Utils
 */
#include "utils/action/action_utils.hxx"
#include "utils/archive/archive_utils.hxx"
#include "utils/data/data_utils.hxx"
#include "utils/particle/particle_utils.hxx"
#include "utils/scene/scene_utils.hxx"
#include "utils/thread/thread_utils.hxx"

/**
 * Include Engines
 */
#include "engines/fmod/fmod_engine.hxx"

class BaseScene : public Scene {
 protected:
  /**
   * Constructor.
   *
   * @param params ~ ValueMap containing parameters for this scene
   * @param bg     ~ Constant pointer to the background layer instance
   */
  explicit BaseScene(const ValueMap& params, BackgroundLayer* bg = nullptr);

  /**
   * Destructor.
   */
  virtual ~BaseScene();

  /**
   * @link Scene::update()
   */
  void update(float interval) override;

  /**
   * @link Scene::onEnter()
   */
  void onEnter() override;

  /**
   * @link Scene::onExit()
   */
  void onExit() override;

  /**
   * Initializes the scene.
   *
   * @note This override initializes all managers based on the data file found in the `path` parameter. The
   * file points to other files containing initialization information for the managers.
   *
   * @return true if initialize() is successful, otherwise false.
   */
  virtual void initialize();
  
  /**
   * Function called when before initialize() starts
   *
   * @note ~ All children classes must implement this since it is a pure virtual function.
   */
  virtual void beforeInitialized() = 0;

  /**
   * Function called when initialize() is finished.
   *
   * @return ~ True if onInitialized() is successful, otherwise false.
   *
   * @note ~ All children classes must implement this since it is a pure virtual function.
   */
  virtual bool onInitialized() = 0;

  // Manages all of the sprites that are associated with this scene.
  unique_ptr<SpriteManager> sprite_manager_;

  // Manages the menu associated with this screen. There is only one menu per scene
  unique_ptr<MenuManager> menu_manager_;

  // Manages all of the text labels that are associated with this scene.
  unique_ptr<TextManager> text_manager_;

  // These are initializer params that will be used in constructors of children classes. This allows each
  // class to initialize with ValueMap of different types.
  unique_ptr<ValueMap> params_;

  // The main loading script. Contains paths to manager script and can also contain level data.
  unique_ptr<ValueMap> script_;

  // The main layer the scene. This is where most content will be placed
  Layer* main_layer_;
  
  // The overlay screen that cant be positioned in many diffent cases
  LayerColor* overlay_layer_;
  
  // The main loading script. Contains paths to manager script and can also contain level data.
  BackgroundLayer* background_layer_;

 private:
  __DISALLOW_COPY_AND_ASSIGN__(BaseScene)
};

#endif  // __BASE_LAYER_HXX__
