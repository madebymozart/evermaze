// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "ximois_llurker.hxx"
#include "scenes/game/controls/controls_game_scene.hxx"
#include "utils/path/path_utils.hxx"

Ximois::Ximois(BaseGameScene* scene, const string& tmx_layer)
    : BaseLlurker(*scene, Ximois::generateStats(), tmx_layer) {
  if (game_scene_.getName() == typeid(ControlsGameScene).name()) {
    generate_index_ = 999;
    ParticleSystem::setSpeedVar(50);
  }
}

Ximois::~Ximois() = default;

ValueMap Ximois::generateStats() {
  ValueMap stats;
  stats[__ENTITY_ID__] = __XIMOIS__;
  stats[__ENTITY_FILE__] = __XIMOIS_ENTITY_FILE__;
  stats[__ENTITY_SPEED__] = 4;
  stats[__BASE_LLURKER_CHASE_DISTANCE__] = 5;
  stats[__BASE_LLURKER_GENERATOR_INDEX__] = 5;
  return stats;
}

void Ximois::onCalculate(const Point from, const Point to) {
  // Creating a path that will be calculated
  auto target = to;

  // If the queue is empty, then we need to generate a random point for the llurker to go to if it isn't
  // already to close to the player. While in chase mode, the generator index decreases
  if (not chase_) {
    // Generate a random number between 1 - 100. If the llurkers generate index is higher than the random
    // number, the llurker will go straight to the last known place of the player.
    uniform_int_distribution<> d(1, 100);
    if (d(gen_) < generate_index_)
      CCLOG("Navigating directly to player Index: %i", int(generate_index_));
    else
      target = tiles_.getRandomPoint("spawn")->first;

    // Increase generated index
    generate_index_ += 4;
  }

  path_utils_.findPath(from, target, path_queue_, chase_ ? 3 : 50);
}
