/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __MODES_SCENE_HXX__
#define __MODES_SCENE_HXX__

/**
 * Scene Script
 */
#define __MODES_SCENE__ "scenes/modes.plist"
#define __MODES_CONFIG__ "configs/modes.plist"
/**
 * Assets
 */
#define __MODES_ASSET_LOGO__ "asset_modes_logo"
#define __MODES_ASSET_SETTINGS__ "asset_modes_settings"
#define __MODES_ASSET_LOGIN__ "asset_modes_login"
#define __MODES_ASSET_SHOP__ "asset_modes_shop"
#define __MODES_ASSET_RATE__ "asset_modes_rate"

/**
 * PageView Size
 */
#define __MODES_PAGE_VIEW_SIZE__ cocos2d::Size(768, 1136)

/**
 * Includes
 */
#include "scenes/base_scene.hxx"
#include "ui/CocosGUI.h"

/**
 * Namespaces
 */
using namespace ui;

class ModesScene : public BaseScene {
  /**
   * Consturctor.
   *
   * @param params ~ parameters needed to initialze the scene.
   * @param bg     ~ pointer to the constant background layer.
   */
  explicit ModesScene(const ValueMap& params, BackgroundLayer* bg);

  /**
   * Destructor.
   */
  virtual ~ModesScene();

 public:
  /**
   * Function to generate the default paramaters needed for a scene to function.
   *
   * @returns ~ Generated default params.
   */
  static ValueMap generateParams();

  /**
   * __CREATE_COCOS__
   */
  __CREATE_COCOS__(ModesScene, ValueMap, BackgroundLayer*)

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
   * Initializes the menu for the modes scene.
   */
  void initMenu();

  /**
   * Initializes the modes page view.
   */
  void initModesView();

  /**
   * Created a panel with the data from the `modes_config_` object.
   *
   * @param data ~ ValueMap data describing a specific mode
   */
  Layout* createModePanel(const ValueMap& data) const;

 private:
  // The page view that allows navigation through the modes.
  PageView* modes_view_;

  // Data neede dto populate the modes view
  ValueVector modes_config_;

  // @link __DISALLOW_COPY_AND_ASSIGN__
  __DISALLOW_COPY_AND_ASSIGN__(ModesScene)
};

#endif  // __MODES_SCENE_HXX__
