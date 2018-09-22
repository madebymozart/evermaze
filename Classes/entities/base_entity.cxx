// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_entity.hxx"
#include "engines/tmx/tmx_engine.hxx"

BaseEntity::BaseEntity(BaseGameScene* scene, ValueMap params, const string& tmx_layer)
    : direction_(NONE),
      game_scene_(*scene),
      stats_(std::move(params)),
      current_point_(Point(-1, -1)),
      movement_(decimal<3>(0)),
      speed_changer_(decimal<3>(0)),
      can_move_(true),
      tiles_(*TmxEngine::getInstance()->getTmxObject(game_scene_.map_->getName())) {
  // Initialize the entity and set parameters
  auto data = ArchiveUtils::loadValueMap(stats_.at(__ENTITY_FILE__).asString());
  initWithDictionary(data);
  setPositionType(PositionType::RELATIVE);
  ParticleSystemQuad::setName(stats_.at(__ENTITY_ID__).asString());
  ParticleSystemQuad::setCascadeOpacityEnabled(true);
  ParticleSystemQuad::setOpacityModifyRGB(true);

  // Set entities speed
  entitiy_speed_ = stats_.at(__ENTITY_SPEED__).asDouble();

  // Initialize the PathUtils pointer
  path_utils_.setWorldSize(game_scene_.map_->getMapSize());

  // Set the current layer that enitiy should be observing
  setCurrentTmxLayer(tmx_layer);
}

BaseEntity::~BaseEntity() = default;

void BaseEntity::setCurrentPoint(const Point& point) { current_point_ = point; }

Point BaseEntity::getCurrentPoint() const { return current_point_; }

void BaseEntity::setCanMove(const bool move) { can_move_.store(move); }

bool BaseEntity::getCanMove() const { return can_move_.load(memory_order_acquire); }

void BaseEntity::setCurrentTmxLayer(const string& layer) {
  current_tmx_layer_ = layer;
  path_utils_.addCollisionList(tiles_, current_tmx_layer_);
}

string BaseEntity::getCurrentTmxLayer() const { return current_tmx_layer_; }
