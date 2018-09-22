/** Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

/**
 * Include Guard
 */
#ifndef __BASE_LLURKER_HXX__
#define __BASE_LLURKER_HXX__

/**
 * Defines
 */
#define __BASE_LLURKER_PREFIX__ "entities/llurkers/"
#define __BASE_LLURKER_CHASE_DISTANCE__ "llurker_chase_distance"
#define __BASE_LLURKER_GENERATOR_INDEX__ "llurker_generate_index"

/**
 * Includes
 */
#include <atomic>
#include <deque>
#include "entities/base_entity.hxx"

/**
 * BaseLlurker
 */
class BaseLlurker : public BaseEntity {
 protected:
  /**
   * Constructor.
   *
   * @param scene     ~ Pointer to the BaseGameScene
   * @param params    ~ The params of the entity
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  explicit BaseLlurker(BaseGameScene& scene, ValueMap params, const string& tmx_layer);

  /**
   * Destructor.
   */
  virtual ~BaseLlurker();

 public:
  /**
   * @brief Create a Llurker based on the passed in values
   *
   * @param scene     ~ Pointer to the game scene on a base level
   * @param llurker   ~ Name of the llurker that will be inflated
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  static BaseLlurker* createLlurker(BaseGameScene* scene, Llurkers llurker,
                                    const string& tmx_layer = string("ui"));

  /**
   * @see BaseEntity::updateEntity()
   */
  void updateEntity() override;

 protected:
  /**
   * Generates the ValueMap data for the llurkers stats. All inherited classes must implement
   *
   * @returns Map of stats for the llurker
   */
  virtual ValueMap generateStats() = 0;

  /**
   * Calculates the path from point. All inherited classes must implement.
   *
   * @param from ~ The from point
   * @param to   ~ The to point
   */
  virtual void onCalculate(Point from, Point to) = 0;

  /**
   * Checks if the next tile, based on the current direction
   */
  void updateDirections();

  /**
   * Updates the position of the llurker
   */
  void updatePosition();

  /**
   * Clear the {@link BaseLlurker#path_queue_} and calculates a path
   */
  void updatePathQueue();

  // Tell the llurker to chase the lluma
  atomic_bool chase_;

  // Generate index that allow the llurker to have a higher chance of calculating a path closer to you
  atomic_int generate_index_;

  // A queue that will periodically get path updated from the future path queue as the llurker travels the map
  // in search for the player
  deque<Point> path_queue_;

  // Mersenne Twister pseudo-random number generator with random seed
  mt19937 gen_;

 private:
  // __DISALLOW_COPY_AND_ASSIGN__(typeName)
  __DISALLOW_COPY_AND_ASSIGN__(BaseLlurker)
};

#endif  // __BASE_LLURKER_HXX__
