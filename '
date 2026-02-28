#pragma once 

#include <vector>
#include <string>

enum class TileType {
  EMPTY,
  GROUND
};

struct Map {
  std::vector<std::vector<TileType>> tiles;
  int width, height;

  Map();
  Map(int width, int height);
  Map(std::string file_name);
  void draw();
};
