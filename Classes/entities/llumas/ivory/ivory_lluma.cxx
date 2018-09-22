// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "ivory_lluma.hxx"

Ivory::Ivory(BaseGameScene* scene, SwipeGesture* gesture, const string& id, const string& tmx_layer)
    : BaseLluma(*scene, *gesture, Ivory::generateStats(id), tmx_layer) {}

Ivory::~Ivory() = default;

ValueMap Ivory::generateStats(const string& id) {
  ValueMap stats;
  stats.emplace(__ENTITY_ID__, id);
  stats.emplace(__ENTITY_FILE__, __IVORY_ENTITY_FILE__);
  stats.emplace(__ENTITY_SPEED__, 6);
  return stats;
}
