
#ifndef BREAKOUTSFML_BALL_H
#define BREAKOUTSFML_BALL_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "Player.h"
#include "Brick.h"
#include <cmath>

class Ball
{
 public:
  Ball();
  ~Ball();

  sf::Sprite* getSprite();
  float returnSpeed() const;
  void setDirection(float dir_x, float dir_y);
  Vector2 returnDirection();

  void move(sf::RenderWindow& window, Player *player, float dt);
  void inBound(sf::RenderWindow& window);
//  void brickInBound(Brick *brick);
  void wallBounce(sf::RenderWindow& window);
  void brickBounce(Brick *brick);
  void paddleBounce(Player *player);
  void loseLife(sf::RenderWindow& window, Player *player);
  void setBallStarted(bool val);
  bool returnBallStarted() const;
  void resetPos(sf::RenderWindow& window, Player *player);
  float returnCornerBounce() const;

 private:
  sf::Sprite sprite;
  sf::Texture texture;

  Vector2 direction = {0, 0};
  float corner_bounce = sqrt(2)/2;
  float speed = 400.f;
  float sprite_width = 24.;
  float sprite_height = 24.;
  bool ball_started;
};

#endif // BREAKOUTSFML_BALL_H
