// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_lluma.hxx"
#include "engines/tmx/tmx_engine.hxx"
#include "ivory/ivory_lluma.hxx"

BaseLluma::BaseLluma(BaseGameScene& scene, SwipeGesture& gesture, ValueMap params, const string& tmx_layer)
    : BaseEntity(&scene, move(params), tmx_layer),
      precision_(__FRAME_SKIPPING__),
      override_swipe_(false),
      gesture_(gesture) {
  // Set up the llurkers physics body
  auto physics = PhysicsBody::createCircle(ParticleSystem::getTexture()->getContentSize().width / 5,
                                           PhysicsMaterial(1, 1, 1));

  // The collision bit mask of the llurker will always
  physics->setCollisionBitmask(1);
  physics->setContactTestBitmask(2);

  // Assure llumas don't bump into each other
  physics->setGroup(1);
  setPhysicsBody(physics);
}

BaseLluma::~BaseLluma() = default;

BaseLluma* BaseLluma::createLluma(BaseGameScene* scene, SwipeGesture* gesture, const string& id,
                                  const string& tmx_layer) {
  return new Ivory(scene, gesture, id, tmx_layer);
  // throw runtime_error("No Lluma can be created with this id");
}

void BaseLluma::updateEntity() {
  // This is used to stop the lluma from moving at anytime and at any position. much more granular
  // than waiting for movement_ to reach zero.
  if (not can_move_) return;

  const auto& dir = gesture_.swipe_direction_;
  const auto override = direction_ not_eq dir;

  if (dir not_eq NONE) {
    if (override and movement_ not_eq decimal_cast<3>(0) and validateSwipeIsOpposite(dir) and
        not override_swipe_) {
      override_swipe_ = true;

      // Update movement and direction
      movement_ = decimal_cast<3>(108) - movement_;
      direction_ = dir;
    } else if (override and movement_ not_eq decimal_cast<3>(0) and considerTolerance(dir) and
               not override_swipe_) {
      override_swipe_ = true;

      // Update movement and direction
      movement_ = decimal_cast<3>(108) - movement_;
      direction_ = getOppositeSwipeDir(direction_);

      // Disable the swipe gesture so the lluma can move on it's own
      gesture_.is_touch_down_ = false;
      gesture_.setEnabled(false);
    } else if (movement_ <= decimal_cast<3>(0)) {
      override_swipe_ = false;

      // We need to round the players position because it may become off do to double not allows being
      // accurate when given certain numbers
      this->setPosition(round(this->getPositionX()), round(this->getPositionY()));

      // If the swipe_precision has been correctly set then update the swipes to be enabled
      if (precision_ == __FRAME_SKIPPING__) gesture_.setEnabled(true);

      // Update the paths for the current position for the player.
      updatePaths();

      // if this returns false, this means we may be frame skipping at the moment in which we don't
      // want to reset the movement variable
      if (updateDirections(dir)) movement_ = movement_ + decimal_cast<3>(108);
    }

    // Update the position as long as we are not zero
    if (movement_ not_eq decimal_cast<3>(0)) updatePosition();
  }
}

void BaseLluma::updatePaths() {
  // Getting the current point and trying to see what directions are available
  current_point_ = tiles_.getPoint(getPosition(), current_tmx_layer_);

  const auto empty = make_pair(Point(-1, -1), Vec2(-1, -1));
  const auto end = tiles_.getPointPair(Point(-1, -1), current_tmx_layer_);

  // Create new point for all possible directions
  const auto up =
      tiles_.getPointPair(Point(getCurrentPoint().x, getCurrentPoint().y - 1), current_tmx_layer_);
  const auto down =
      tiles_.getPointPair(Point(getCurrentPoint().x, getCurrentPoint().y + 1), current_tmx_layer_);
  const auto left =
      tiles_.getPointPair(Point(getCurrentPoint().x - 1, getCurrentPoint().y), current_tmx_layer_);
  const auto right =
      tiles_.getPointPair(Point(getCurrentPoint().x + 1, getCurrentPoint().y), current_tmx_layer_);

  // clear out vector to add the following points.
  fill_n(paths_, 4, empty);
  paths_[0] = up not_eq end ? make_pair(up->first, up->second) : empty;
  paths_[1] = down not_eq end ? make_pair(down->first, down->second) : empty;
  paths_[2] = left not_eq end ? make_pair(left->first, left->second) : empty;
  paths_[3] = right not_eq end ? make_pair(right->first, right->second) : empty;
}

bool BaseLluma::updateDirections(const Directions direction) {
  const auto empty = make_pair(Point(-1, -1), Vec2(-1, -1));
  auto valid_tiles = 0;

  // Check if is worth doing this in the first place
  for (const auto& path : paths_)
    if (path.first not_eq empty.first) valid_tiles++;
  if (valid_tiles >= 3 and gesture_.is_touch_down_ and gesture_.swipe_direction_ == direction_ and
      precision_ > 0) {
    precision_--;
    return false;
  }

  // Reset the precision since the evaluation above return false
  precision_ = __FRAME_SKIPPING__;

  // Update the direction
  Point point;
  if (direction == UP)
    point = paths_[0].first;
  else if (direction == DOWN)
    point = paths_[1].first;
  else if (direction == LEFT)
    point = paths_[2].first;
  else if (direction == RIGHT)
    point = paths_[3].first;

  // Checking to see which direction should be set.
  if (point not_eq empty.first)
    direction_ = direction;
  else if (direction_ == direction)
    direction_ = NONE;
  else
    updateDirections(direction_);

  return true;
}

void BaseLluma::updatePosition() {
  // This means the lluma is in the center of a tile. Game
  if (movement_ == decimal_cast<3>(108)) {
    if (not game_scene_.logic()) return;

    // This is used to change the speed live.
    if (speed_changer_ not_eq decimal_cast<3>(0)) {
      entitiy_speed_ = speed_changer_;
      speed_changer_ = decimal_cast<3>(0);
    }
  }

  // update the movement variable
  movement_ = movement_ - entitiy_speed_;
  if (direction_ == NONE) {
    movement_ = decimal_cast<3>(0);
    return;
  }

  // Allows the timer to start

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

bool BaseLluma::validateSwipeIsOpposite(const Directions swipe) const {
  if (direction_ == NONE) return false;

  // Validate the opposite direction of the swipe
  if (direction_ == UP) return swipe == DOWN;
  if (direction_ == DOWN) return swipe == UP;
  if (direction_ == LEFT) return swipe == RIGHT;
  if (direction_ == RIGHT) return swipe == LEFT;

  // False return. This should never happen
  return false;
}

Directions BaseLluma::getOppositeSwipeDir(const Directions swipe) {
  if (swipe == NONE) return NONE;

  // Return the opposite direction of the swipe
  if (swipe == UP) return DOWN;
  if (swipe == DOWN) return UP;
  if (swipe == LEFT) return RIGHT;
  if (swipe == RIGHT) return LEFT;

  // Empty return. This should never happen
  return {};
}

bool BaseLluma::considerTolerance(const Directions swipe) const {
  // check conditions
  if (movement_ < decimal_cast<3>(108 - entitiy_speed_.getAsDouble() * 6)) return false;

  // Calculate the point in which we are trying to create toleration for
  Point point;
  if (swipe == UP)
    point = Point(current_point_.x, current_point_.y - 1);
  else if (swipe == DOWN)
    point = Point(current_point_.x, current_point_.y + 1);
  else if (swipe == LEFT)
    point = Point(current_point_.x - 1, current_point_.y);
  else if (swipe == RIGHT)
    point = Point(current_point_.x + 1, current_point_.y);

  // Assure that point is in the map. if it isn't the it is false
  return tiles_.containsPoint(point, current_tmx_layer_);
}
