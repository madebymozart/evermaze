// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_scene.hxx"
#include "utils/archive/archive_utils.hxx"

BaseScene::BaseScene(const ValueMap& params, BackgroundLayer* bg)
    : sprite_manager_(nullptr),
      menu_manager_(nullptr),
      text_manager_(nullptr),
      params_(make_unique<ValueMap>(params)),
      script_(make_unique<ValueMap>(ArchiveUtils::loadValueMap(params_->at(__SCRIPT__).asString()))),
      main_layer_(nullptr),
      overlay_layer_(nullptr),
      background_layer_(bg) {
  // Set low level node attributes
  Node::setOpacity(0);
  Node::setCascadeOpacityEnabled(true);
  Node::setAnchorPoint(Vec2::ANCHOR_MIDDLE);

  // Immediately schedule the update function
  scheduleUpdate();
}

BaseScene::~BaseScene() = default;

void BaseScene::update(const float /* interval */) {
  // Update the audio engine instance
  AudioUtils::getInstance()->update();
}

void BaseScene::onEnter() {
  // Set the background layer
  if (background_layer_ not_eq nullptr) {
    // Remove the background from it's previous scene
    if (background_layer_->getParent() != nullptr) {
      background_layer_->removeFromParentAndCleanup(false);
    }

    // Add it this this scene and reschedule the scroll pattern.
    addChild(background_layer_, 0);
  }

  // Set as an auto release scene
  autorelease();
  Scene::onEnter();
}

void BaseScene::onExit() {
  // Unload all scripts and params
  __CCLOGWITHFUNCTION("Unloading Scene: %s", getName().c_str());
  script_.reset();
  params_.reset();

  // Unload all managers.
  sprite_manager_.reset();
  text_manager_.reset();
  menu_manager_.reset();

  // Remove everything from the main layer
  main_layer_->removeAllChildren();

  // Remove everything from the scene
  removeAllChildren();

  // Unload all unused textures.
  Director::getInstance()->purgeCachedData();

  // Call Base
  Scene::onExit();
}

void BaseScene::initialize() {
  // Initialize the parent class `Scene::init()`
  CCASSERT(Scene::init(), "failed to init scene...");

  // Add the main layer to this scene
  main_layer_ = Layer::create();
  main_layer_->setCascadeOpacityEnabled(true);
  addChild(main_layer_, 1, __MAIN__);

  // Add the overlay layer to this scene
  overlay_layer_ = LayerColor::create(Color4B::BLACK);
  overlay_layer_->setCascadeOpacityEnabled(true);
  overlay_layer_->setOpacity(0);
  main_layer_->addChild(overlay_layer_, -1, __OVERLAY__);

  // Every scene needs a texture and pack associated with it. Crash if this texture isn't found
  CCASSERT(script_->find(__TEXTURE__) != script_->end(), "No texture found");
  CCASSERT(script_->find(__PACK__) != script_->end(), "No pack found");
  const auto& texture = script_->at(__TEXTURE__).asString();

  // Called before initialize so inhertied all children to have a chance to do somthing before the
  // initialization.
  beforeInitialized();

  // Load the texture in async
  Director::getInstance()->getTextureCache()->addImageAsync(texture, [=](Texture2D* loaded) {
    loaded->setAntiAliasTexParameters();

    // Cache the sprite frames using the async loaded textures
    auto plist = ArchiveUtils::loadValueMap(script_->at(__PACK__).asString());
    SpriteFrameCache::getInstance()->addSpriteFramesWithDictionary(plist, loaded);

    // Initialize sprites.
    if (script_->find(__SPRITES__) != script_->end())
      sprite_manager_.reset(new (nothrow)
                                SpriteManager(script_->at(__SPRITES__).asValueVector(), *main_layer_));

    // Initialize menu.
    if (script_->find(__MENU__) != script_->end())
      menu_manager_.reset(new (nothrow) MenuManager(script_->at(__MENU__).asValueMap(), *main_layer_));

    // Initialize text.
    if (script_->find(__TEXT__) != script_->end())
      text_manager_.reset(new (nothrow) TextManager(script_->at(__TEXT__).asValueMap(), *main_layer_));

    // Fade in if the scene's opacity is 0
    if (getOpacity() == 0) runAction(ActionUtils::fadeIn(0, 0.3f));

    // Initialize is complete and now call on initialized from the child class
    onInitialized();
  });
}
