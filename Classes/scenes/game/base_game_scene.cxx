// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_game_scene.hxx"
#include "engines/tmx/tmx_engine.hxx"
#include "entities/llumas/base_lluma.hxx"
#include "entities/llurkers/base_llurker.hxx"

BaseGameScene::BaseGameScene(const ValueMap& params, BackgroundLayer* bg) : BaseScene(params, bg) {}

BaseGameScene::~BaseGameScene() = default;

void BaseGameScene::beforeInitialized() {
  
}

bool BaseGameScene::onInitialized() {
  // The `map_` variable should be initialized to BaseGameScene::onInitialized() being called.
  // Check to validate that is true
  CCASSERT(map_ not_eq nullptr,
           "BaseGameScene: TiledMap must be initialize before BaseGameScene::onInitialized() is called...");

  // Setup Physics on the scene.
  getPhysicsWorld()->setGravity(Vec2::ZERO);
#ifdef COCOS2D_DEBUG
  getPhysicsWorld()->setDebugDrawMask(0xffff);
#endif

  // Initialize loading layer.
  loading_layer_ = LayerColor::create(Color4B::BLACK);
  loading_layer_->setCascadeOpacityEnabled(true);
  addChild(loading_layer_, 4);

  // Initialize UI layer.
  ui_layer_ = Layer::create();
  ui_layer_->setCascadeOpacityEnabled(true);
  addChild(ui_layer_, 5);

  // Initializes swipe detection.
  swipe_gesture_ = new (nothrow) SwipeGesture();
  swipe_gesture_->setEnabled(false);
  addChild(swipe_gesture_);

  // Initializations.
  initializeUi();
  initializeMenu();
  initializeText();
  initializePlayer();
  initializeCollisionDetection();

  // Main layer will allows follow the playing initially.
  auto follow_action = Follow::create(player_);
  follow_action->setTag(__FOLLOW_TAG__);
  main_layer_->runAction(follow_action);

  // Base initialization successful.
  return true;
}

void BaseGameScene::update(const float delta) {
  // Continue to update the BaseScene.
  BaseScene::update(delta);

  if (player_ == nullptr) return;
  player_->updateEntity();
  for_each(llurkers_.begin(), llurkers_.end(), [](BaseLlurker* llurker) { llurker->updateEntity(); });
}

void BaseGameScene::initializeMap(const string& path, const string& name) {
  // Initialize the tile map and all of the things with it.
  const auto tmx = string(path + name + ".tmx");
  map_ = TMXTiledMap::createWithXML(ArchiveUtils::loadString(tmx), string());

  // Set up TileMap setting
  map_->setName(tmx);
  map_->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  map_->setPosition(Globals::getScreenPosition());
  map_->setCascadeOpacityEnabled(true);

  // Invoke the TmxEngine to create a TmxObject for the map
  TmxEngine::getInstance()->cache(map_, Modes(params_->at(__MODE__).asInt()));
  main_layer_->addChild(map_, 0);
}

void BaseGameScene::initializeCollisionDetection() {
  auto contact_listener = EventListenerPhysicsContact::create();
  contact_listener->onContactBegin = [=](PhysicsContact& contact) -> bool {
    const auto a = contact.getShapeA()->getBody();
    const auto b = contact.getShapeB()->getBody();

    // If the collision mask is the same, that means either the player isn't affected by llurkers at the
    // moment or 2 llurkers collided which should do nothing
    if (a->getCollisionBitmask() == b->getCollisionBitmask()) return true;

    // If either of the collisionMasks are 1, This means that the llurker has touched a lluma. The lluma
    // should notify what to do in this situation.
    if (a->getCollisionBitmask() == 1 or b->getCollisionBitmask() == 1) {
      onCollision();
      return true;
    }

    return true;
  };

  // Add collision to the main layer
  main_layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact_listener, this);
}

void BaseGameScene::pan(const Vec2& to, CallFunc& to_function, CallFunc& back_function,
                        const float to_duration, const float to_rate, const float delay,
                        const float back_duration, const float back_rate) const {
  // Create action to pan to the `to` location
  const auto pan_to = MoveTo::create(to_duration, -convertToNodeSpaceAR(to));

  // Create action to pan to back from the `to` position.
  const auto pan_back = MoveTo::create(back_duration, -convertToNodeSpaceAR(player_->getPosition()));

  // Create complex action
  const auto pan_action = ActionUtils::sequence({
      EaseInOut::create(pan_to, to_rate),
      &to_function,
      DelayTime::create(delay),
      EaseInOut::create(pan_back, back_rate),
      &back_function,
  });

  // Run this action on the main layer
  main_layer_->runAction(pan_action);
}
