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

  Player player = Player(300, 400);
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
      float const jump_impulse = 500.0f; 
      float const accel = 500.0f;
    
      if (a_input) player.facing = Facing::LEFT;
      if (d_input) player.facing = Facing::RIGHT;
      
      if (player.facing == Facing::RIGHT)
        player.dx +=  accel * dt;
      else
        player.dx -=  accel * dt;
    
      // collision
      static float const tile_size = 64.0f;
      
      int const rows = test_level.tiles.size();
      int const cols = test_level.tiles.empty() ? 
          0 : test_level.tiles[0].size();
      printf("%d\n", std::max(-1, (int)std::floor(player.x / tile_size)));
      int const left = std::max(-1, (int)std::floor(player.x / tile_size));
      int const right =
        std::min(cols - 1,(int)std::floor((player.x + player.w) / tile_size));
      int const top = std::max(0, (int)std::floor(player.y / tile_size));
      int const bottom =
        std::min(rows - 1, (int)std::floor((player.y + player.h) / tile_size));
      
      // jumping
      static float jump_cooldown {0};
      static bool is_grounded {false};

      jump_cooldown -= 1.0f * dt;
      if (jump_cooldown < 0.0f) jump_cooldown = 0.0f;
      if (w_input and jump_cooldown < 0.001f)
      {
        jump_cooldown += 1.0f;
        player.dy -= jump_impulse;
        is_grounded = false;
      }

      player.dy += gravity * dt; 
      { // vertical collision
        bool top_collision = 
          (int)test_level.get(left, top)
          or 
          (int)test_level.get(right, top);
        bool bottom_collision = 
          (int)test_level.get(left, bottom)
          or 
          (int)test_level.get(right, bottom);
        if (top_collision) player.dy = std::fmax(0.0f, player.dy);
        if (bottom_collision) 
        {
          player.dy = std::fmin(0.0f, player.dy);
          is_grounded = true;
        } else {
          is_grounded = false;
        }
      }
      player.y += player.dy * dt;
      { // horizontal collision
        bool left_collision = 
          (int)test_level.get(left, top)
          or 
          (!is_grounded and (int)test_level.get(left, bottom));
        if (player.facing == Facing::LEFT and left_collision)
        {
          player.facing = Facing::RIGHT;
          player.dx *= -1;
        }
        
        bool right_collision = 
          (int)test_level.get(right, top)
          or 
          (!is_grounded and (int)test_level.get(right, bottom));
        if (player.facing == Facing::RIGHT and right_collision)
        {
          player.facing = Facing::LEFT;
          player.dx *= -1;
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
