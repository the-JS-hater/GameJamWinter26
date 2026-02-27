#pragma once

#include <raylib.h>
#include <raymath.h>


void render_scene(RenderTexture2D, Camera2D);

void render_to_screen(
  RenderTexture2D, 
  float const, 
  float const 
);
