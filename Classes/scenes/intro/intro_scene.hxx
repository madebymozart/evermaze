/** Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

#ifndef __INTRO_SCENE_HXX__
#define __INTRO_SCENE_HXX__

/**
 * Scene Script
 */
#define __INTRO_SCENE__ "scenes/intro.plist"

/**
 * Assets
 */
#define __INTRO_ASSET_COCOS__ "asset_intro_cocos2dx"
#define __INTRO_ASSET_FIREBASE__ "asset_intro_firebase"
#define __INTRO_ASSET_FMOD__ "asset_intro_fmod"
#define __INTRO_ASSET_PROD_BY_MOZART__ "asset_intro_prod_by_mozart_logo"

/**
 * Includes
 */
#include "scenes/base_scene.hxx"

class IntroScene : public BaseScene {
  /**
   * Consturctor.
   *
   * @param params ~ parameters needed to initialze the scene.
   * @param bg     ~ pointer to the constant background layer.
   */
  explicit IntroScene(const ValueMap& params, BackgroundLayer* bg);

  /**
   * Destructor.
   */
  virtual ~IntroScene();

 public:
  /**
   * Function to generate the default paramaters needed for a scene to function.
   *
   * @returns ~ Generated default params.
   */
  static ValueMap generateParams();

  // __CREATE_COCOS__
  __CREATE_COCOS__(IntroScene, ValueMap, BackgroundLayer*)

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
   * Monitors the intro scene and wait for the animations to finish before deciding where to
   * go next.
   *
   * @param dt ~ 1 / framrate, starting from when it was scheduled.
   */
  void monitor(float dt);

 private:
  __DISALLOW_COPY_AND_ASSIGN__(IntroScene)
};

#endif  // __INTRO_SCENE_HXX__
