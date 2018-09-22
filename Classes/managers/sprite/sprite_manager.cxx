// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "sprite_manager.hxx"
#include "utils/archive/archive_utils.hxx"

SpriteManager::SpriteManager(const ValueVector& info, Layer& layer) : BaseManager(layer) {
  // Load all sprites into the node map
  for_each(info.begin(), info.end(), [&](const auto& data) { addSpriteToNodeMap(data.asValueMap()); });
}

SpriteManager::~SpriteManager() = default;

Sprite* SpriteManager::findSprite(const string& key) { return dynamic_cast<Sprite*>(findNode(key)); }

void SpriteManager::addSpriteToNodeMap(const ValueMap& props) {
  // Create a sprite object by passing a sprite frame object into a sprite constructor
  const auto& name = props.at(__FILE_NAME__).asString();
  auto sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
  sprite->setCascadeOpacityEnabled(true);

  // Set required sprite properties from the properties value map.
  sprite->setName(name);
  sprite->setPosition(Globals::getScreenPosition(props.at(__POSX__).asFloat(), props.at(__POSY__).asFloat()));
  sprite->setLocalZOrder(props.at(__ZINDEX__).asInt());

  // Set optional sprite properties from the properties value map.
  if (props.find(__ACP_X__) != props.end() and props.find(__ACP_Y__) != props.end())
    sprite->setAnchorPoint(Vec2(props.at(__ACP_X__).asFloat(), props.at(__ACP_Y__).asFloat()));

  if (props.find(__SCALE__) != props.end()) sprite->setScale(props.at(__SCALE__).asFloat());
  if (props.find(__OPACITY__) != props.end()) sprite->setOpacity(GLubyte(props.at(__OPACITY__).asInt()));

  // Auto add to layer if set
  if (props.find(__ADD_TO_LAYER__) not_eq props.end() and props.find(__ADD_TO_LAYER__)->second.asBool())
    main_layer_.addChild(sprite);

  // Create a new node object and add to the sprites map.
  node_map_.insert(name, sprite);

  // If the props has an action then run it on the sprite
  addActionFromProps(sprite, props);
}
