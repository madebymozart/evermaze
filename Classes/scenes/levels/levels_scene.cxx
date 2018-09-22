// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "levels_scene.hxx"

LevelsScene::LevelsScene(const ValueMap& params, BackgroundLayer* bg)
    : BaseScene(params, bg), mode_(Modes(params.at(__MODE__).asInt())) {}

LevelsScene::~LevelsScene() = default;

ValueMap LevelsScene::generateParams() {
  // Add default params
  ValueMap params;
  params.emplace(__SCRIPT__, __LEVELS_SCENE__);
  params.emplace(__NAME__, typeid(LevelsScene).name());

  // Return default values
  return params;
}

void LevelsScene::beforeInitialized() {
  overlay_layer_->_setLocalZOrder(5);
  overlay_layer_->setOpacity(255);
}

bool LevelsScene::onInitialized() {
  // Delay the appearance of the background layer until the into animation is done.
  overlay_layer_->runAction(ActionUtils::sequence({
      ActionUtils::fadeOut(1.2f, 0.3f),
  }));

  // Initialize
  initSprites();

  // Schedule our load function which will orchestrate this intro sequence
  schedule(schedule_selector(LevelsScene::monitor), 0.5f);

  return true;
}

void LevelsScene::initSprites() {
  // Find the correct icon for the current mode
  mode_icon_ = getCurrentModeIcon();

  // Run the animation of the mode icon
  const auto action = ActionUtils::makeAction(ArchiveUtils::loadValueMap(__LEVELS_MODE_ICON_ACTION__));
  mode_icon_->runAction(action);
  main_layer_->addChild(mode_icon_, 10);
}

Sprite* LevelsScene::getCurrentModeIcon() {
  // From params, we should load the correct icon for mode from the __MODE__ param
  switch (mode_) {
    case ESACPE:
      return sprite_manager_->findSprite(__LEVELS_ESCAPE_ICON__);
    case ENERGY:
      return sprite_manager_->findSprite(__LEVELS_ENERGY_ICON__);
    case SEARCH:
      return sprite_manager_->findSprite(__LEVELS_SEARCH_ICON__);
    case SURVIVE:
      return sprite_manager_->findSprite(__LEVELS_SURVIVE_ICON__);
    case PORTALS:
      return sprite_manager_->findSprite(__LEVELS_PORTALS_ICON__);
    case TILES:
      return sprite_manager_->findSprite(__LEVELS_TILES_ICON__);
    default:
      throw runtime_error("LevelsScene: Could not find tis mode...");
  }
}

void LevelsScene::monitor(float dt) {}
