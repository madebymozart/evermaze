// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "modes_scene.hxx"
#include "Particle3D/PU/CCPUColorAffector.h"
#include "utils/data/data_utils.hxx"

ModesScene::ModesScene(const ValueMap& params, BackgroundLayer* bg)
    : BaseScene(params, bg), modes_view_(nullptr) {
  // Initiaize Modes data
  modes_config_ = ArchiveUtils::loadValueVector(__MODES_CONFIG__);
  CCASSERT(not modes_config_.empty(), "ModesScene: Modes view data did not load...");
}

ModesScene::~ModesScene() = default;

ValueMap ModesScene::generateParams() {
  // Add default params
  ValueMap params;
  params.emplace(__SCRIPT__, __MODES_SCENE__);
  params.emplace(__NAME__, typeid(ModesScene).name());

  // Return default values
  return params;
}

void ModesScene::beforeInitialized() {}

bool ModesScene::onInitialized() {
  // Delay the appearance of the background layer until the into animation is done.
  overlay_layer_->setOpacity(255);
  overlay_layer_->_setLocalZOrder(5);
  overlay_layer_->runAction(ActionUtils::sequence({
      ActionUtils::fadeOut(1.2f, 0.3f),
  }));

  initMenu();
  initModesView();

  return true;
}

void ModesScene::initMenu() {
  const auto& settings = menu_manager_->findItem(__MODES_ASSET_SETTINGS__);
  const auto& login = menu_manager_->findItem(__MODES_ASSET_LOGIN__);
  const auto& shop = menu_manager_->findItem(__MODES_ASSET_SHOP__);
  const auto& rate = menu_manager_->findItem(__MODES_ASSET_RATE__);

  // Click callback for settings
  settings->setCallback([&](Ref*) -> void {});

  // Click callback for login
  login->setCallback([&](Ref*) -> void {
    // Disable modes view
    modes_view_->setEnabled(false);

    // Reference to overlay to fade back in and restart the scene
    const auto& overlay = main_layer_->getChildByName(__OVERLAY__);
    overlay->setLocalZOrder(999);
    overlay->runAction(ActionUtils::sequence({ActionUtils::fadeIn(0, 0.7f), CallFunc::create([=]() -> void {
                                                SceneUtils::replaceScene(INTRO, background_layer_);
                                              })}));
  });

  // Click callback for shop
  shop->setCallback([&](Ref* sender) -> void {});

  // Click callback for rate
  rate->setCallback([&](Ref* sender) -> void {});
}

void ModesScene::initModesView() {
  modes_view_ = PageView::create();
  modes_view_->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  modes_view_->setContentSize(__MODES_PAGE_VIEW_SIZE__);
  modes_view_->setDirection(PageView::Direction::HORIZONTAL);
  modes_view_->setIndicatorEnabled(true);
  modes_view_->setIndicatorIndexNodesScale(0.25);
  modes_view_->setIndicatorSpaceBetweenIndexNodes(0);
  modes_view_->setIndicatorPosition(Globals::getScreenPosition(2, 8.5f));
  modes_view_->setIndicatorSelectedIndexColor(Color3B::WHITE);
  modes_view_->setPropagateTouchEvents(true);
  modes_view_->setPosition(Globals::getScreenPosition());

  // Populate the modes view data.
  for_each(modes_config_.begin(), modes_config_.end(),
           [&](const Value& data) { modes_view_->pushBackCustomItem(createModePanel(data.asValueMap())); });

  // Set the mode to the last mode the player ever clicked into.
  const auto index = DataUtils::getOtherData(__KEY_CURRENT_MODE__, 0);
  modes_view_->setCurrentPageIndex(index);

  // Add modes view
  main_layer_->addChild(modes_view_, 0);
}

Layout* ModesScene::createModePanel(const ValueMap& data) const {
  // Configure panel
  auto panel = Layout::create();
  panel->setContentSize(__MODES_PAGE_VIEW_SIZE__);
  panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  panel->setCascadeOpacityEnabled(true);
  panel->setPosition(
      Vec2(modes_view_->getContentSize().width / 2.0f, modes_view_->getContentSize().height / 2.0f));

  const auto& trial = Button::create(data.at(__TEXTURE__).asString(), "", "", Widget::TextureResType::PLIST);
  trial->setZoomScale(0);
  trial->setPosition(Vec2(panel->getContentSize().width / 2.0f, panel->getContentSize().height / 2.0));
  trial->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  trial->setCascadeOpacityEnabled(true);

  // When the modes is clicked...
  trial->addTouchEventListener([=](Ref* sender, const Widget::TouchEventType type) -> const void {
    if (type == Widget::TouchEventType::ENDED) {
      // Run Asserstions
      CCASSERT(sender not_eq nullptr, "ModesScene: sender is null...");
      modes_view_->setEnabled(false);

      // Current page index
      const auto index = static_cast<int>(modes_view_->getCurrentPageIndex());

      // Save the currently selected page in the data base.
      DataUtils::saveOtherData(__KEY_CURRENT_MODE__, index);

      // Transition to the next scene with params
      ValueMap params;
      params.emplace(__MODE__, index);
      overlay_layer_->setLocalZOrder(998);
      overlay_layer_->runAction(
          ActionUtils::sequence({ActionUtils::fadeIn(0, 0.5f), CallFunc::create([=]() -> void {
                                   SceneUtils::replaceScene(LEVELS, background_layer_, params, 0.5f);
                                 })}));
    }
  });
  panel->addChild(trial, 10);

  // Load title & description
  panel->addChild(text_manager_->findLabel(data.at(__TITLE__).asString()), 20);
  panel->addChild(text_manager_->findLabel(data.at(__DESCRIPTION__).asString()), 20);

  // The particle background
  auto particle_bg = ParticleUtils::load(data.at(__PARTICLE__).asString());
  particle_bg->setPosition(trial->getPosition());
  particle_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  particle_bg->stopSystem();
  particle_bg->runAction(
      ActionUtils::sequence({DelayTime::create(1), CallFunc::create([=]() { particle_bg->resetSystem(); })}));
  panel->addChild(particle_bg, 5);

  // Return completed panel
  return panel;
}
