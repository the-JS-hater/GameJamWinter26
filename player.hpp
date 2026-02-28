#pragma once

enum class Facing 
{
  LEFT,
  RIGHT
};

struct Player 
{ 
  float x,y,w,h;
  float dx,dy;
  float max_speed;
  Facing facing;

  Player(float x, float y) : 
    x{x}, y{y}, w{64.0f}, h{128.0f}, 
    dx{0.0f}, dy{0.0f}, max_speed{400.0f},
    facing{Facing::RIGHT}
    { }
};
