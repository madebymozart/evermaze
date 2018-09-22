#include "path_utils.hxx"
#include <algorithm>

using namespace std::placeholders;

PathNode::PathNode(Vec2 coordinates_, PathNode* parent_) {
  parent = parent_;
  coordinates = coordinates_;
  G = H = 0;
}

uint PathNode::getScore() { return G + H; }

Generator::Generator() {
  setDiagonalMovement(false);
  setHeuristic(&Heuristic::manhattan);
  direction = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
}

void Generator::setWorldSize(Vec2 worldSize_) { worldSize = worldSize_; }

void Generator::setDiagonalMovement(bool enable_) { directions = (enable_ ? 8 : 4); }

void Generator::setHeuristic(HeuristicFunction heuristic_) { heuristic = std::bind(heuristic_, _1, _2); }

void Generator::addCollision(Vec2 coordinates_) { walls.push_back(coordinates_); }

void Generator::addCollisionList(std::vector<Point> coordinates_) { walls.swap(coordinates_); }

void Generator::addCollisionList(TmxObject& tmx_object, const string& layer) {
  addCollisionList(tmx_object.collistions_map_.at(layer));
}

void Generator::removeCollision(Vec2 coordinates_) {
  auto it = std::find(walls.begin(), walls.end(), coordinates_);
  if (it != walls.end()) {
    walls.erase(it);
  }
}

void Generator::clearCollisions() { walls.clear(); }

void Generator::findPath(Vec2 source_, Vec2 target_, deque<Point>& queue, int limit) {
  PathNode* current = nullptr;
  NodeSet openSet, closedSet;
  openSet.insert(new PathNode(source_));

  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto node : openSet) {
      if (node->getScore() <= current->getScore()) {
        current = node;
      }
    }

    if (current->coordinates == target_) break;

    closedSet.insert(current);
    openSet.erase(std::find(openSet.begin(), openSet.end(), current));

    for (uint i = 0; i < directions; ++i) {
      Vec2 newCoordinates(current->coordinates + direction[i]);
      if (detectCollision(newCoordinates) || findNodeOnList(closedSet, newCoordinates)) {
        continue;
      }

      uint totalCost = current->G + ((i < 4) ? 10 : 14);

      PathNode* successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = new PathNode(newCoordinates, current);
        successor->G = totalCost;
        successor->H = heuristic(successor->coordinates, target_);
        openSet.insert(successor);
      } else if (totalCost < successor->G) {
        successor->parent = current;
        successor->G = totalCost;
      }
    }
  }

  while (current != nullptr) {
    queue.emplace_front(current->coordinates);
    current = current->parent;
  }

  while (queue.size() > limit) {
    queue.pop_back();
  }

  releaseNodes(openSet);
  releaseNodes(closedSet);
}

PathNode* Generator::findNodeOnList(NodeSet& nodes_, Vec2 coordinates_) {
  for (auto node : nodes_) {
    if (node->coordinates == coordinates_) {
      return node;
    }
  }
  return nullptr;
}

void Generator::releaseNodes(NodeSet& nodes_) {
  for (auto it = nodes_.begin(); it != nodes_.end();) {
    delete *it;
    it = nodes_.erase(it);
  }
}

bool Generator::detectCollision(Vec2 coordinates_) {
  return coordinates_.x < 0 || coordinates_.x >= worldSize.x || coordinates_.y < 0 ||
         coordinates_.y >= worldSize.y || std::find(walls.begin(), walls.end(), coordinates_) != walls.end();
}

Vec2 Heuristic::getDelta(Vec2 source_, Vec2 target_) {
  return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

uint Heuristic::manhattan(Vec2 source_, Vec2 target_) {
  auto delta = std::move(getDelta(source_, target_));
  return static_cast<uint>((delta.x + delta.y));
}

uint Heuristic::euclidean(Vec2 source_, Vec2 target_) {
  auto delta = std::move(getDelta(source_, target_));
  return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

uint Heuristic::octagonal(Vec2 source_, Vec2 target_) {
  auto delta = std::move(getDelta(source_, target_));
  return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}
