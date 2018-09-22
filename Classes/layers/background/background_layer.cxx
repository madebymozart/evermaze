// Copyright (c) 2016 Mozart Alexander Louis. All rights reserved.

// Includes
#include "background_layer.hxx"
#include "utils/archive/archive_utils.hxx"
#include "utils/data/data_utils.hxx"

BackgroundLayer::BackgroundLayer() : scroll_fix_(false), speed_(2.0f) {
  // This background need to stay alive at all times
  retain();

  // Initializing color data
  const auto colors = ArchiveUtils::loadValueVector(__COLOR_CONFIG__);
  for_each(colors.begin(), colors.end(), [this](const Value& data) {
    const auto& map = data.asValueMap();

    // Run assertions on required fields
    CCASSERT(map.find(__ID__) not_eq map.end(), "No `id` field in map (required)...");
    CCASSERT(map.find(__COLOR__) not_eq map.end(), "No `colors` field in map (required)...");
    CCASSERT(map.at(__COLOR__).asValueVector().size() == 3, "More/Less than 3 in colors array...");

    // Populate
    color_config_.emplace(map.at(__ID__).asString(), map.at(__COLOR__).asValueVector());
  });

  // Initialize pattern data
  const auto patterns = ArchiveUtils::loadValueVector(__PATTERN_CONFIG__);
  for_each(patterns.begin(), patterns.end(), [this](const Value& data) {
    const auto& map = data.asValueMap();

    // Run assertions on required fields
    CCASSERT(map.find(__ID__) not_eq map.end(), "No `id` field in map (required)...");
    CCASSERT(map.find(__TEXTURE__) not_eq map.end(), "No `texture` field in map (required)...");

    // Populate
    pattern_config_.emplace(map.at(__ID__).asString(), map);
  });
}

BackgroundLayer::~BackgroundLayer() {
  color_config_.clear();
  pattern_config_.clear();
  unscheduleScrollPattern();
}

bool BackgroundLayer::init() {
  CCASSERT(Layer::init(), "Failed to initialize BackgroundLayer");

  // Set initial color and pattern on the background layer. This will check the local
  // data base for which is set.
  setBackgroundColor();
  setBackgroundPattern();

  // The pattern will continually scroll throughout the entire game.
  scheduleScrollPattern();
  return true;
}

void BackgroundLayer::setBackgroundColor(const string& preview) {
  string current_color;
  if (not preview.empty())
    current_color = preview;
  else
    current_color = DataUtils::getOtherData(__KEY_CURRENT_COLOR__, __DEFAULT_COLOR__);

  // Retrive the colors from the color data
  const auto end_color = Color3B(GLubyte(color_config_.at(current_color).at(0).asInt()),
                                 GLubyte(color_config_.at(current_color).at(1).asInt()),
                                 GLubyte(color_config_.at(current_color).at(2).asInt()));

  if (not getChildByName(__BG_COLOR_TAG__)) {
    // Create and add the color gradient to the background.
    const auto overlay_gradient = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 0, 0));
    const auto color_layer = LayerColor::create(Color4B(end_color));

    addChild(color_layer, 0, __BG_COLOR_TAG__);
    addChild(overlay_gradient, 1);

    return;
  }

  // Update the color gradient to the background.
  const auto& color_layer = dynamic_cast<LayerColor*>(getChildByName(__BG_COLOR_TAG__));
  color_layer->runAction(TintTo::create(0.25f, end_color.r, end_color.g, end_color.b));
}

void BackgroundLayer::setBackgroundPattern(const string& preview) {
  string current_pattern;
  if (not preview.empty())
    current_pattern = preview;
  else
    current_pattern = DataUtils::getOtherData(__KEY_CURRENT_PATTERN__, __DEFAULT_PATTERN__);

  // Get current pattern data
  const auto& data = pattern_config_.at(current_pattern);
  const auto& pattern_texture = data.at(__TEXTURE__).asString();

  // Check if this is the current texture in the stack. We should do nothing if it is.
  if (Director::getInstance()->getTextureCache()->getTextureForKey(pattern_texture) != nullptr) return;

  // Async load the pattern image into cache
  Director::getInstance()->getTextureCache()->addImageAsync(pattern_texture, [=](Texture2D* texture) {
    // remove a currently running pattern.
    if (getChildByName(__BG_PATTERN_ONE_TAG__) not_eq nullptr or
        getChildByName(__BG_PATTERN_TWO_TAG__) not_eq nullptr) {
      scroll_fix_ = false;
      unscheduleScrollPattern();
      removeChildByName(__BG_PATTERN_ONE_TAG__);
      removeChildByName(__BG_PATTERN_TWO_TAG__);

      // Unload all unused textures.
      Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }

    // Create new patterns with the loaded texture
    auto pattern_one = Sprite::createWithTexture(texture);
    pattern_one->setPosition(Globals::getScreenPosition(1, 1136));
    pattern_one->setAnchorPoint(Vec2::ZERO);
    addChild(pattern_one, 1, __BG_PATTERN_ONE_TAG__);

    if (data.find(__SPIN__) not_eq data.end() and data.at(__SPIN__).asBool()) {
      pattern_one->setScale(1.50f);
      pattern_one->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
      pattern_one->setPosition(Globals::getScreenPosition());
      pattern_one->runAction(RepeatForever::create(RotateBy::create(10, 360)));
      return;
    }

    auto pattern_two = Sprite::createWithTexture(texture);
    pattern_two->setPosition(Globals::getScreenPosition(768, 1136));
    pattern_two->setAnchorPoint(Vec2::ZERO);
    addChild(pattern_two, 1, __BG_PATTERN_TWO_TAG__);

    // Setting optional fields
    if (data.find(__SPEED__) not_eq data.end()) speed_ = data.at(__SPEED__).asFloat();
    if (data.find(__FLIP__) != data.end()) pattern_two->setFlippedX(data.at(__FLIP__).asBool());

    // Reschedule pattern scrolling
    scheduleScrollPattern();
  });
}

void BackgroundLayer::scheduleScrollPattern() {
  this->schedule(schedule_selector(BackgroundLayer::scrollPattern));
}

void BackgroundLayer::unscheduleScrollPattern() {
  this->unschedule(schedule_selector(BackgroundLayer::scrollPattern));
}

void BackgroundLayer::scrollPattern(float dt) {
  if (getChildByName(__BG_PATTERN_ONE_TAG__) not_eq nullptr and
      getChildByName(__BG_PATTERN_TWO_TAG__) not_eq nullptr) {
    const auto& width = Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width;
    const auto& pattern_one = getChildByName(__BG_PATTERN_ONE_TAG__);
    const auto& pattern_two = getChildByName(__BG_PATTERN_TWO_TAG__);

    auto pos1 = pattern_one->getPosition();
    auto pos2 = pattern_two->getPosition();
    pos1.x -= speed_;
    pos2.x -= speed_;

    // A quick a fix to keep infinite scrolling working on all screen sizes.
    if (!scroll_fix_) {
      scroll_fix_ = true;
      if (pos1.x != pos2.x + width) {
        scroll_fix_ = false;
        pos1.x = pos2.x + width;
      }
    }

    if (pos1.x <= -Globals::getVisibleSize().width + (Globals::getVisibleSize().width - width))
      pos1.x = pos2.x + Globals::getVisibleSize().width + (width - Globals::getVisibleSize().width);
    if (pos2.x <= -Globals::getVisibleSize().width + (Globals::getVisibleSize().width - width))
      pos2.x = pos1.x + Globals::getVisibleSize().width + (width - Globals::getVisibleSize().width);

    pattern_one->setPosition(pos1);
    pattern_two->setPosition(pos2);
  }
}
