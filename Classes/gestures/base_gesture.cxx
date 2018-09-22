// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_gesture.hxx"

std::vector<Color4F> BaseGesture::touch_colors_ = {
    // 10 colors
    {1, 1, 1, 1},              // WHITE
    {1, 1, 0, 1},              // YELLOW
    {0, 1, 0, 1},              // GREEN
    {0, 0, 1, 1},              // BLUE
    {1, 0, 0, 1},              // RED
    {1, 0, 1, 1},              // MAGENTA
    {0, 0, 0, 1},              // BLACK
    {1, 0.5f, 0, 1},           // ORANGE
    {0.65f, 0.65f, 0.65f, 1},  // GRAY
    {0.5f, 0.3f, 0.7f, 1}};

BaseGesture::BaseGesture(const int fingers) : fingerNumber(fingers) {
  visibleSize = Globals::getVisibleSize();
  addTouchListener();
  scheduleUpdate();
};

BaseGesture::~BaseGesture() {
  if (listener not_eq nullptr) {
    Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
    listener = nullptr;
  }
};

void BaseGesture::addTouchListener() {
  listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2(BaseGesture::_onTouchBegan, this);
  listener->onTouchMoved = CC_CALLBACK_2(BaseGesture::_onTouchMoved, this);
  listener->onTouchCancelled = CC_CALLBACK_2(BaseGesture::_onTouchCancelled, this);
  listener->onTouchEnded = CC_CALLBACK_2(BaseGesture::_onTouchEnded, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool BaseGesture::isTouchInNode(Touch* t) {
  // object contains point or touch started in the object and exited
  return nodeContainsThePoint(this, t->getLocation()) or mapHasKey(touches, t->getID());
}

void BaseGesture::setFillParentEnabled(const bool enabled) { isFillParentEnabled = enabled; }

void BaseGesture::setContentSize(const Size& content_size) {
  // CCLOG("BaseGesture::setContentSize");
  isFillParentEnabled = false;
  Node::setContentSize(content_size);
}

bool BaseGesture::nodeContainsThePoint(Node* node, const Vec2& pt) {
  const auto& node_sz = node->getContentSize();
  const auto location_in_node_space = node->convertToNodeSpace(pt);

  return Rect(0, 0, node_sz.width, node_sz.height).containsPoint(location_in_node_space);
}

float BaseGesture::computeSignedAngle(const Vec2& v1, const Vec2& v2) const {
  if (not v1.isZero() and not v2.isZero()) {
    const auto a = CC_RADIANS_TO_DEGREES(cocos2d::Vec2::angle(v1, v2));
    Vec3 normal;
    Vec3::cross({v1.x, v1.y, 0}, {v2.x, v2.y, 0}, &normal);
    return normal.z <= 0 ? a : -a;
  }

  return 0;
}

void BaseGesture::setDebugMode(const bool is_debug_mode, const int mask) {
  if (is_debug_mode) {
    debugMask = mask;
    if (debugNode == nullptr) {
      debugNode = DrawNode::create();
      addChild(debugNode);
      addDebugArea();
    }
  } else {
    if (debugNode != nullptr) {
      debugMask = 0;
      debugNode->removeFromParent();
      debugNode = nullptr;
    }
  }
}

bool BaseGesture::isDebugModeEnabled() const { return debugNode != nullptr; }

void BaseGesture::setSwallowTouches(const bool swallow) const {
  if (listener) listener->setSwallowTouches(swallow);
}

void BaseGesture::addDebugArea() const {
  if (debugNode not_eq nullptr and (debugMask bitand __BASE_GESTURE_DEBUG_AREA__) not_eq 0u) {
    const auto origin = Vec2::ZERO;
    const auto dest = origin + getBoundingBox().size;
    debugNode->setLineWidth(4.0f);
    debugNode->drawRect(origin, dest, Color4F(1.0f, 0.0f, 0.0f, 0.5f));
    debugNode->drawSolidRect(origin, dest, Color4F(1.0f, 0.0f, 0.0f, 0.2f));
    debugNode->drawDot(getBoundingBox().size * 0.5f, 3, Color4F(1.0f, 0.0f, 0.0f, 0.5f));
  }
}

void BaseGesture::resetDebugNode() const {
  if (debugNode != nullptr) {
    debugNode->clear();
    addDebugArea();
  }
}

bool BaseGesture::_onTouchBegan(Touch* touch, Event* ev) {
  if (enabled and isTouchInNode(touch)) {
    if (debugNode and debugMask bitand __BASE_GESTURE_DEBUG_POINTS__)
      debugNode->drawPoint(convertToNodeSpace(touch->getLocation()), 4.0f, touch_colors_[touch->getID()]);

    return onTouchBegan(touch, ev);
  }
  return false;
}

void BaseGesture::_onTouchMoved(Touch* touch, Event* ev) {
  if (enabled && isTouchInNode(touch)) {
    if (debugNode && debugMask & __BASE_GESTURE_DEBUG_POINTS__)
      debugNode->drawPoint(convertToNodeSpace(touch->getLocation()), 4.0f, touch_colors_[touch->getID()]);
    onTouchMoved(touch, ev);
  }
}

void BaseGesture::_onTouchCancelled(Touch* touch, Event* ev) {
  if (enabled and isTouchInNode(touch)) {
    if (debugNode) resetDebugNode();
    onTouchCancelled(touch, ev);
  }
}

void BaseGesture::_onTouchEnded(Touch* touch, Event* ev) {
  if (enabled and isTouchInNode(touch)) {
    if (debugNode) resetDebugNode();
    onTouchEnded(touch, ev);
  }
}

Point BaseGesture::mean(const std::unordered_map<int, Point>& map) {
  auto result = Point::ZERO;
  if (not map.empty()) {
    const float size = map.size();
    for (auto& pair : map) result += pair.second;
    return result / size;
  }
  return result;
}

void BaseGesture::scheduleTimeout(const std::function<void(float)> func) {
  unschedule(__BASE_GESTURE_TIMEOUT_KEY__);
  scheduleOnce(func, timeoutSeconds, __BASE_GESTURE_TIMEOUT_KEY__);
}

void BaseGesture::update(const float delta) {
  // Gesture recognizer content size matches with parent content size
  Node::update(delta);
  if (isFillParentEnabled) {
    const auto parent = getParent();
    if (parent not_eq nullptr and not parent->getContentSize().equals(getContentSize())) {
      Node::setContentSize(parent->getContentSize());
      resetDebugNode();
    }
  }
}
