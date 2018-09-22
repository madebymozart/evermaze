/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __BASE_LLUMA_HXX__
#define __BASE_LLUMA_HXX__

/**
 * Defines
 */
#define __FRAME_SKIPPING__ 10

/**
 * Includes
 */
#include "entities/base_entity.hxx"
#include "gestures/swipe/swipe_gesture.hxx"
#include "objects/tmx/tmx_object.hxx"

class BaseLluma : public BaseEntity {
 public:
  /**
   * Constructor.
   *
   * @param scene     ~ Pointer to the BaseGameScene
   * @param params    ~ The params of the entity
   * @param gesture   ~ Pointer to the active swipe gesture pointer on the game scene.
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  explicit BaseLluma(BaseGameScene& scene, SwipeGesture& gesture, ValueMap params, const string& tmx_layer);

  /**
   * Destructor.
   */
  virtual ~BaseLluma();

  /**
   * Creates a Lluma based on the passed in values.
   *
   * @param scene     ~ Pointer to the active game layer.
   * @param id        ~ The id of the lluma to load.
   * @param gesture   ~ Pointer to the active swipe gesture pointer on the game scene.
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   *
   * @returns ~ A Lluma base don the input.
   */
  static BaseLluma* createLluma(BaseGameScene* scene, SwipeGesture* gesture, const string& id,
                                const string& tmx_layer = string("ui"));

  /**
   * @link BaseEntity::updateEntity()
   */
  void updateEntity() override;

 protected:
  /**
   * Generates the ValueMap data for the Llumas stats.
   *
   * @param id ~ The id of the lluma.
   *
   * @returns ~ Stats for this lluma.
   */
  virtual ValueMap generateStats(const string& id) = 0;

  /**
   * Checks if the next tile, based on the current swipe direction, is available. This will stop
   * the player if they can not move in the direction of the swipe.
   */
  void updatePaths();

  /**
   * Checks if the next tile, based on the current swipe direction, is available. This will stop
   * the player if they can not move in the direction of the swipe.
   *
   * @param direction ~ The direction in which the lluma is moving.
   *
   * @returns ~ If the direction was changed.
   */
  bool updateDirections(Directions direction);

  /**
   * Updates the position of the player based on the `swipe_direction_`
   */
  void updatePosition();

  /**
   * Validates that the given path is the opposite of the swipe. LEFT -> Right || UP -> DOWN
   *
   * @param swipe ~ The direction of the swipes
   *
   * @returns ~ True if the swipe is the opposite, otherwise false.
   */
  bool validateSwipeIsOpposite(Directions swipe) const;

  /**
   * Gets the opposite swipe direction of current swipe
   *
   * @param swipe ~ The swipe direction
   *
   * @return ~ The opposite direction of the swipe
   */
  static Directions getOppositeSwipeDir(Directions swipe);

  /**
   * @brief This method is to give the play a little bit of mistake tolerance in the game.
   *
   * @param swipe The swipe direction
   * @return If the tolerance considered should change the direction of the lluma
   */
  bool considerTolerance(Directions swipe) const;

 private:
  // A vector holding 4 position for the available paths. The order is in strict sequence up,
  // down, left, right.
  pair<Point, Vec2> paths_[4];

  // The precision variable that is used to allow a little frame skipping to make swiping a
  // little more tolerable
  int precision_;

  // Whether the update swipe method should recalculate the players' tile position.
  bool override_swipe_;

  // A reference to the swipe gesture
  SwipeGesture& gesture_;

  // __DISALLOW_COPY_AND_ASSIGN__
  __DISALLOW_COPY_AND_ASSIGN__(BaseLluma)
};

#endif  // __BASE_LLUMA_HXX__
