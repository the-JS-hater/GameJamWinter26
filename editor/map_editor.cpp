#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "map_editor.hpp"
#include "render.hpp"

int const tile_size = 48;

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
  for (int y = 0; y < height; y++) {
    std::vector<TileType> row;
    for (int x = 0; x < width; x++) {
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
  for (int y = 0; y < height; y++) {
    std::vector<TileType> row;
    for (int x = 0; x < width; x++) {
      TileType tile;
      file >> tile;
      row.push_back(tile);
    }
    tiles.push_back(row);
  }
  file.close();
}

void Map::set(int x, int y, TileType tile) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    TraceLog(LOG_WARNING, "Tried to set tile on map at (%d, %d) to %d; but it was outside of map.", x, y, tile);
    return;
  }
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

void handle_mouse(Map& map, Camera2D& camera, RenderTexture2D render_target) {
  if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    return;
  }


  Vector2 world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

  int tile_x = world_pos.x / tile_size;
  int tile_y = world_pos.y / tile_size;
  int value = (int)map.get(tile_x, tile_y);
  ++value %= (int)TileType::ENDOFTYPES;
  map.set(tile_x, tile_y, (TileType)value);
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
  RenderTexture2D render_target = LoadRenderTexture((int)game_screen_w, (int)game_screen_h);

  Camera2D camera {
    { 0, 0 }, // Offset
    { // Camera target (rotation and zoom origin)
      0, 0
    },
    0.0f, // Camera rotation in degrees
    1.0f // Camera zoom (scaling) 
  };

  while(!WindowShouldClose())
  {
    float w_input, a_input, s_input, d_input;
    float const dt = GetFrameTime();

    w_input = IsKeyDown(KEY_W);
    a_input = IsKeyDown(KEY_A);
    s_input = IsKeyDown(KEY_S);
    d_input = IsKeyDown(KEY_D);

    int dx = 0;
    int dy = 0;
    if (w_input) {
      ++dy;
    }
    if (a_input) {
      ++dx;
    }
    if (s_input) {
      --dy;
    }
    if (d_input) {
      --dx;
    }

    const int camera_move_speed = 100;
    float next_x = camera.offset.x + dx * camera_move_speed * dt;
    float next_y = camera.offset.y + dy * camera_move_speed * dt;
    std::cout << next_x << next_y << std::endl;
    std::cout << game_screen_w*2 - (tile_size * map.width)*3 - 10 << std::endl;
    std::cout << game_screen_h * 2 - (tile_size * map.height) * 3 << std::endl;
    if (next_x > 10 || next_x < game_screen_w * 2 - (tile_size * map.width) * 3 - 10) {
      next_x = camera.offset.x;
    }
    if (next_y > 10 || next_y < game_screen_h * 2 - (tile_size * map.height) * 3 - 10) {
      next_y = camera.offset.y;
    }
    camera.offset.x = next_x;
    camera.offset.y = next_y;

    handle_mouse(map, camera, render_target);

    // --- Render --- //
    render_scene(render_target, camera, map);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }

  map.write_to_file(file_name);
  CloseWindow();
}
