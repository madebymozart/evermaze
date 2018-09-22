/**  Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

#ifndef __TAP_GESTURE_HXX__
#define __TAP_GESTURE_HXX__

#define TAP_MOVE_DELTA 100
#define TAP_MAX_BEETWEEN_TOUCHES 0.5f  // in seconds

#include "../base_gesture.hxx"

struct TouchInfo {
  TouchInfo(cocos2d::Point p, int c = 0) {
    point = p;
    touchCount = c;
  }
  cocos2d::Point point{0, 0};
  int touchCount{0};
};

class TapGesture : public BaseGesture {
 public:
  /**
   * Constructor
   */
  explicit TapGesture(int fingerCount, int tapNumber, float maxTime);

  /**
   * Destructor
   */
  virtual ~TapGesture();

  /**
   * An Implementation the "static create()" from method done manually.
   */
  static TapGesture *create(int /* fingerCount */ = 1, int /* tap_amount_ */ = 1,
                            float /* maxTime */ = TAP_MAX_BEETWEEN_TOUCHES);

  /**
   * Sets and Gets the callback for when a swipe is detected
   */
  void setTapCallback(std::function<void(TapGesture *)> /* callback */);

 private:
  /**
   * @see BaseGesture::onTouchBegan
   */
  bool onTouchBegan(Touch * /* touch */, Event * /* ev */) override;

  /**
   * @see BaseGesture::onTouchMoved
   */
  void onTouchMoved(Touch * /* touch */, Event * /* ev */) override;

  /**
   * @see BaseGesture::onTouchCancelled
   */
  void onTouchCancelled(Touch * /* touch */, Event * /* ev */) override;

  /**
   * @see BaseGesture::onTouchEnded
   */
  void onTouchEnded(Touch * /* touch */, Event * /* ev */) override;

  /**
   *
   */
  bool touchEndCheck(Touch * /* touch */);

  /**
   *
   */
  bool existNeighbor(cocos2d::Point aPoint, int &touchIndex);

  /**
   * Resets the tap gesture status
   */
  void reset(float dt = 0);

  /// The Swipe function callback that will be called every time a tap is detected
  function<void(TapGesture *)> callback_;

  /// The amount of touched for a given id in a map
  unordered_map<int, int> touch_count_;

  /// The amount of taps we are looking for to call the callback
  const int tap_amount_;

  /// The number of tap
  int tap_count_{0};
};

#endif  /// __TAP_GESTURE_HXX__
