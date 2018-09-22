/**  Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __BASE_GESTURE_HXX__
#define __BASE_GESTURE_HXX__

/**
 * Keys
 */
#define __BASE_GESTURE_DEBUG_AREA__ (1 << 0)
#define __BASE_GESTURE_DEBUG_POINTS__ (1 << 1)
#define __BASE_GESTURE_DEBUG_ALL__ (0XFFFFFFFF)
#define __BASE_GESTURE_TIMEOUT_KEY__ "GestureRecognizerTimeoutKey"

/**
 * Includes
 */
#include <unordered_map>
#include <vector>
#include "globals.hxx"

/**
 * Enumeration
 **/
enum class GestureStatus {
  POSSIBLE,    // Initial state
  RECOGNIZED,  // Recognized gesture (ended in the continuous gesture case)
  FAILED,      // Recognition failed
  BEGAN,       // Continuous gesture began
  CHANGED,     // Continuous gesture change state
  CANCELED     // Continuous gesture canceled
};

class BaseGesture : public Node {
 public:
  static std::vector<Color4F> touch_colors_;

  BaseGesture(int fingers = 1);

  virtual ~BaseGesture();

  /**
   *
   * @param mode enable-disable debug mode
   * @param mask specify what informations will be drawn
   */
  void setDebugMode(bool mode, int mask = __BASE_GESTURE_DEBUG_ALL__);

  bool isDebugModeEnabled() const;

  // getters
  GestureStatus getStatus() const { return status; }

  Point getGestureLocation() const { return gestureLocation; }

  // setters
  void setEnabled(bool _enabled) { enabled = _enabled; }

  void setSwallowTouches(bool swallow) const;

  /**
   *
   * @param enabled If enabled the content size of the recognizer will match with it's parent one
   */
  void setFillParentEnabled(bool enabled);

  /**
   * Automatically disable the fill parent feature
   * @param contentSize
   */
  void setContentSize(const Size& contentSize) override;

  /**
   *
   * @param map
   * @param key
   * @return true if the container 'map' has the element 'key'
   */
  template <class Map, class Key>
  bool mapHasKey(const Map& map, const Key& key) {
    return map.find(key) != map.end();
  }

  /**
   *
   * @param node
   * @param pt
   * @return true if 'node' contains the point 'pt'
   */
  static bool nodeContainsThePoint(Node* node, const Vec2& pt);

  /**
   *
   * @param v1
   * @param v2
   * @return angle between v1 and v2. The angle is positive if v1 overlaps v2 clockwise, negative otherwise.
   */
  float computeSignedAngle(const Vec2& v1, const Vec2& v2) const;

 protected:
  bool isTouchInNode(Touch* t);

  void scheduleTimeout(std::function<void(float)> func);

  virtual void timeout(float dt){};

  static Point mean(const std::unordered_map<int, Point>& map);

  // Pure virtual function implemented by subclasses
  virtual bool onTouchBegan(Touch* touch, Event* event) = 0;

  virtual void onTouchMoved(Touch* touch, Event* event) = 0;

  virtual void onTouchCancelled(Touch* touch, Event* event) = 0;

  virtual void onTouchEnded(Touch* touch, Event* event) = 0;

  void update(float delta) override;

  // Dati membro
  bool enabled{true};  // if false the recognizer does not receive the touches
  bool isFillParentEnabled{true};
  const int fingerNumber;
  Size visibleSize;
  EventListenerTouchOneByOne* listener{nullptr};
  Point gestureLocation{Point::ZERO};
  std::unordered_map<int, Point> touches;  // key touch id -> value location of the point
  GestureStatus status{GestureStatus::POSSIBLE};
  float timeoutSeconds{0};  // timeout for gesture recognition

  // Debug stuff
  int debugMask{0};
  DrawNode* debugNode{nullptr};

 private:
  void addTouchListener();

  void addDebugArea() const;

  void resetDebugNode() const;

  /*
   * Functions linked to the EventListenerTouchOneByOne
   */
  virtual bool _onTouchBegan(Touch* touch, Event* ev);

  virtual void _onTouchMoved(Touch* touch, Event* ev);

  virtual void _onTouchCancelled(Touch* touch, Event* ev);

  virtual void _onTouchEnded(Touch* touch, Event* ev);
};

#endif  /// __BASE_GESTURE_HXX__
