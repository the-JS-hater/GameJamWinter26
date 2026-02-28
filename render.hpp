#pragma once

#include <raylib.h>
#include <raymath.h>

#include "player.hpp"
#include "map.hpp"


void render_scene(
  RenderTexture2D, 
  Camera2D, 
  Player, 
  Map const&
);

void render_to_screen(
  RenderTexture2D, 
  float const, 
  float const
);
