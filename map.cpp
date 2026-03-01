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
  file >> this->start_pos.x;
  file >> this->start_pos.y;
  file >> this->goal_pos.x;
  file >> this->goal_pos.y;
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

TileType Map::get(int x, int y) const {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return TileType::EMPTY;
  }
  return tiles[y][x];
}

std::vector<Rectangle> Map::get_colliders(float tile_size) const {
  std::vector<Rectangle> colliders;
  for (int local_y = 0; local_y < height; ++local_y) {
    for (int local_x = 0; local_x < width; ++local_x) {
      if (get(local_x, local_y) != TileType::EMPTY) {
        float global_x = local_x * tile_size;
        float global_y = local_y * tile_size;
        colliders.push_back(Rectangle{
          global_x,
          global_y,
          tile_size,
          tile_size
        });
      }
    }
  }
  return colliders;
}