/// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

#include <utility>

#include "tap_gesture.hxx"

TapGesture::TapGesture(int fingers, int taps, float maxTime) : BaseGesture(fingers), tap_amount_(taps) {
  timeoutSeconds = maxTime;
}

TapGesture::~TapGesture() = default;

TapGesture* TapGesture::create(int fingerCount, int tapNumber, float maxTime) {
  auto p_ret = new TapGesture(fingerCount, tapNumber, maxTime);
  if (p_ret->init()) {
    p_ret->autorelease();
    return p_ret;
  }

  /// In extremely rare cases, Something may go wrong with the initialization so
  /// clean up and throw an error.
  delete p_ret;
  return nullptr;
}

void TapGesture::setTapCallback(function<void(TapGesture*)> callback) { callback_ = std::move(callback); }

bool TapGesture::onTouchBegan(Touch* touch, Event* ev) {
  if (touches.empty()) {
    status = GestureStatus::POSSIBLE;
    reset();
    scheduleTimeout(CC_CALLBACK_1(TapGesture::reset, this));
  }

  touches.insert({touch->getID(), touch->getLocation()});
  touch_count_.insert({touch->getID(), 0});  // tap counter for each id start at 0

  if (touches.size() > fingerNumber) {
    // too many touches!
    status = GestureStatus::FAILED;
    reset();
    return false;
  }
  return true;
}

void TapGesture::onTouchMoved(Touch* touch, Event* ev) {
  if (status == GestureStatus::POSSIBLE) {
    int neighborIndex;
    if (!existNeighbor(touch->getLocation(), neighborIndex)) {
      // finger moved here
      status = GestureStatus::FAILED;
      reset();
    }
  }
}

void TapGesture::onTouchCancelled(Touch* touch, Event* ev) {
  status = GestureStatus::FAILED;
  reset();
}

void TapGesture::onTouchEnded(Touch* touch, Event* ev) {
  if (status == GestureStatus::POSSIBLE && touches.size() == fingerNumber) {
    if (touchEndCheck(touch))  // all fingers lifted
      tap_count_++;

    if (tap_count_ == tap_amount_) {
      status = GestureStatus::RECOGNIZED;
      gestureLocation = mean(touches);
      reset();
    }
  }
}

/**
 *
 * @param aPoint Point of we search the neighbor
 * @param touchIndex output parameter, id of the neighbor
 * @return true if neighbor is found
 */
bool TapGesture::existNeighbor(cocos2d::Point aPoint, int& touchIndex) {
  for (auto& p : touches) {
    auto p2 = p.second;

    if (aPoint.distance(p2) <= TAP_MOVE_DELTA) {
      touchIndex = p.first;
      return true;
    }
  }
  return false;
}

/**
 *
 * @param touch
 * @return true when there exist "fingerNumber" touches with counter equals to "tapCount + 1"
 */
bool TapGesture::touchEndCheck(Touch* touch) {
  int neighborIndex;
  if (existNeighbor(touch->getLocation(), neighborIndex))
    touch_count_.at(neighborIndex)++;  // update counter relative to lifted finger

  auto count = std::count_if(touch_count_.begin(), touch_count_.end(),
                             [=](const std::pair<int, int>& p) { return p.second == (int)tap_count_ + 1; });
  return count == fingerNumber;
}

void TapGesture::reset(float dt) {
  if (callback_) callback_(this);

  unschedule(__BASE_GESTURE_TIMEOUT_KEY__);
  touches.clear();
  touch_count_.clear();
  tap_count_ = 0;
}
