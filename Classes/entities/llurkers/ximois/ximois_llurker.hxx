/** Copyright (c) 2017 Mozart Alexander Louis. All rights reserved. */

/**
 * Include Guard
 */
#ifndef __XIOMOIS_HXX__
#define __XIOMOIS_HXX__

/**
 * Entity File for Ximois
 */
#define __XIMOIS__ "ximois"
#define __XIMOIS_ENTITY_FILE__ "entities/llurkers/ximois.plist"

/**
 * Includes
 */
#include "entities/llurkers/base_llurker.hxx"

class Ximois : public BaseLlurker {
 public:
  /**
   * @brief Constructor
   *
   * @param scene     ~ Pointer to the active game layer.
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  explicit Ximois(BaseGameScene* scene, const string& tmx_layer);

  /**
   * @brief Destructor
   */
  virtual ~Ximois();

 private:
  /**
   * @see BaseLlurker::generateStats();
   */
  ValueMap generateStats() override;

  /**
   * @see BaseLlurker::calculateFuturePath();
   */
  void onCalculate(Point from, Point player_position) override;

  // @see __DISALLOW_COPY_AND_ASSIGN__(typeName)
  __DISALLOW_COPY_AND_ASSIGN__(Ximois)
};

#endif  // __XIOMOIS_HXX__
