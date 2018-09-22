/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __IVORY_HXX__
#define __IVORY_HXX__

/**
 * Entity File for Ivory
 */
#define __IVORY_ENTITY_FILE__ "entities/llumas/ivory.plist"

/**
 * Includes
 */
#include "entities/llumas/base_lluma.hxx"

class Ivory : public BaseLluma {
 public:
  /**
   * Constructor.
   *
   * @param scene     ~ Pointer to the active game layer.
   * @param id        ~ The id of the lluma to load.
   * @param gesture   ~ Pointer to the active swipe gesture pointer on the game scene.
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  explicit Ivory(BaseGameScene* scene, SwipeGesture* gesture, const string& id, const string& tmx_layer);

  /**
   * Destructor
   */
  ~Ivory();

 protected:
  /**
   * @link BaseLluma::generateStats();
   */
  ValueMap generateStats(const string& id) override;

 private:
  /**
   *  __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(Ivory)
};

#endif  // __IVORY_HXX__
