/** Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

/**
 * Include Guard
 */
#ifndef __BACKGROUND_LAYER_HXX__
#define __BACKGROUND_LAYER_HXX__

/**
 * Tags
 */
#define __BG_COLOR_TAG__ "color_tag"
#define __BG_PATTERN_ONE_TAG__ "pattern_one"
#define __BG_PATTERN_TWO_TAG__ "pattern_two"

/**
 * Color & Pattern Data Scirpts
 */
#define __COLOR_CONFIG__ "configs/colors.plist"
#define __PATTERN_CONFIG__ "configs/patterns.plist"

/**
 * Includes
 */
#include "globals.hxx"

/**
 * Aliases
 */
using BackgroundMap = unordered_map<string, ValueMap>;

class BackgroundLayer : public Layer {
 public:
  /**
   * Constructor.
   */
  explicit BackgroundLayer();

  /**
   * Destructor.
   */
  virtual ~BackgroundLayer();

  /**
   * @link Layer::init
   */
  bool init() override;

  /**
   * Sets the background gradient low color from the loaded color data script.
   *
   * @param preview ~ The color id to preview. This is used by the store to temparaly change
   *                  the color.
   */
  void setBackgroundColor(const string& preview = "");

  /**
   * Sets the backgrounf pattern from the loaded pattern data script.
   *
   * @param preview ~ The pattern id to preview. This is used by the store to temparaly change
   *                  the pattern.
   */
  void setBackgroundPattern(const string& preview = "");

  /**
   * Schedule the scroll pattern methods.
   */
  void scheduleScrollPattern();

  /**
   * unschedules the scroll pattern methods.
   */
  void unscheduleScrollPattern();

  // Create function
  CREATE_FUNC(BackgroundLayer)

 protected:
  /**
   * @brief Scheduled by a scheduler to update the position of the bg pattern every time it's called
   */
  void scrollPattern(float dt);

 private:
  // Contains the data that will be used to load textures for the background colors
  unordered_map<string, ValueVector> color_config_;

  // Contains the data that will be used to load colors for the background patterns.
  unordered_map<string, ValueMap> pattern_config_;

  // Hot fix for scrolling maze background.
  bool scroll_fix_;

  // Speed in which the pattern should travel on the screen, relative to pixels moving.
  atomic_int64_t speed_;

  __DISALLOW_COPY_AND_ASSIGN__(BackgroundLayer)
};

#endif  // __BACKGROUND_LAYER_HXX__
