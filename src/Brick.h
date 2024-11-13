
#ifndef BREAKOUTSFML_BRICK_H
#define BREAKOUTSFML_BRICK_H

#include <SFML/Graphics.hpp>

class Brick
{
 public:
  Brick();
  ~Brick();

  enum Colours {Blue, Green, Grey, Purple, Red, Yellow};
  void addTexture(Colours colour);

  bool getVisibility() const;
  void setVisibility(bool val);

  float spriteWidth() const;
  float spriteHeight() const;
  int returnGridSizeX() const;
  int returnGridSizeY() const;
  int returnBrickCount() const;

  sf::Sprite* getSprite();

 private:
  bool is_visible = true;
  int brick_count = 0;

  float sprite_width = 48.;
  float sprite_height = 24.;

  int grid_size_x = 16;
  int grid_size_y = 8;

  sf::Sprite sprite;

  sf::Texture blue_texture;
  sf::Texture green_texture;
  sf::Texture grey_texture;
  sf::Texture purple_texture;
  sf::Texture red_texture;
  sf::Texture yellow_texture;
};

#endif // BREAKOUTSFML_BRICK_H
