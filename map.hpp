#pragma once 

#include <raylib.h>
#include <vector>
#include <string>

enum class TileType {
  EMPTY,
  GROUND
};

struct Map {
  std::vector<std::vector<TileType>> tiles;
  int width, height;
  Vector2 start_pos;
  Vector2 goal_pos;

  Map();
  Map(int width, int height);
  Map(std::string file_name);
  TileType get(int x, int y) const;
  void draw();
  std::vector<Rectangle> get_colliders(float tile_size) const;
};
