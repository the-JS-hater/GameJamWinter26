#include <raylib.h>
#include <raymath.h>

#include "render.hpp"

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

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();
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
    render_scene(render_target, camera);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }
  CloseWindow();
}
