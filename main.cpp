#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <algorithm>
#include <cmath>

#include "render.hpp"
#include "player.hpp"
#include "map.hpp"


#define TILE_SIZE 48
#define DEBUG true


enum class GameState {
  START,
  PLAYING,
  WON,
  GAME_OVER,
};


bool has_won(Player player, Vector2 goal_pos) {
  Vector2 player_pos{player.x, player.y};
  goal_pos *= TILE_SIZE;
  return Vector2Distance(player_pos, goal_pos) < TILE_SIZE;
}

bool dash = true;
  
int main()
{
  int win_w{1280}, win_h{720};
  char const *win_title{"TITLE"}; 
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(win_w, win_h, win_title);
  GameState state = GameState::START;

  float res_w {1920.0f}, res_h {1080.0f};
  float game_screen_w{1920.0f}, game_screen_h{1080.0f};
  RenderTexture2D render_target = LoadRenderTexture(game_screen_w, game_screen_h);
  SetTextureFilter(render_target.texture, TEXTURE_FILTER_POINT);

  Map test_level = Map("levels/level2.wad");
  Player player = Player(test_level.start_pos.x * TILE_SIZE, test_level.start_pos.y * TILE_SIZE);
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

  static float timer {0.0f}; 
  while(!WindowShouldClose())
  {
    float const dt = GetFrameTime();
    float const gravity = 750.0f; 
    float w_input, a_input, s_input, d_input, shift_input;

    // --- Input --- //
    {
      w_input = IsKeyPressed(KEY_W);
      a_input = IsKeyPressed(KEY_A);
      shift_input = IsKeyPressed(KEY_LEFT_SHIFT);
      //s_input = IsKeyDown(KEY_S);
      d_input = IsKeyPressed(KEY_D);

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    }

    // --- Update --- //
    if (state == GameState::PLAYING)
    { 
      timer += 1.0f * dt; 
      float const jump_impulse = 600.0f; 
      const float accel = 750.0f;
      player.max_speed = 400.0f;
    
      if (a_input) {
        player.facing = Facing::LEFT;
      }
      if (d_input) {
        player.facing = Facing::RIGHT;
      }

      if (shift_input && dash) {
        if (player.facing == Facing::RIGHT){
          player.dx += 800;
        }
        if (player.facing == Facing::LEFT){
          player.dx -= 800;
        }
        dash = false;
      }

      
      if (player.facing == Facing::RIGHT)
        player.dx += accel * dt;
      else
        player.dx -= accel * dt;
    
      // collision
      static float const tile_size = 48.0f;
      
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
      float prev_y = player.y;
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
            player.y = prev_y;
            player.y -= player.dy * dt;
            player.dy = 0.25 * -player.dy;
            jump_cooldown = 0.0f;
            dash = true;
            break;
          }
        }
      }
      // cap speed
      player.dx -= player.dx * 0.8f * dt;
      // integrate horizontal movement
      float prev_x = player.x;
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
            player.x = prev_x;
            player.x -= player.dx * dt;
            player.dx *= -0.9f;
            player.facing = player.facing == Facing::LEFT ? Facing::RIGHT : Facing::LEFT;
            jump_cooldown = 0.0f;
            break;
          }
        }
      }

      // update camera
      camera.target = {
        Lerp(camera.target.x, player.x + 96, 0.2), 
        Lerp(camera.target.y, player.y + 96, 0.2)
      };
    }
    else if (state == GameState::START) {
      state = GameState::PLAYING;
    }
    else {  // update for game over state and win state
      if (IsKeyPressed(KEY_R)) {
        // reset game
        player.x = test_level.start_pos.x * TILE_SIZE;
        player.y = test_level.start_pos.y * TILE_SIZE;
        timer = 0;
        state = GameState::PLAYING;
      }
    }
    if (DEBUG) {
      if (IsKeyPressed(KEY_R)) {
        // reset game
        player.x = test_level.start_pos.x * TILE_SIZE;
        player.y = test_level.start_pos.y * TILE_SIZE;
        timer = 0;
        state = GameState::PLAYING;
      }
    }

    if (has_won(player, test_level.goal_pos)) {
      state = GameState::WON;
    }

    // --- Render --- //
    render_scene(render_target, camera, player, test_level, timer);
    render_to_screen(render_target, game_screen_w, game_screen_h);
  }
  CloseWindow();
}
