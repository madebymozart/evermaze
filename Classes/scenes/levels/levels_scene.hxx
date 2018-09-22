/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __LEVELS_SCENE_HXX__
#define __LEVELS_SCENE_HXX__

/**
 * Levels Script
 */
#define __LEVELS_SCENE__ "scenes/levels.plist"

/**
 * Levels Icons
 */
#define __LEVELS_ESCAPE_ICON__ "asset_levels_escape"
#define __LEVELS_ENERGY_ICON__ "asset_levels_energy"
#define __LEVELS_SEARCH_ICON__ "asset_levels_search"
#define __LEVELS_SURVIVE_ICON__ "asset_levels_survive"
#define __LEVELS_PORTALS_ICON__ "asset_levels_portals"
#define __LEVELS_TILES_ICON__ "asset_levels_tiles"

/**
 * Levels Actions
 */
#define __LEVELS_MODE_ICON_ACTION__ "actions/levels_mode_intro.plist"

/**
 * Includes
 */
#include "scenes/base_scene.hxx"

class LevelsScene : public BaseScene {
  /**
   * Consturctor.
   *
   * @param params ~ parameters needed to initialze the scene.
   * @param bg     ~ pointer to the constant background layer.
   */
  explicit LevelsScene(const ValueMap& params, BackgroundLayer* bg);

  /**
   * Destructor.
   */
  virtual ~LevelsScene();

 public:
  /**
   * Function to generate the default paramaters needed for a scene to function.
   *
   * @returns ~ Generated default params.
   */
  static ValueMap generateParams();

  __CREATE_COCOS__(LevelsScene, ValueMap, BackgroundLayer*)

 protected:
  /**
   * @see BaseScene::beforeInitialize()
   */
  void beforeInitialized() override;
  
  /**
   * @see BaseScene::onInitialized()
   */
  bool onInitialized() override;

  /**
   * Initializes Sprites
   */
  void initSprites();

  /**
   * Loads the correct mode icon based on the current mode.
   *
   * @return ~ Current icon for mode.
   */
  Sprite* getCurrentModeIcon();

  /**
   * Monitors the intro scene and wait for the animations to finish before deciding where to
   * go next.
   *
   * @param dt ~ 1 / framrate, starting from when it was scheduled.
   */
  void monitor(float dt);

  // The current mode of the level
  const Modes mode_;

  // Icon for te current mode
  Sprite* mode_icon_;

 private:
  __DISALLOW_COPY_AND_ASSIGN__(LevelsScene)
};

#endif  // __LEVELS_SCENE_HXX__
