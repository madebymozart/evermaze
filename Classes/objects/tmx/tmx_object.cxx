// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "tmx_object.hxx"
#include <random>
#include "utils/archive/archive_utils.hxx"

TmxObject::TmxObject(const TMXTiledMap& map, const ValueVector& info) {
  // Get center point of calculation
  const auto div = Globals::getScreenPosition();
  const auto size = map.getMapSize();

  for_each(info.begin(), info.end(), [&](const Value& data) -> void {
    const auto& values = data.asValueMap();

    // Run assertions on required data
    CCASSERT(values.find(__NAME__) not_eq values.end(), "TmxObject: No name was found...");
    CCASSERT(values.find(__LAYER__) not_eq values.end(), "TmxObject: No layer was found...");
    const auto name = values.at(__NAME__).asString();
    const auto layer = values.at(__LAYER__).asString();

    CCASSERT(map.getLayer(layer) not_eq nullptr,
             string("TmxObject: TiledMap does not contain layer by this name" + layer).c_str());

    // Getting a reference to the exact layer
    const auto& tmx_layer = map.getLayer(layer);

    // create maps the be files
    PointH points;
    Vec2H positions;
    vector<Point> collistions;

    // By default, inverse positions are not enable. So check to see that they are
    const auto use_positions =
        values.find(__INVERSE__) not_eq values.end() and values.at(__INVERSE__).asBool();
    const auto use_collistions =
        values.find(__COLLISIONS__) not_eq values.end() and values.at(__COLLISIONS__).asBool();

    // Loop through all of the iteration of the map to generate the correct tiles.
    for (auto px = 0; px < size.width; px++)
      for (auto py = 0; py < size.height; py++) {
        // The point we are currently evaluating.
        const auto point = Point(px, py);

        // Assure the tile is not null
        if (tmx_layer->getTileAt(point) == nullptr) {
          if (use_collistions) collistions.emplace_back(point);
          continue;
        }

        // If the value set has GIDs, Assure that the gid of the tile is part of this list
        if (values.find(__GIDS__) not_eq values.end()) {
          const auto& gids = values.at(__GIDS__).asValueMap();
          const auto& gid = tmx_layer->getTileGIDAt(point);
          if (gids.find(to_string(gid)) == gids.end()) continue;
        }

        // Calculate location on screen and construct position object
        const auto vx = div.x - ((size.width - 1) / 2 - px) * 108;
        const auto vy = div.y + ((size.height - 1) / 2 - py) * 108;
        const auto position = Vec2(vx, vy);

        // Insert tiles into the maps
        points.emplace(point, position);
        if (use_positions) positions.emplace(position, point);
      }

    // Emplace maps. The positions is only added if necessary and defined in the values
    point_map_.emplace(name, points);
    if (not positions.empty()) positions_map_.emplace(name, positions);
    if (not collistions.empty()) collistions_map_.emplace(name, collistions);
  });
}

TmxObject::~TmxObject() {
  point_map_.clear();
  positions_map_.clear();
}

Point TmxObject::getPoint(const Vec2& position, const string& layer) {
  const auto& info = positions_map_.at(layer);
  const auto& itr = info.find(position);
  if (itr == info.end()) return Point(-1, -1);
  return itr->second;
}

Vec2 TmxObject::getPosition(const Point& point, const string& layer) {
  const auto& info = point_map_.at(layer);
  const auto& itr = info.find(point);
  if (itr == info.end()) return Vec2(-1, -1);
  return itr->second;
}

bool TmxObject::containsPoint(const Point& point, const string& layer) {
  const auto& info = point_map_.at(layer);
  return info.find(point) not_eq info.end();
}

bool TmxObject::containsPosition(const Vec2& position, const string& layer) {
  const auto& info = positions_map_.at(layer);
  return info.find(position) not_eq info.end();
}

PointH::const_iterator TmxObject::getPointPair(const Point& point, const string& layer) {
  const auto& info = point_map_.at(layer);
  return find_if(info.begin(), info.end(),
                 [point](const pair<Point, Vec2> pair) { return point == pair.first; });
}

Vec2H::const_iterator TmxObject::getPositionPair(const Vec2& position, const string& layer) {
  const auto& info = positions_map_.at(layer);
  return find_if(info.begin(), info.end(),
                 [position](const pair<Vec2, Point> pair) { return position == pair.first; });
}

// ReSharper disable once CppLocalVariableMayBeConst
PointH::const_iterator TmxObject::getRandomPoint(const string& layer) {
  const auto& info = point_map_.at(layer);
  auto engine = mt19937(random_device{}());
  auto distrobution = uniform_int_distribution<>(0, int(info.size()) - 1);
  auto it = info.begin();

  std::advance(it, distrobution(engine));
  return it;
}

// ReSharper disable once CppLocalVariableMayBeConst
Vec2H::const_iterator TmxObject::getRandomPosition(const string& layer) {
  const auto& info = positions_map_.at(layer);
  auto engine = mt19937(random_device{}());
  auto distrobution = uniform_int_distribution<>(0, int(info.size()) - 1);
  auto it = info.begin();

  std::advance(it, distrobution(engine));
  return it;
}
