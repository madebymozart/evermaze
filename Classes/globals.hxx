/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __GLOBALS_HXX__
#define __GLOBALS_HXX__

/**
 * Includes
 */
#include <typeinfo>
#include <utility>
#include "cocos2d.h"
#include "constants.hxx"
#include "enums.hxx"
#include "macros.hxx"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <ciso646>
#endif

/**
 * Namespaces
 */
using namespace cocos2d;
using namespace std;

class Globals {
 public:
  /**
   * Gets the visible size of the device.
   *
   * @returns ~ Visible size of the device
   */
  static cocos2d::Size getVisibleSize() { return Director::getInstance()->getVisibleSize(); }

  /**
   * Gets the origin point of the device.
   *
   * @returns ~ Origin position on screen.
   */
  static Vec2 getOrigin() { return Director::getInstance()->getVisibleOrigin(); }

  /**
   * Calculates the position on screen using the visible size and point of origin on the device.
   *
   * @param width  ~ Position width
   * @param height ~ Position height
   *
   * @returns ~ Calculated position.
   */
  static Vec2 getScreenPosition(const float width = 2, const float height = 2) {
    return Vec2(getVisibleSize().width / width + getOrigin().x,
                getVisibleSize().height / height + getOrigin().y);
  }

  /**
   * Returns a boolean if the Object parameter (which can be an expression) is an instance of a class type.
   *
   * @param pointer ~ Object to be checked.
   *
   * @note This is equivalent to Java instanceOf.
   */
  template <typename Base, typename T>
  static bool instanceof (const T* pointer) {
    return dynamic_cast<const Base*>(pointer) not_eq nullptr;
  }

  /**
   * Hash to allow strings to be in switch statements
   *
   * @param str ~ The string to be hashed.
   * @param h   ~ h position to 5381.
   *
   * @return ~ switchable hashed string as unsigned integer.
   */
  constexpr static unsigned int sshash(const char* str, const uint32_t h = 0) {
    return not str[h] ? 5381 : sshash(str, h + 1) * 33 ^ str[h];
  }

  /**
   * Runs assertions on a ValueMap to check if the fields exist.
   *
   * @param map  ~ Reference to the {@link ValueMap}
   * @param keys ~ List of keys expected to be in the {@link ValueMap}
   */
  static void assertValueMap(const ValueMap& map, initializer_list<string> keys) {
    for_each(keys.begin(), keys.end(), [&](const auto& key) {
      CCASSERT(map.find(key) not_eq map.end(), string("ValueMap does not contain key: " + key).c_str());
    });
  }

 private:
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(Globals)
};

#endif  // __GLOBALS_HXX__
