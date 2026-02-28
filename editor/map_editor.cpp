#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "map_editor.hpp"
#include "render.hpp"

std::ostream& operator<<(std::ostream& os, const TileType& tile) {
  return os << (int)tile;
}

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
  file >> width;
  file >> height;
  for (int y = 0; y < width; y++) {
    std::vector<TileType> row;
    for (int x = 0; x < height; x++) {
      TileType tile;
      file >> tile;
      row.push_back(tile);
    }
    tiles.push_back(row);
  }
  file.close();
}

void Map::set(int x, int y, TileType tile) {
  tiles[y][x] = tile;
}

TileType Map::get(int x, int y) const {
  return tiles[y][x];
}

void Map::write_to_file(std::string file_name) const {
  std::ofstream file;
  file.open(file_name);
  file << *this;
  file.close();
}

std::ostream& operator<<(std::ostream& os, Map const&  map) {
  os << map.width << std::endl;
  os << map.height << std::endl;
  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      os << map.get(x, y) << " ";
    }
    os << std::endl;
  }
}

void handle_mouse(Map& map) {
  if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    return;
  }

  const double tile_size = 32;

  int mouse_x = GetMouseX();
  int mouse_y = GetMouseY();

  int tile_x = mouse_x / tile_size;
  int tile_y = mouse_y / tile_size;

  map.set(tile_x, tile_y, TileType::GROUND);
}

int main(int argc, char* argv[])
{
  int win_w{1280}, win_h{720};
  char const *win_title{"TITLE"}; 
  InitWindow(win_w, win_h, win_title);

  Map map;
  std::string file_name;

  if (argc == 2) {
    //Read from file
    map = Map(argv[1]);
    file_name = argv[1];
  }
  else if (argc == 4) {
    // Create new file
    map = Map(std::stoi(argv[1]), std::stoi(argv[2]));
    file_name = argv[3];
  }
  else {
    std::cout << "Wrong number of args!" << std::endl;
    return -1;
  }

  float res_w {1920.0f}, res_h {1080.0f};
  float game_screen_w{1920.0f}, game_screen_h{1080.0f};
  RenderTexture2D render_target = LoadRenderTexture(game_screen_w, game_screen_h);

  Camera2D camera {
    { game_screen_w / 2.0f, game_screen_h / 2.0f }, // Offset
    { // Camera target (rotation and zoom origin)
      game_screen_w / 2.0f, 
      game_screen_h / 2.0f
    },
    0.0f, // Camera rotation in degrees
    1.0f // Camera zoom (scaling) 
  };

  while(!WindowShouldClose())
  {
    float w_input, a_input, s_input, d_input;
    float const dt = GetFrameTime();

    handle_mouse(map);

    // --- Render --- //
    render_scene(render_target, camera, map);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }

  map.write_to_file(file_name);
  CloseWindow();
}
