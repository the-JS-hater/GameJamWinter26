#include "map.hpp"
#include "player.hpp"
#include "raylib.h"
#include <fstream>
#include <iostream>
#include <cmath>



std::istream& operator>>(std::istream& is, TileType& tile) {
  int tile_type;
  is >> tile_type;
  tile = (TileType)tile_type; 
  return is;
}

Map::Map(){}

Map::Map(int width, int height) : width(width), height(height) {
  for (int y = 0; y < width; y++) {
    std::vector<TileType> row;
    for (int x = 0; x < height; x++) {
      row.push_back(TileType::EMPTY);
    }
    tiles.push_back(row);
  }
}

Map::Map(std::string file_name) {
  std::ifstream file;
  file.open(file_name);
  file >> this->width;
  file >> this->height;
  for (int y = 0; y < this->width; y++) {
    std::vector<TileType> row;
    for (int x = 0; x < this->height; x++) {
      TileType tile;
      file >> tile;
      row.push_back(tile);
    }
    this->tiles.push_back(row);
  }
  file.close();
}
