#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

enum class TileType {
  EMPTY,
  GROUND,
  ENDOFTYPES
};

std::ostream& operator<<(std::ostream& os, const TileType& tile);
std::istream& operator>>(std::istream& is, TileType& tile);

struct Map {
  std::vector<std::vector<TileType>> tiles;
  int width, height;

  Map();
  Map(int width, int height);
  Map(std::string file_name);

  void set(int x, int y, TileType tile);
  TileType get(int x, int y) const;

  void write_to_file(std::string file_name) const;
};

std::ostream& operator<<(std::ostream& os, Map const& map);