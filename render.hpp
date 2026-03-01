#pragma once

#include <raylib.h>
#include <raymath.h>

#include "player.hpp"
#include "map.hpp"

enum class GameState {
  START,
  PLAYING,
  WON,
  GAME_OVER,
};

void init_resources();

void render_scene(
  RenderTexture2D, 
  Camera2D, 
  Player, 
  Map const&,
  float const,
  GameState const
);

void render_to_screen(
  RenderTexture2D, 
  float const, 
  float const
);
