// Copyright (c) 201 Mozart Alexander Louis. All rights reserved.

// Includes
#include "swipe_gesture.hxx"
#include "utils/data/data_utils.hxx"

SwipeGesture::SwipeGesture()
    : swipe_direction_(NONE),
      is_touch_down_(false),
      uy_(-Globals::getVisibleSize().width * __SWIPE_GESTURE_UP_DOWN_RESISTANCE__),
      dx_(Globals::getVisibleSize().width * __SWIPE_GESTURE_UP_DOWN_RESISTANCE__),
      ry_(-Globals::getVisibleSize().width * __SWIPE_GESTURE_LEFT_RIGHT_RESISTANCE__),
      lx_(Globals::getVisibleSize().width * __SWIPE_GESTURE_LEFT_RIGHT_RESISTANCE__) {
  updateReverseSwipe();
}

SwipeGesture::~SwipeGesture() = default;

void SwipeGesture::updateReverseSwipe() {
  reverse_swipe_ = bool(DataUtils::getOtherData(__KEY_REVERSE_SWIPE__, 0));
}

void SwipeGesture::resetSwipeDirection() { swipe_direction_ = NONE; }

bool SwipeGesture::onTouchBegan(Touch* touch, Event* event) {
  const auto loc = touch->getLocation();
  touch_pos_[0] = loc.x;
  touch_pos_[1] = loc.y;
  touch_pos_[2] = touch_pos_[0];
  touch_pos_[3] = touch_pos_[1];
  is_touch_down_ = true;
  return true;
}

void SwipeGesture::onTouchMoved(Touch* touch, Event* event) {
  // Updates the current touch position.
  const auto loc = touch->getLocation();
  touch_pos_[2] = loc.x;
  touch_pos_[3] = loc.y;

  if (is_touch_down_) {
    if (touch_pos_[1] - touch_pos_[3] < uy_) {
      swipe_direction_ = reverse_swipe_ ? DOWN : UP;
      touch_pos_[0] = touch_pos_[2];
      touch_pos_[1] = touch_pos_[3];
      // onTouchEnded(nullptr, nullptr);
    } else if (touch_pos_[1] - touch_pos_[3] > dx_) {
      swipe_direction_ = reverse_swipe_ ? UP : DOWN;
      touch_pos_[0] = touch_pos_[2];
      touch_pos_[1] = touch_pos_[3];
      // onTouchEnded(nullptr, nullptr);
    } else if (touch_pos_[0] - touch_pos_[2] > lx_) {
      swipe_direction_ = reverse_swipe_ ? RIGHT : LEFT;
      touch_pos_[0] = touch_pos_[2];
      touch_pos_[1] = touch_pos_[3];
      // onTouchEnded(nullptr, nullptr);
    } else if (touch_pos_[0] - touch_pos_[2] < ry_) {
      swipe_direction_ = reverse_swipe_ ? LEFT : RIGHT;
      touch_pos_[0] = touch_pos_[2];
      touch_pos_[1] = touch_pos_[3];
      // onTouchEnded(nullptr, nullptr);
    }
  }
}

void SwipeGesture::onTouchCancelled(Touch* touch, Event* event) { onTouchEnded(nullptr, nullptr); }

void SwipeGesture::onTouchEnded(Touch* touch, Event* event) {
  is_touch_down_ = false;
  fill_n(touch_pos_, 4, 0);
}
