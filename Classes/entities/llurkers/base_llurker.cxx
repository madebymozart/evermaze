// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_llurker.hxx"
#include "entities/llumas/base_lluma.hxx"
#include "entities/llurkers/ximois/ximois_llurker.hxx"
#include "utils/path/path_utils.hxx"

BaseLlurker::BaseLlurker(BaseGameScene& scene, ValueMap params, const string& tmx_layer)
    : BaseEntity(&scene, move(params), tmx_layer),
      chase_(false),
      generate_index_(stats_.at(__BASE_LLURKER_GENERATOR_INDEX__).asInt()),
      gen_(random_device{}()) {
  // Set up the llurkers physics body
  auto physics = PhysicsBody::createCircle(ParticleSystem::getTexture()->getContentSize().width / 5,
                                           PhysicsMaterial(1, 1, 1));

  // The collision bit mask of the llurker will always
  physics->setCollisionBitmask(2);
  physics->setContactTestBitmask(2);

  // Assure llurkers don't bump into each other
  physics->setGroup(-1);
  setPhysicsBody(physics);
}

BaseLlurker::~BaseLlurker() = default;

BaseLlurker* BaseLlurker::createLlurker(BaseGameScene* scene, const Llurkers llurker,
                                        const string& tmx_layer) {
  if (llurker == XIMOIS) return new Ximois(scene, tmx_layer);
  // if (llurker == CERBERUS) return new Cerberus(layer);
  // if (llurker == ADDROSS) return new Addross(layer);
  return nullptr;
}

void BaseLlurker::updateEntity() {
  // This is used to stop the llurker from moving at anytime and at any position. much more granular than
  // waiting for movement_ to reach zero
  if (not can_move_ or not game_scene_.player_->getCanMove() or current_point_ == Point(-1, -1)) return;

  // If our path_queue_ has nothing in it, the llurker cannot move until it contains values.
  if (path_queue_.empty()) updatePathQueue();

  if (movement_ == decimal_cast<3>(0)) {
    // We need to round the players position because it may become off do to double not allows being accurate
    // when given certain numbers
    setPosition(round(getPositionX()), round(getPositionY()));

    // If the llurker is close enough to the place, the will start to "chase the player by constantly the
    // distance to the player
    const auto distance = Heuristic::manhattan(current_point_, game_scene_.player_->getCurrentPoint());
    const auto chase_index = stats_.at(__BASE_LLURKER_CHASE_DISTANCE__).asInt();

    if (distance <= chase_index and not chase_) {
      __CCLOGWITHFUNCTION("Initializing Chase...");
      chase_ = true;
      updatePathQueue();

      // Make the llurker look like it it enraged
      ParticleSystem::setSpeedVar(50);
    }

    if (distance > chase_index and chase_) {
      __CCLOGWITHFUNCTION("To Far: Going to another node");
      chase_ = false;
      updatePathQueue();

      // Make the llurker look like it it enraged
      if (generate_index_ < 100) ParticleSystem::setSpeedVar(0);
    }

    // Reset movement to the tile size
    current_point_ = tiles_.getPoint(getPosition(), current_tmx_layer_);
    if (not path_queue_.empty()) {
      updateDirections();
      path_queue_.pop_front();
      movement_ = decimal_cast<3>(108);
    }
  }

  // As long as the movement is not 0, then we can move. This is one of the keys to butter smooth moving of
  // llurkers
  if (movement_ not_eq decimal<3>(0)) updatePosition();
}

void BaseLlurker::updatePosition() {
  // This means the lluma is in the center of a tile. Gam
  if (movement_ == decimal_cast<3>(108)) {
    // This is used to change the speed live.
    if (speed_changer_ not_eq decimal_cast<3>(0)) {
      entitiy_speed_ = speed_changer_;
      speed_changer_ = decimal_cast<3>(0);
    }
  }

  if (direction_ == NONE) {
    movement_ = decimal_cast<3>(0);
    updateEntity();
    return;
  }

  // Update the movement variable
  movement_ = movement_ - entitiy_speed_;

  // Getting position reference to move the particles
  const auto& x = getPositionX();
  const auto& y = getPositionY();

  // Move based on direction
  if (direction_ == UP)
    setPositionY(static_cast<float>(y + entitiy_speed_.getAsDouble()));
  else if (direction_ == DOWN)
    setPositionY(static_cast<float>(y - entitiy_speed_.getAsDouble()));
  else if (direction_ == LEFT)
    setPositionX(static_cast<float>(x - entitiy_speed_.getAsDouble()));
  else if (direction_ == RIGHT)
    setPositionX(static_cast<float>(x + entitiy_speed_.getAsDouble()));
}

void BaseLlurker::updatePathQueue() {
  // Clear path_queue.
  path_queue_.clear();

  // Call implementation of onCalculate with the player position
  onCalculate(current_point_, game_scene_.player_->getCurrentPoint());

  // Always pop the first of the queue since it will be the current positiotn of the entitiy
  path_queue_.pop_front();
}

void BaseLlurker::updateDirections() {
  // Getting the current point and trying to see what directions are available
  const auto up = Point(current_point_.x, current_point_.y - 1);
  const auto down = Point(current_point_.x, current_point_.y + 1);
  const auto left = Point(current_point_.x - 1, current_point_.y);
  const auto right = Point(current_point_.x + 1, current_point_.y);

  // Update the direction
  if (path_queue_.front() == up)
    direction_ = UP;
  else if (path_queue_.front() == down)
    direction_ = DOWN;
  else if (path_queue_.front() == left)
    direction_ = LEFT;
  else if (path_queue_.front() == right)
    direction_ = RIGHT;
  else
    direction_ = NONE;
}
