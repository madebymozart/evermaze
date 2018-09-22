/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __SPRITE_MANAGER_HXX__
#define __SPRITE_MANAGER_HXX__

/**
 * Includes
 */
#include "managers/base_manager.hxx"

/**
 * SpriteManager
 */
class SpriteManager : public BaseManager {
 public:
  /**
   * Constructor.
   *
   * @param info  ~ ValueVector of info for the initialization of the SpriteManager
   * @param layer ~ Reference to the main layer associated to the scene.
   */
  explicit SpriteManager(const ValueVector& info, Layer& layer);

  /**
   * Destructor.
   */
  virtual ~SpriteManager();

  /**
   * Searches & converts the result of the call to {@link BaseManager#findNode} into a {@link cocos2d#Sprite}.
   *
   * @param key ~ Key of the sprite you are searching for.
   *
   * @returns ~ Converted node to {@link cocos2d#Sprite}.
   *
   * @note ~ This will throw an error if the node cannot be found.
   */
  Sprite* findSprite(const string& key);

 protected:
  /**
   * Create a sprite and set properties. if the properties contains an action key, we will also
   * apply the action to the sprite.
   *
   * @param props ~ Properties of the sprite to add to sprite map
   */
  void addSpriteToNodeMap(const ValueMap& props);

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(SpriteManager)
};

#endif  // __SPRITE_MANAGER_HXX__
