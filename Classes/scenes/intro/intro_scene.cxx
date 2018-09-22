// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "intro_scene.hxx"

IntroScene::IntroScene(const ValueMap& params, BackgroundLayer*) : BaseScene(params, nullptr) {}

IntroScene::~IntroScene() = default;

ValueMap IntroScene::generateParams() {
  // Add default params
  ValueMap params;
  params.emplace(__SCRIPT__, __INTRO_SCENE__);
  params.emplace(__NAME__, typeid(IntroScene).name());

  // Return default values
  return params;
}

void IntroScene::beforeInitialized() {
  
}

bool IntroScene::onInitialized() {
  AudioUtils::getInstance()->playAudioWithParam("event:/Music/Moon Ray", "fullLoop", 1.0f);

  // Schedule our load function which will orchestrate this intro sequence
  schedule(schedule_selector(IntroScene::monitor), 0.5f);
  return true;
}

void IntroScene::monitor(float dt) {
  // Getting a reference to all sprite on the layer
  const auto& probymozart = sprite_manager_->findSprite(__INTRO_ASSET_PROD_BY_MOZART__);
  const auto& cocos = sprite_manager_->findSprite(__INTRO_ASSET_COCOS__);
  const auto& firebase = sprite_manager_->findSprite(__INTRO_ASSET_FIREBASE__);
  const auto& fmod = sprite_manager_->findSprite(__INTRO_ASSET_FMOD__);

  // Make sure actions are not running on any of the sprites
  if (ActionUtils::isRunningAction({probymozart, cocos, firebase, fmod})) return;

  // Remove this function from scheduler.
  unschedule(schedule_selector(IntroScene::monitor));

  // Check to see if the user has completed the tutorial yet. It not, transition to the tutorial scene.
  if (DataUtils::getOtherData(__KEY_TUTORIAL_COMPLETE__, 0))
    SceneUtils::replaceScene(MODES, BackgroundLayer::create());
  else
    SceneUtils::replaceScene(CONTROLS, BackgroundLayer::create());
}
