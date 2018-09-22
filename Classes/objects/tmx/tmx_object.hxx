/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __TMX_OBJECT_HXX__
#define __TMX_OBJECT_HXX__

/**
 * Defines
 */
#define __HASH_SEED__ 86969
#define __HASH_STRING_POINT__ "D)G9Ud?`Sdt.W=2`"
#define __HASH_STRING_VEC2__ "R^malv)#,1%<;a43"

/**
 * Includes
 */
#include "globals.hxx"
#include "xxhash/xxhash.h"

/**
 * Forward Declaration
 */
class Generator;

/**
 * Hash Function for Points
 */
struct PointHash {
  size_t operator()(const Point& k) const {
    // Using xxhash to has the point. Each point in an instance on this a
    // tmxObject should be unique.
    const auto str = to_string(k.x) + __HASH_STRING_POINT__ + to_string(k.y);
    const auto result = XXH32(str.c_str(), static_cast<int>(str.length()), __HASH_SEED__);
    return static_cast<size_t>(result);
  }
};

/**
 * Hash Function for Vec2
 */
struct Vec2Hash {
  size_t operator()(const Vec2& k) const {
    // Using xxhash to has the point. Each point in an instance on this a
    // tmxObject should be unique.
    const auto str = to_string(k.x) + __HASH_STRING_VEC2__ + to_string(k.y);
    const auto result = XXH32(str.c_str(), static_cast<int>(str.length()), __HASH_SEED__);
    return static_cast<size_t>(result);
  }
};

/**
 * Type Aliases
 */
using PointH = unordered_map<const Point, const Vec2, PointHash>;
using Vec2H = unordered_map<const Vec2, const Point, Vec2Hash>;

/**
 * TmxObject
 */
class TmxObject : Ref {
  /**
   * Friend Classes
   */
  friend Generator;

 public:
  /**
   * Construction.
   *
   * @param map  ~ Reference to the tiled map that will create this object for.
   * @param info ~ ValueVextor of info to generate the tmx-object.
   */
  explicit TmxObject(const TMXTiledMap& map, const ValueVector& info);

  /**
   * Destructor.
   */
  virtual ~TmxObject();

  /**
   * Gets a point from the `tmx_object` by layer
   *
   * @param position   ~ The position reference.
   * @param layer      ~ The layer name to search for the point.
   *
   * @returns ~ Point if found, otherwise Point(-1, -1).
   */
  Point getPoint(const Vec2& position, const string& layer);

  /**
   * Gets a point from the `tmx_object` by layer
   *
   * @param point      ~ The point reference.
   * @param layer      ~ The layer name to search for the point.
   *
   * @returns ~ Position if found, otherwise Point(-1, -1).
   */
  Vec2 getPosition(const Point& point, const string& layer);

  /**
   * Check if a point in the TmxObject by layer exists.
   *
   * @param point      ~ The position reference.
   * @param layer      ~ The layer name to search for the point.
   */
  bool containsPoint(const Point& point, const string& layer);

  /**
   * Check if a point in the TmxObject by layer exists.
   *
   * @param position   ~ The position reference.
   * @param layer      ~ The layer name to search for the point.
   */
  bool containsPosition(const Vec2& position, const string& layer);

  /**
   * Gets a pair in a point hashed map.
   *
   * @param point      ~ The point key.
   * @param layer      ~ The layer name to search for the point.
   *
   * @returns ~ Pair containing the point as key and position as value.
   */
  PointH::const_iterator getPointPair(const Point& point, const string& layer);

  /**
   * Gets a pair in a position hashed map.
   *
   * @param position   ~ The position reference
   * @param layer      ~ The layer name to search for the point.
   *
   * @returns ~ Pair containing the position as key and point as value.
   */
  Vec2H::const_iterator getPositionPair(const Vec2& position, const string& layer);

  /**
   * Gets a random point pair from a point hashed map
   *
   * @param layer      ~ The layer name to search for the point.
   * @returns ~ Pair containing the position as key and point as value.
   */
  PointH::const_iterator getRandomPoint(const string& layer);

  /**
   * Gets a random point pair from a point hashed map
   *
   * @param layer      ~ The layer name to search for the point.
   * @returns ~ Pair containing the position as key and point as value.
   */
  PointH::const_iterator getRandomPosition(const string& layer);

 protected:
  // Map containing maps of points -> vec2 positions
  unordered_map<string, PointH> point_map_;

  // Map containing vectors of collistion walls.
  unordered_map<string, vector<Point>> collistions_map_;

  // Map containing maps of vec2 positions -> points
  unordered_map<string, Vec2H> positions_map_;

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(TmxObject)
};

#endif  // __TMX_OBJECT_HXX__
