#pragma once

#include <raylib.h>
#include <raymath.h>

#include "player.hpp"

void render_scene(RenderTexture2D, Camera2D, Player);

void render_to_screen(
  RenderTexture2D, 
  float const, 
  float const 
);
