#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <algorithm>
#include <cmath>

#include "render.hpp"
#include "player.hpp"
#include "map.hpp"

  
int main()
{
  int win_w{1280}, win_h{720};
  char const *win_title{"TITLE"}; 
  InitWindow(win_w, win_h, win_title);

  float res_w {1920.0f}, res_h {1080.0f};
  float game_screen_w{1920.0f}, game_screen_h{1080.0f};
  RenderTexture2D render_target = LoadRenderTexture(game_screen_w, game_screen_h);

  Player player = Player(100, 100);
  Map test_level = Map("levels/test.wad");
  printf("Size of map: %d * %d\n", test_level.width, test_level.height);

  Camera2D camera {
    { game_screen_w / 2.0f, game_screen_h / 2.0f }, // Offset
    { // Camera target (rotation and zoom origin)
      game_screen_w / 2.0f, 
      game_screen_h / 2.0f
    },
    0.0f, // Camera rotation in degrees
    1.0f // Camera zoom (scaling) 
  };

  init_resources();

  while(!WindowShouldClose())
  {
    float w_input, a_input, s_input, d_input;
    float const dt = GetFrameTime();
    float const gravity = 750.0f; 

    // --- Input --- //
    {
      w_input = IsKeyPressed(KEY_W);
      a_input = IsKeyPressed(KEY_A);
      //s_input = IsKeyDown(KEY_S);
      d_input = IsKeyPressed(KEY_D);

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    }

    // --- Update --- //
    { 
      float const jump_impulse = 600.0f; 
      float const accel = 500.0f;
    
      if (a_input) player.facing = Facing::LEFT;
      if (d_input) player.facing = Facing::RIGHT;
      
      if (player.facing == Facing::RIGHT)
        player.dx +=  accel * dt;
      else
        player.dx -=  accel * dt;
    
      // collision
      static float const tile_size = 96.0f;
      
      // jumping
      static float jump_cooldown {0};
      static bool is_grounded {false};

      jump_cooldown -= 1.0f * dt;
      if (jump_cooldown < 0.0f) jump_cooldown = 0.0f;
      if (w_input && jump_cooldown < 0.001f)
      {
        printf("jump");
        jump_cooldown = 1.0f;
        player.dy -= jump_impulse;
        is_grounded = false;
      }
      else {
        is_grounded = true;
      }

      player.dy += gravity * dt; 
      player.y += player.dy * dt;
      { // vertical collision
        Rectangle player_rect;
        if (player.dy > 0) {
          player_rect = {player.x + player.w / 4, player.y + player.h, player.w / 2, 1};
        }
        else {
          player_rect = {player.x + player.w / 4, player.y - 1, player.w / 2, 1};
        }
        for (Rectangle ground_rect : test_level.get_colliders(tile_size)) {
          if (CheckCollisionRecs(player_rect, ground_rect)) {
            printf("vert coll at (%f, %f)\n", ground_rect.x, ground_rect.y);
            player.y -= player.dy * dt;
            player.dy = 0;
            break;
          }
        }
      }
      // cap speed
      if (fabs(player.dx) > player.max_speed)
      {
        player.dx = player.dx < 0.0f ? 
          -player.max_speed : 
          player.max_speed;
      }
      // integrate horizontal movement
      player.x += player.dx * dt;
      { // horizontal collision
        Rectangle player_rect;
        if (player.dx > 0) {
          player_rect = {player.x + player.w, player.y + 1, 1, player.h - 2};
        }
        else {
          player_rect = {player.x - 1, player.y + 1, 1, player.h - 2};
        }
        for (Rectangle ground_rect : test_level.get_colliders(tile_size)) {
          if (CheckCollisionRecs(player_rect, ground_rect)) {
            printf("horz coll at (%f, %f)\n", ground_rect.x, ground_rect.y);
            player.x += player.dx * dt;
            player.dx *= -1;
            player.dx = 0;
            break;
          }
        }
      }

      // update camera
      camera.target = {
        Lerp(camera.target.x, player.x, 0.001), 
        Lerp(camera.target.y, player.y, 0.001)
      };
    }

    // --- Render --- //
    render_scene(render_target, camera, player, test_level);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }
  CloseWindow();
}
