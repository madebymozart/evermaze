/**  Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __SWIPE_GESTURE_HXX__
#define __SWIPE_GESTURE_HXX__

/**
 * Gesture Resistances
 */
#define __SWIPE_GESTURE_UP_DOWN_RESISTANCE__ 0.08f
#define __SWIPE_GESTURE_LEFT_RIGHT_RESISTANCE__ 0.07f

/**
 * Includes
 */
#include "gestures/base_gesture.hxx"

/**
 * Forward Declaration
 */
class BaseLluma;

class SwipeGesture : public BaseGesture {
  /**
   * Friend Classes
   */
  friend BaseLluma;

 public:
  /**
   * Constructor.
   */
  explicit SwipeGesture();

  /**
   * Destructor.
   */
  virtual ~SwipeGesture();

  /**
   * Updates the {@link SwipeGesture#reverse_swipe_} with the configuration
   */
  void updateReverseSwipe();

  /**
   * Resets the swipe direction to NONE
   */
  void resetSwipeDirection();

 protected:
  /**
   * {@link BaseGesture#onTouchBegan}
   */
  bool onTouchBegan(Touch* touch, Event* event) override;

  /**
   * {@link BaseGesture#onTouchMoved}
   */
  void onTouchMoved(Touch* touch, Event* event) override;

  /**
   * {@link BaseGesture#onTouchCancelled}
   */
  void onTouchCancelled(Touch* touch, Event* event) override;

  /**
   * {@link BaseGesture#onTouchEnded}
   */
  void onTouchEnded(Touch* touch, Event* event) override;

  // Top level swipe direction that gets
  Directions swipe_direction_;

  // Set to true when player is touching screen.
  bool is_touch_down_;

  // Keeps a record of the initial & current touch positions.
  float touch_pos_[4]{};

  // Swipe up resistance
  float uy_{};

  // Swipe down resistance
  float dx_{};

  // Swipe right resistance
  float ry_{};

  // Swipe left resistance
  float lx_{};

  // Boolean that determines whether or no to reverse swipe or not
  atomic_bool reverse_swipe_{};

 private:
  __DISALLOW_COPY_AND_ASSIGN__(SwipeGesture)
};

#endif  // __SWIPE_GESTURE_HXX__
