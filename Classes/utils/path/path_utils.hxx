/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __PATH_UTILS__
#define __PATH_UTILS__

/**
 * Includes
 */
#include <functional>
#include <set>
#include <vector>
#include "globals.hxx"
#include "objects/tmx/tmx_object.hxx"

using uint = unsigned int;
using HeuristicFunction = std::function<uint(Point, Point)>;

struct PathNode {
  uint G, H;
  Point coordinates;
  PathNode* parent;

  explicit PathNode(Point coord_, PathNode* parent_ = nullptr);

  uint getScore();
};

using NodeSet = std::set<PathNode*>;

class Generator {
  bool detectCollision(Point coordinates_);

  PathNode* findNodeOnList(NodeSet& nodes_, Point coordinates_);

  void releaseNodes(NodeSet& nodes_);

 public:
  Generator();

  void setWorldSize(Point worldSize_);

  void setDiagonalMovement(bool enable_);

  void setHeuristic(HeuristicFunction heuristic_);

  void findPath(Point source_, Point target_, deque<Point>& queue, int limit = 50);

  void addCollision(Point coordinates_);

  void addCollisionList(std::vector<Point> coordinates_);

  void addCollisionList(TmxObject& tmx_object, const string& layer = "ui");

  void removeCollision(Point coordinates_);

  void clearCollisions();

 private:
  HeuristicFunction heuristic;
  std::vector<Point> direction, walls;
  Point worldSize;
  uint directions;
};

class Heuristic {
  static Point getDelta(Point source_, Point target_);

 public:
  static uint manhattan(Point source_, Point target_);

  static uint euclidean(Point source_, Point target_);

  static uint octagonal(Point source_, Point target_);
};

#endif  // __PATH_UTILS__
