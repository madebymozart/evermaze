/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

/**
 * Include Guard
 */
#ifndef __BASE_ENTITY_HXX__
#define __BASE_ENTITY_HXX__

/**
 * Defines
 */
#define __ENTITY_FILE__ "file"
#define __ENTITY_ID__ "id"
#define __ENTITY_SPEED__ "speed"
#define __ENTITY_TMX_LAYER__ "tmx_layer"

/**
 * Includes
 */
#include "globals.hxx"
#include "objects/decimal/decimal_object.hxx"
#include "scenes/game/base_game_scene.hxx"
#include "utils/path/path_utils.hxx"

/**
 * Namespaces
 */
using namespace dec;

/**
 * BaseEntity
 */
class BaseEntity : public ParticleSystemQuad {
 protected:
  /**
   * Constructor.
   *
   * @param scene     ~ Pointer to the BaseGameScene
   * @param params    ~ The params of the entity
   * @param tmx_layer ~ The layer to set as a current layer for walls and traversal
   */
  explicit BaseEntity(BaseGameScene* scene, ValueMap params, const string& tmx_layer);

  /**
   * Virtual Destructor.
   */
  ~BaseEntity();

  /**
   * Updates the entity state. This should be called every frame.
   */
  virtual void updateEntity() = 0;

  // The current direction in which the llurker is going.
  Directions direction_;

  // A reference to the current layer that entity will reside one
  BaseGameScene& game_scene_;

  // The stats of the entity. This can vary between different types but are ultimate keep for the life of the
  // entity
  ValueMap stats_;

  // Current point of the entity
  Point current_point_;

  // All maps are based on 108x108 tiles. This variable takes care of subtracting by multiple of 108 to
  // traverse through the map. the default traversal is 9.
  decimal<3> movement_;

  // Pace in which the entity moves through the map. This is based on a 108 factor number. The default is 6.
  decimal<3> entitiy_speed_;

  // Pace in which the player moves through the map. This is based on a 108 factor number. The default is 0.
  decimal<3> speed_changer_;

  // Tell the entity if they are allowed to move and when the entity has first moved
  atomic_bool can_move_;

  // Reference to the ui tiled in the TmxEngine cache.
  TmxObject& tiles_;

  // The current layer to look at when trying to navigate through the maze
  string current_tmx_layer_ = "ui";

  // Class to help with path finding
  Generator path_utils_;

 public:
  /**
   * Setter for {@link BaseEntity#current_point_}
   */
  void setCurrentPoint(const Point& point);

  /**
   * GEtter for {@link BaseEntity#current_point_}
   */
  Point getCurrentPoint() const;

  /**
   * Setter for {@link BaseEntity#current_point_}
   */
  void setCanMove(bool move);

  /**
   * Getter for {@link BaseEntity#current_point_}
   */
  bool getCanMove() const;

  /**
   * Setter for {@link BaseEntity#current_tmx_layer_}
   */
  void setCurrentTmxLayer(const string& layer);

  /**
   * Getter for {@link BaseEntity#current_tmx_layer_}
   */
  string getCurrentTmxLayer() const;

 private:
  // __DISALLOW_COPY_AND_ASSIGN__
  __DISALLOW_COPY_AND_ASSIGN__(BaseEntity)
};

#endif  // __BASE_ENTITY_HXX__
