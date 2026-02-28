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
  // DrawRectangle(-1000, 0, 3000, 2000, GREEN);
  draw_map(map);
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
