/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __CONTROLS_GAME_SCENE_HXX__
#define __CONTROLS_GAME_SCENE_HXX__

/**
 * Scene Script
 */
#define __CONTROLS_SCENE__ "scenes/controls.plist"
#define __CONTROLS_CONFIG__ "configs/controls.plist"
#define __CONTROLS_TMX_PATH__ "tmx/controls/"
#define __CONTROLS_TMX__ "controls"

/**
 * Tmx Layers
 */
#define __CONTROLS_LAYER_ONE__ "controls_layer_one"
#define __CONTROLS_LAYER_TWO__ "controls_layer_two"

/**
 * Assets
 */
#define __CONTROLS_ASSET_LOGO__ "asset_controls_logo"
#define __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE__ "asset_controls_checkbox_reverse_swipe"
#define __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE_CHECKED__ "asset_controls_checkbox_reverse_swipe_checked"
#define __CONTROLS_ASSET_SKIP__ "asset_controls_skip"
#define __CONTROLS_ASSET_HAND_SWIPE__ "asset_controls_hand_swipe"

/**
 * Actions
 */
#define __CONTROLS_ACTION_REVERSE_SWIPE_CHECKBOX__ "actions/controls_reverse_swipe_checkbox.plist"

/**
 * Events
 */
#define __CONTROLS_EVENT_SWIPE__ "swipe_event"
#define __CONTROLS_EVENT_EXIT__ "exit_event"
#define __CONTROLS_EVENT_BEWARE__ "beware_event"

/**
 * Text
 */
#define __CONTROLS_TEXT_CONTROLS__ "controls_controls"
#define __CONTROLS_TEXT_REVERSE__ "controls_reverse_swipe"
#define __CONTROLS_TEXT_SKIP__ "controls_skip"
#define __CONTROLS_TEXT_TITLE__ "controls_title"
#define __CONTROLS_TEXT_DESCRIPTION__ "controls_description"

/**
 * Other
 */
#define __CONTROLS_STOP_PLAYER__ "stop_player"

/**
 * Includes
 */
#include <unordered_set>
#include "objects/tmx/tmx_object.hxx"
#include "scenes/game/base_game_scene.hxx"
#include "ui/CocosGUI.h"

/**
 * Namespaces
 */
using namespace ui;

class ControlsGameScene : public BaseGameScene {
  /**
   * Constructor.
   *
   * @param params ~ Parameters needed to initialize the scene.
   * @param bg     ~ Pointer to the constant background layer.
   */
  explicit ControlsGameScene(const ValueMap& params, BackgroundLayer* bg);

  /**
   * Destructor.
   */
  virtual ~ControlsGameScene();

 public:
  /**
   * Function to generate the default parameters needed for a scene to function.
   *
   * @returns ~ Generated default params.
   */
  static ValueMap generateParams();

 protected:
  /**
   * @see BaseScene::beforeInitialize()
   */
  void beforeInitialized() override;
  
  /**
   * @see BaseScene#onInitialized()
   */
  bool onInitialized() override;

  /**
   * @see BaseGameScene#initializeMap(string, string)
   */
  void initializeMap(const string& path, const string& name) override;

  /**
   * @see BaseGameScene#initializeUi()
   */
  void initializeUi() override;

  /**
   * @see BaseGameScene#initializeMenu()
   */
  void initializeMenu() override;

  /**
   * @see BaseGameScene#initializeMenu()
   */
  void initializeText() override;

  /**
   * @see BaseGameScene::initializePlayer()
   */
  void initializePlayer() override;

  /**
   * @see BaseGameScene::onCollision();
   */
  void onCollision() override;

  /**
   * @see BaseGameScene::logic()
   */
  bool logic() override;

  /**
   * @see BaseGameScene::finish
   */
  void finish(Scenes scene, CallFunc* function) override;

  /**
   * Triggers a tutorial event the reads from the script to display instructions for the player.
   *
   * @param event ~ The event that was triggered.
   *
   * @retruns ~ true if player swiping should be disabled because of the event specified, otherwise false.
   */
  bool onTutorialEvent(const string& event);

  /**
   * Triggers a pan for a specific event on the controls scene. Disables movement and other functionality
   * until pan action is complete.
   *
   * @param name         ~ Name of the event.
   * @param events       ~ Reference to the events object
   * @param event_params ~ Parameters for the specific event.
   */
  void onPan(const string& name, ValueMap& events, const ValueMap& event_params);

  /**
   * Create the "Reverse Swipe" Check box.
   */
  void initializeReverseSwipeCheckbox();

  // The check box used to invert the swiping when enabled
  CheckBox* reverse_swipe_checkbox_{};

  // map containing defined points that will be used to "narrate" the controls tutorial
  unordered_map<Point, string, PointHash> check_points_{};

  // Reference to the main ui TmxObject
  TmxObject* tmx_object_ui_{};

 public:
  /**
   * __CREATE_COCOS_PHYSICS__
   */
  __CREATE_COCOS_PHYSICS__(ControlsGameScene, ValueMap, BackgroundLayer*)

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(ControlsGameScene)
};

#endif  // __CONTROLS_GAME_SCENE_HXX__
