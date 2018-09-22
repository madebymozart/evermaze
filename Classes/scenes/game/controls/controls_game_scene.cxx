// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "controls_game_scene.hxx"
#include "engines/language/language_engine.hxx"
#include "engines/tmx/tmx_engine.hxx"
#include "entities/llumas/base_lluma.hxx"
#include "entities/llurkers/base_llurker.hxx"

ControlsGameScene::ControlsGameScene(const ValueMap& params, BackgroundLayer* bg)
    : BaseGameScene(params, bg) {
  TmxEngine::destroyInstance();
}

ControlsGameScene::~ControlsGameScene() = default;

void ControlsGameScene::beforeInitialized() {
  
}

bool ControlsGameScene::onInitialized() {
  // Map Initialization
  initializeMap(__CONTROLS_TMX_PATH__, __CONTROLS_TMX__);

  // Initialize reference to ui Layer
  tmx_object_ui_ = TmxEngine::getInstance()->getTmxObject(map_->getName());

  // Base Initialization
  CCASSERT(BaseGameScene::onInitialized(), "ControlsGameScene: BaseGameScene failed to initialize...");

  // Local Initializations
  initializeReverseSwipeCheckbox();

  // Delay the appearance of the background layer and map as the logo animation runs
  loading_layer_->setLocalZOrder(9);
  loading_layer_->runAction(ActionUtils::sequence(
      {ActionUtils::fadeOut(1.9f, 0.3f), CallFunc::create([&]() { swipe_gesture_->setEnabled(true); })}));

  // Set Checkpoints for the controls tutorial.
  check_points_.emplace(Point(12, 20), __CONTROLS_EVENT_EXIT__);
  check_points_.emplace(Point(3, 7), __CONTROLS_EVENT_BEWARE__);

  // Show first text in the controls script
  onTutorialEvent(__CONTROLS_EVENT_SWIPE__);

  // Initialization Successful
  return true;
}

void ControlsGameScene::initializeMap(const string& path, const string& name) {
  BaseGameScene::initializeMap(path, name);
  map_->getLayer(__CONTROLS_LAYER_TWO__)->setOpacity(0);
}

void ControlsGameScene::initializeUi() { addChild(sprite_manager_->findSprite(__CONTROLS_ASSET_LOGO__), 10); }

void ControlsGameScene::initializeMenu() {
  const auto& skip = menu_manager_->findItem(__CONTROLS_ASSET_SKIP__);

  // Click callback for skip
  skip->setCallback([&](Ref*) -> void { finish(MODES, nullptr); });
  ui_layer_->addChild(menu_manager_->getMenu(), 10, __MENU__);
}

void ControlsGameScene::initializeText() {
  ui_layer_->addChild(text_manager_->findLabel(__CONTROLS_TEXT_CONTROLS__));
  ui_layer_->addChild(text_manager_->findLabel(__CONTROLS_TEXT_REVERSE__));
  ui_layer_->addChild(text_manager_->findLabel(__CONTROLS_TEXT_SKIP__));
  ui_layer_->addChild(text_manager_->findLabel(__CONTROLS_TEXT_TITLE__));
  ui_layer_->addChild(text_manager_->findLabel(__CONTROLS_TEXT_DESCRIPTION__));
}

void ControlsGameScene::initializePlayer() {
  player_ = BaseLluma::createLluma(this, swipe_gesture_,
                                   DataUtils::getOtherData(__KEY_CURRENT_LLUMA__, __DEFAULT_LLUMA__),
                                   __CONTROLS_LAYER_ONE__);
  player_->setCurrentPoint(Point(0, 24));
  player_->setPosition(
      tmx_object_ui_->getPosition(player_->getCurrentPoint(), player_->getCurrentTmxLayer()));
  main_layer_->addChild(player_, 99);
}

void ControlsGameScene::onCollision() {
  player_->setCanMove(false);
  player_->setSpeed(1000);

  finish(CONTROLS, CallFunc::create([&]() {
           main_layer_->runAction(ActionUtils::sequence({
               DelayTime::create(0.3f),
               CallFunc::create([&]() { player_->setEmissionRate(0); }),
           }));
         }));
}

bool ControlsGameScene::logic() {
  // Check if the tutorial was completed
  if (player_->getCurrentPoint() == Point(24, 6)) {
    ParticleUtils::stop(main_layer_->getChildByName(__EXIT__));
    finish(MODES, nullptr);
  }

  // Reference title and description
  const auto& title = text_manager_->findLabel(__CONTROLS_TEXT_TITLE__);
  const auto& description = text_manager_->findLabel(__CONTROLS_TEXT_DESCRIPTION__);
  if (title->getOpacity() not_eq 0 and not ActionUtils::isRunningAction(title)) {
    const auto fadeout = ActionUtils::fadeOut(2, 0.7f);
    title->runAction(fadeout->clone());
    description->runAction(fadeout->clone());
  }

  if (check_points_.find(player_->getCurrentPoint()) == check_points_.end()) return true;
  return onTutorialEvent(check_points_.at(player_->getCurrentPoint()));
}

void ControlsGameScene::finish(const Scenes scene, CallFunc* function) {
  // Disable object
  player_->setCanMove(false);
  for_each(llurkers_.begin(), llurkers_.end(), [](BaseLlurker* llurker) { llurker->setCanMove(false); });
  main_layer_->stopAllActions();
  swipe_gesture_->resetSwipeDirection();
  getEventDispatcher()->setEnabled(false);
  menu_manager_->getMenu()->setEnabled(false);

  // Set tutorial complete flag in database
  if (scene == MODES) DataUtils::saveOtherData(__KEY_TUTORIAL_COMPLETE__, 1);

  // Making the loading fade above all layers to transition
  loading_layer_->setLocalZOrder(999);
  loading_layer_->runAction(ActionUtils::sequence(
      {function, ActionUtils::fadeIn(0.6f, 0.5f),
       CallFunc::create([=]() -> void { SceneUtils::replaceScene(scene, background_layer_); })}));
}

bool ControlsGameScene::onTutorialEvent(const string& event) {
  if (script_->find(__EVENTS__) == script_->end()) return true;

  // Reference event from the scene script.
  // Check if the event exists in the map since event only happen once and could have run already.
  auto& events = script_->at(__EVENTS__).asValueMap();
  if (events.find(event) == events.end()) return true;

  // Transition layers if the player it at that point
  if (player_->getCurrentPoint() == Point(12, 20)) {
    map_->getLayer(__CONTROLS_LAYER_ONE__)->runAction(ActionUtils::fadeOut(0, 0.5f));
    map_->getLayer(__CONTROLS_LAYER_TWO__)->runAction(ActionUtils::fadeIn(0, 0.5f));
    player_->setCurrentTmxLayer(__CONTROLS_LAYER_TWO__);
  }

  // Reference the event parameters to determine functionality & check fro required params
  const auto& event_params = events.at(event).asValueMap();
  Globals::assertValueMap(event_params, {__TITLE__, __DESCRIPTION__, __CONTROLS_STOP_PLAYER__});

  // Set the title and description and run the fade in and out action on them.
  const auto& title = text_manager_->findLabel(__CONTROLS_TEXT_TITLE__);
  const auto& description = text_manager_->findLabel(__CONTROLS_TEXT_DESCRIPTION__);
  title->setString(LanguageEngine::getInstance()->getStringForKey(event_params.at(__TITLE__).asString()));
  description->setString(
      LanguageEngine::getInstance()->getStringForKey(event_params.at(__DESCRIPTION__).asString()));

  const auto action = ActionUtils::fadeIn(2, 0.7f);
  title->runAction(action->clone());
  description->runAction(action->clone());

  // if the player is stopping during this event, create the necessary pan needed and return false for logic
  const auto stop_player = event_params.at(__CONTROLS_STOP_PLAYER__).asBool();
  if (stop_player) onPan(event, events, event_params);

  // erase the event to prevent it from occurring again.
  events.erase(events.find(event));
  return not stop_player;
}

void ControlsGameScene::onPan(const string& name, ValueMap& events, const ValueMap& event_params) {
  // verify points exist in these params
  Globals::assertValueMap(event_params, {__POSX__, __POSY__});

  // Create the point that we will pan to. Assure point exist in this map
  const auto point = Point(event_params.at(__POSX__).asInt(), event_params.at(__POSY__).asInt());
  CCASSERT(tmx_object_ui_->containsPoint(point, player_->getCurrentTmxLayer()),
           "ControlsGameScene: ui map does not contain this point...");
  const auto location = tmx_object_ui_->getPosition(point, player_->getCurrentTmxLayer());

  // Disable
  player_->setCanMove(false);
  main_layer_->stopAllActions();
  swipe_gesture_->resetSwipeDirection();
  getEventDispatcher()->setEnabled(false);

  // Function called when the to is available
  const auto to_funciton = CallFunc::create([&, location]() {
    if (name == __CONTROLS_EVENT_EXIT__) {
      main_layer_->addChild(ParticleUtils::load(__PARTICLE_EXIT__, location.x, location.y, true), 10,
                            __EXIT__);
    }

    if (name == __CONTROLS_EVENT_BEWARE__) {
      auto x = BaseLlurker::createLlurker(this, XIMOIS, __CONTROLS_LAYER_TWO__);
      x->setCurrentPoint(Point(21, 18));
      x->stop();
      x->setPosition(tmx_object_ui_->getPosition(x->getCurrentPoint(), x->getCurrentTmxLayer()));
      llurkers_.emplace_back(x);
      main_layer_->addChild(x, 10);

      auto x2 = BaseLlurker::createLlurker(this, XIMOIS, __CONTROLS_LAYER_TWO__);
      x2->setCurrentPoint(Point(23, 18));
      x2->stop();
      x2->setPosition(tmx_object_ui_->getPosition(x2->getCurrentPoint(), x->getCurrentTmxLayer()));
      llurkers_.emplace_back(x2);
      main_layer_->addChild(x2, 11);

      x->runAction(
          ActionUtils::sequence({DelayTime::create(0.3f), CallFunc::create([=]() { x->resetSystem(); })}));
      x2->runAction(
          ActionUtils::sequence({DelayTime::create(0.7f), CallFunc::create([=]() { x2->resetSystem(); })}));
    }
  });

  // Pan to this point and disable action of the player
  const auto back_function = CallFunc::create([&]() {
    getEventDispatcher()->setEnabled(true);
    main_layer_->runAction(Follow::create(player_));
    player_->setCanMove(true);
  });

  // Pan to this point and disable action of the player
  pan(location, *to_funciton, *back_function, 1.35f, 3.5f, 2.5f);
}

void ControlsGameScene::initializeReverseSwipeCheckbox() {
  reverse_swipe_checkbox_ = CheckBox::create(
      __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE__, __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE_CHECKED__,
      __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE_CHECKED__, __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE__,
      __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE__, Widget::TextureResType::PLIST);
  reverse_swipe_checkbox_->setPosition(Globals::getScreenPosition(6, 1.15f));
  reverse_swipe_checkbox_->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

  // Adding an event listen to check box for when it's click.
  reverse_swipe_checkbox_->addEventListener([&](Ref*, const CheckBox::EventType type) -> void {
    ThreadUtils::runOnAsyncThread([=]() {
      switch (type) {
        case CheckBox::EventType::SELECTED:
          DataUtils::saveOtherData(__KEY_REVERSE_SWIPE__, 1);
          CCLOG("ControlsGameScene: Reverse Swipe enabled...");
          break;
        case CheckBox::EventType::UNSELECTED:
          DataUtils::saveOtherData(__KEY_REVERSE_SWIPE__, 0);
          CCLOG("ControlsGameScene: Reverse Swipe disabled...");
          break;
      }

      // Update the swipe gesture.
      swipe_gesture_->updateReverseSwipe();
    });
  });

  // The player may be restarting the tutorial since they failed. get the previous preference and set it
  if (DataUtils::getOtherData(__KEY_REVERSE_SWIPE__, 0)) reverse_swipe_checkbox_->setSelected(true);

  // Add check box
  ui_layer_->addChild(reverse_swipe_checkbox_, 2, __CONTROLS_ASSET_CHECKBOX_REVERSE_SWIPE__);
}

ValueMap ControlsGameScene::generateParams() {
  // Add default params
  ValueMap params;
  params.emplace(__SCRIPT__, __CONTROLS_SCENE__);
  params.emplace(__MODE__, CONTROLSM);
  params.emplace(__NAME__, typeid(ControlsGameScene).name());

  // Return default values
  return params;
}
