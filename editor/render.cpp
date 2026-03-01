#include "render.hpp"


inline Color get_color_of(TileType tileType) {
  switch (tileType) {
    case TileType::EMPTY: {
      return WHITE;
    }
    case TileType::GROUND: {
      return BROWN;
    }
    default: {
      TraceLog(LOG_ERROR, "Tried to get color of unknown tile in get_color_of()");
      return PURPLE;
    }
  }
}


void render_scene(RenderTexture2D render_target, Camera2D camera, Map map)
{
  BeginTextureMode(render_target);
  ClearBackground(WHITE);
  BeginMode2D(camera);

  int tile_size = 48;
  
  for (int local_y = 0; local_y < map.height; ++local_y) {
    int world_y = local_y * tile_size;
    for (int local_x = 0; local_x < map.width; ++local_x) {
      int world_x = local_x * tile_size;
      Color color = get_color_of(map.get(local_x, local_y));
      DrawRectangle(world_x, world_y, tile_size, tile_size, color);
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
  Rectangle src = {
    0.0f,
    0.0f,
    (float)render_target.texture.width,
    -(float)render_target.texture.height
  };
  Rectangle dst = {
    0.0f,
    0.0f,
    (float)render_target.texture.width,
    -(float)render_target.texture.height
  };
  
  DrawTexturePro(render_target.texture, src, dst, {0, 0}, 0.0f, WHITE);

  EndDrawing();
}
