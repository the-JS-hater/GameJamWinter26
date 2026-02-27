#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "render.hpp"
#include "player.hpp"

  
int main()
{
  int win_w{1280}, win_h{720};
  char const *win_title{"TITLE"}; 
  InitWindow(win_w, win_h, win_title);

  float res_w {1920.0f}, res_h {1080.0f};
  float game_screen_w{1920.0f}, game_screen_h{1080.0f};
  RenderTexture2D render_target = LoadRenderTexture(game_screen_w, game_screen_h);

  Player player = Player(0, -128);

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
    float const gravity = 750.0f; 

    // --- Input --- //
    {
      w_input = IsKeyPressed(KEY_W);
      a_input = IsKeyDown(KEY_A);
      //s_input = IsKeyDown(KEY_S);
      d_input = IsKeyDown(KEY_D);

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    }

    // --- Update --- //
    { 
      float const impulse = 2500.0f; 
      float const jump_impulse = 500.0f; 
      float const mass = 20.0f;
      
      static float jump_cooldown {0};

      float ax{0.0f}, ay{0.0f};
      ax = ((bool)a_input ^ (bool)d_input) * impulse / mass;
      player.dx += ax * dt;
      if (fabs(player.dx) > player.max_speed)
      {
        player.dx = player.dx < 0.0f ? 
          -player.max_speed : 
          player.max_speed;
      }
      printf("player speed: %f\n", player.dx);

      player.x += d_input * player.dx * dt;
      player.x -= a_input * player.dx * dt;
      
      if (player.y + player.h > 0.0f and player.dy > 0.0f) player.dy = 0.0f;
      if (player.y + player.h < 0.0f) 
      {
        player.dy += gravity * dt; 
      }
      jump_cooldown -= 1.0f * dt;
      if (jump_cooldown < 0.0f) jump_cooldown = 0.0f;
      if (w_input and jump_cooldown < 0.001f)
      {
        jump_cooldown += 1.0f;
        player.dy -= jump_impulse;
      }
      player.y += player.dy * dt;
      
      camera.target = {
        Lerp(camera.target.x, player.x, 0.001), 
        Lerp(camera.target.y, player.y, 0.001)
      };
    }

    // --- Render --- //
    render_scene(render_target, camera, player);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }
  CloseWindow();
}
