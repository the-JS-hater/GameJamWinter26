#include <raylib.h>
#include <raymath.h>


int main()
{
  int win_w{1280}, win_h{720};
  char const *win_title{"TITLE"}; 
  InitWindow(win_w, win_h, win_title);

  struct TestPlayer 
  { 
    float x, y, w, h, speed;
  };
  
  TestPlayer player {};
  player.speed = 100.0f;
  player.w = 20.0f;
  player.h = 40.0f;
  
  //float res_w {720.0f}, res_h {480.0f};
  //float res_w {1280.0f}, res_h {720.0f};
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

    // --- Input --- //
    {
      w_input = IsKeyDown(KEY_W);
      a_input = IsKeyDown(KEY_A);
      s_input = IsKeyDown(KEY_S);
      d_input = IsKeyDown(KEY_D);
    }

    // --- Update --- //
    {
      player.y -= w_input * player.speed * dt;
      player.y += s_input * player.speed * dt;
      player.x += d_input * player.speed * dt;
      player.x -= a_input * player.speed * dt;
      
      camera.target = {
        Lerp(camera.target.x, player.x, 0.001), 
        Lerp(camera.target.y, player.y, 0.001)
      };
    }

    // --- Render --- //
    {
      BeginTextureMode(render_target);
      ClearBackground(WHITE);
      BeginMode2D(camera);
      
      // Draw scene
      DrawRectangleRec({player.x, player.y, player.w, player.h}, RED);
      DrawRectangle(100, 120, 15, 75, GREEN);

      EndMode2D();

      // Draw HUD
      int pos_x = 10, pos_y = 10, font_size = 40;
      const char *text = TextFormat(
        "FPS: %d\nX: %f\nY: %f", 
        GetFPS(), player.x, player.y
      );
      DrawText(text, pos_x, pos_y, font_size, BLACK);
      EndTextureMode();
      
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
  }
  CloseWindow();
}
