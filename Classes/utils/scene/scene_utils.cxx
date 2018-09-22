// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "scene_utils.hxx"
#include "scenes/game/controls/controls_game_scene.hxx"
#include "scenes/intro/intro_scene.hxx"
#include "scenes/levels/levels_scene.hxx"
#include "scenes/modes/modes_scene.hxx"

void SceneUtils::replaceScene(const Scenes& scene) {
  replaceScene(scene, nullptr, ValueMap(), __DEFAULT_TRANSITION_SPEED__);
}

void SceneUtils::replaceScene(const Scenes& scene, BackgroundLayer* bg) {
  replaceScene(scene, bg, ValueMap(), __DEFAULT_TRANSITION_SPEED__);
}

void SceneUtils::replaceScene(const Scenes& scene, BackgroundLayer* bg, const ValueMap& params) {
  replaceScene(scene, bg, params, __DEFAULT_TRANSITION_SPEED__);
}

void SceneUtils::replaceScene(const Scenes& scene, BackgroundLayer* bg, const float speed) {
  replaceScene(scene, bg, ValueMap(), speed);
}

void SceneUtils::replaceScene(const Scenes& scene, BackgroundLayer* bg, const ValueMap& params,
                              const float speed) {
  Director::getInstance()->replaceScene(TransitionFade::create(speed, createScene(scene, bg, params)));
}

Scene* SceneUtils::createScene(const Scenes& scene, BackgroundLayer* bg, ValueMap params) {
  if (scene == INTRO) {
    // Release Background since IntroLayer will create it again...
    bg->removeAllChildren();
    bg->removeFromParent();
    CC_SAFE_RELEASE(bg);

    return IntroScene::create(merge(params, IntroScene::generateParams()), nullptr);
  }

  if (scene == CONTROLS)
    return ControlsGameScene::create(merge(params, ControlsGameScene::generateParams()), bg);
  if (scene == MODES) return ModesScene::create(merge(params, ModesScene::generateParams()), bg);
  if (scene == LEVELS) return LevelsScene::create(merge(params, LevelsScene::generateParams()), bg);
  throw runtime_error("SceneUtils: No scene created...");
}

ValueMap SceneUtils::merge(ValueMap& ref, const ValueMap& merging) {
  ref.insert(merging.begin(), merging.end());
  return ref;
}
