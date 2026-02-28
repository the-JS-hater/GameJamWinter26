#include "render.hpp"
#include "map.hpp"
#include <cmath>

#define tile_size 96
#define player_size tile_size * 2
#define player_animation_frames 4

Texture2D player_moving_right_tex;
Texture2D ground_tex;
Texture2D breakable_ground_tex;

int const mountainAmount = 3;
Texture2D mountains[mountainAmount];

Texture2D scale(Texture2D tex, int width, int height) {
  Image im = LoadImageFromTexture(tex);
  ImageResize(&im, width, height);
  return LoadTextureFromImage(im);
}

void init_resources() {
  player_moving_right_tex = scale(LoadTexture("assets/player_moving_right.png"), player_size * player_animation_frames, player_size);
  ground_tex = scale(LoadTexture("assets/ground.png"), tile_size, tile_size);
  breakable_ground_tex = scale(LoadTexture("assets/breakable_ground.png"), tile_size, tile_size);

  mountains[0] = scale(LoadTexture("assets/mountains1.png"), 1920, 1080);
  mountains[1] = scale(LoadTexture("assets/mountains2.png"), 1920, 1080);
  mountains[2] = scale(LoadTexture("assets/mountains3.png"), 1920, 1080);
} 

void draw_background(Camera2D camera){
    float const parallax = -0.5;
    for(int i = 0; i < mountainAmount; i++){
      for(int j = -2; j < 2; j++){
        DrawTexture(mountains[i], camera.target.x + fmod(camera.target.x * (i + 1) * parallax, 1920) + j * 1920, camera.target.y + 40*i - camera.offset.y, WHITE);
      }
    }
  return;
}

void draw_map(Map const& map) {
  for (int y = 0; y < map.height; ++y)
    for (int x = 0; x < map.width; ++x)
    {
      Texture2D* tex;
      switch (map.get(x, y)) {
        case TileType::GROUND: {
          tex = &ground_tex;
          break;
        }
        case TileType::EMPTY: {
          continue;
        }
        default: {
          TraceLog(LOG_WARNING, "Tried to draw unknown tiletype in draw_map()");
          continue;
        }
      }
      int world_x = x * tile_size;
      int world_y = y * tile_size;
      DrawTexture(*tex, world_x, world_y, WHITE);
    }
}

void render_player(Player const& player) {
  const int fps = 12;

  int frame = ((int)(GetTime() * fps)) % player_animation_frames;
  Rectangle current_frame{frame * player_size, 0, player_size, player_size};
  DrawTextureRec(
    player_moving_right_tex, 
    current_frame, 
    Vector2{player.x - 0.5f * player.w, player.y - 0.5f * player.h}, 
    WHITE
  );
  DrawRectangleLinesEx(
    {player.x, player.y, player.w, player.h},
    3,
    GREEN
  );
}

void render_scene(
  RenderTexture2D render_target, 
  Camera2D camera, 
  Player player,
  Map const& map
) {
  BeginTextureMode(render_target);
  ClearBackground(WHITE);
  BeginMode2D(camera);

  draw_background(camera);
  
  //  Draw ground
  draw_map(map);
  
  int const rows = map.tiles.size();
  int const cols = map.tiles.empty() ? 
      0 : map.tiles[0].size();
  int const left = std::max(0, (int)std::floor(player.x / tile_size));
  int const right =
    std::min(cols - 1,(int)std::floor((player.x + player.w) / tile_size));
  int const top = std::max(0, (int)std::floor(player.y / tile_size));
  int const bottom =
    std::min(rows - 1, (int)std::floor((player.y + player.h) / tile_size));
  
  render_player(player);
  EndMode2D();

  // Draw HUD
  int pos_x = 10, pos_y = 10, font_size = 40;
  const char *text = TextFormat(
    "FPS: %d", 
    GetFPS()
  );
  DrawText(text, pos_x, pos_y, font_size, BLACK);
  EndTextureMode();
}


void render_to_screen(
  RenderTexture2D render_target, 
  float const game_screen_w, 
  float const game_screen_h
) {
  // render to screen
  BeginDrawing();
  ClearBackground(BLACK);
  float scale = fmin(
    GetScreenWidth() / game_screen_w,
    GetScreenHeight() / game_screen_h
  );

  float scaled_w = game_screen_w * scale;
  float scaled_h = game_screen_h * scale;
  float offset_x = (GetScreenWidth() - scaled_w) * 0.5f;
  float offset_y = (GetScreenHeight() - scaled_h) * 0.5f;

  Rectangle src = {
    0.0f,
    0.0f,
    (float)render_target.texture.width,
    -(float)render_target.texture.height
  };
  Rectangle dst = { offset_x, offset_y, scaled_w, scaled_h };
  
  DrawTexturePro(render_target.texture, src, dst, {0, 0}, 0.0f, WHITE);

  EndDrawing();
}
