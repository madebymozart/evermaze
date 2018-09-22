/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __TEXT_MANAGER_HXX__
#define __TEXT_MANAGER_HXX__

#define __TEXT_MANAGER_PREFIX__ "text/"

/**
 * Includes
 */
#include "managers/base_manager.hxx"

class TextManager : public BaseManager {
 public:
  /**
   * Constructor.
   *
   * @param info  ~ ValueMap of info for the initialization of the TextManager
   * @param layer ~ Reference to the main layer associated to the scene
   */
  explicit TextManager(const ValueMap& info, Layer& layer);

  /**
   * Destructor.
   */
  virtual ~TextManager();

  /**
   * Searches & converts the result of the call to {@link BaseManager#findNode} into a {@link cocos2d#Label}
   *
   * @param key ~ Key of the label you are searching for.
   *
   * @returns ~ Converted node to {@link cocos2d#Label}
   *
   * @note ~ This will throw an error if the node cannot be found
   */
  Label* findLabel(const string& key);

  /**
   * Gets the string font from a Font enumeration.
   *
   * @param font ~ Font found int enums.hxx.
   *
   * @returns The string name of the font for loading.
   */
  static string getFont(Fonts font);

  /**
   * Gets the correct TextHAlignment Vec2 from a string.
   *
   * @param align ~ String representing alignment (right, left, center)
   *
   * @returns Vec2 alignment based on AnchorPoint
   */
  static TextHAlignment getHAlignment(const string& align = "center");

  /**
   * Gets the correct TextVAlignment Vec2 from a string.
   *
   * @param align ~ String representing alignment (right, left, center)
   *
   * @returns Vec2 alignment based on AnchorPoint
   */
  static TextVAlignment getVAlignment(const string& align = "center");

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(TextManager)
};

#endif  // __TEXT_MANAGER_HXX__
