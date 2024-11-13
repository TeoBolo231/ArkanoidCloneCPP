
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Brick.h"

Brick::Brick()
{
  if (!blue_texture.loadFromFile("Data/Images/element_blue_rectangle.png") ||
      !green_texture.loadFromFile("Data/Images/element_green_rectangle.png") ||
      !grey_texture.loadFromFile("Data/Images/element_grey_rectangle.png") ||
      !purple_texture.loadFromFile("Data/Images/element_purple_rectangle.png") ||
      !red_texture.loadFromFile("Data/Images/element_red_rectangle.png") ||
      !yellow_texture.loadFromFile("Data/Images/element_yellow_rectangle.png"))
  {
    std::cout <<"Brick texture not loaded\n";
  }
}
Brick::~Brick()
{

}

/*============================================================================*/

bool Brick::getVisibility() const
{
  return is_visible;
}

void Brick::setVisibility(bool val)
{
  is_visible = val;
}

/*============================================================================*/

void Brick::addTexture(Colours colour)
{
  if (colour == Colours::Blue)
  {
    sprite.setTexture(blue_texture);
  }
  else if (colour == Colours::Green)
  {
    sprite.setTexture(green_texture);
  }
  else if (colour == Colours::Grey)
  {
    sprite.setTexture(grey_texture);
  }
  else if (colour == Colours::Purple)
  {
    sprite.setTexture(purple_texture);
  }
  else if (colour == Colours::Red)
  {
    sprite.setTexture(red_texture);
  }
  else
  {
    sprite.setTexture(yellow_texture);
  }
}
float Brick::spriteHeight() const
{
  return sprite_height;
}
float Brick::spriteWidth() const
{
  return sprite_width;
}
sf::Sprite* Brick::getSprite()
{
  return &sprite;
}
int Brick::returnGridSizeY() const
{
  return grid_size_y;
}
int Brick::returnGridSizeX() const
{
  return grid_size_x;
}
int Brick::returnBrickCount() const
{
  return brick_count;
}