#pragma once

#include <raylib.h>
#include <raymath.h>
#include "map_editor.hpp"


void render_scene(RenderTexture2D, Camera2D, Map);

void render_to_screen(
  RenderTexture2D, 
  float const, 
  float const 
);
