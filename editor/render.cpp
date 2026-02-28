#include "render.hpp"


void render_scene(RenderTexture2D render_target, Camera2D camera, Map map)
{
  BeginTextureMode(render_target);
  ClearBackground(WHITE);
  BeginMode2D(camera);

  int tile_width = GetScreenWidth() / map.width;
  int tile_height = GetScreenHeight() / map.height;
  
  for (int local_y = 0; local_y < map.height; ++local_y) {
    int world_y = local_y * tile_height;
    for (int local_x = 0; local_x < map.width; ++local_x) {
      int world_x = local_x * tile_width;
      DrawRectangle(world_x, world_y, )
    }
  }

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
