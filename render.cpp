#include "render.hpp"
#include "map.hpp"

void draw_map(Map const& map) {
  static float tile_size = 128.0f;
  for (int y = 0; y < map.height; ++y)
    for (int x = 0; x < map.width; ++x)
    {
      if (!(int)map.tiles[y][x]) continue;
      DrawRectangleRec(
        {
          x * tile_size, 
          y * tile_size,
          tile_size,
          tile_size
        },
        GREEN
      );
    }
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
  
  //  Draw ground
  draw_map(map);

  static float const tile_size = 128.0f;
  
  int const rows = map.tiles.size();
  int const cols = map.tiles.empty() ? 
      0 : map.tiles[0].size();
  int const left = std::max(0, (int)std::floor(player.x / tile_size));
  int const right =
    std::min(cols - 1,(int)std::floor((player.x + player.w) / tile_size));
  int const top = std::max(0, (int)std::floor(player.y / tile_size));
  int const bottom =
    std::min(rows - 1, (int)std::floor((player.y + player.h) / tile_size));
  
  DrawRectangleLinesEx(
    {(left) * tile_size, top * tile_size, tile_size, tile_size},
    3, BLUE
  );
  DrawRectangleLinesEx(
    {(left) * tile_size, bottom * tile_size, tile_size, tile_size},
    3, BLUE        
  );
  DrawRectangleLinesEx(
    {(right) * tile_size, top * tile_size, tile_size, tile_size}, 
    3, BLUE
  );
  DrawRectangleLinesEx(
    {(right) * tile_size, bottom * tile_size, tile_size, tile_size},
    3, BLUE    
  );

  DrawRectangleRec({player.x, player.y, player.w, player.h}, RED);
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
