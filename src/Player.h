
#ifndef BREAKOUTSFML_PLAYER_H
#define BREAKOUTSFML_PLAYER_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Player
{
 public:
  Player();
  ~Player();

  void incrementScore();
  void resetScore();
  int returnScore() const;
  void setScoreText();

  void setLives(int val);
  int returnLives() const;
  int returnStartLives() const;
  void resetPos(sf::RenderWindow& window);

  void inBound(sf::RenderWindow& window);

  sf::Sprite* getSprite();
  sf::Text getScoreText();

  float returnPaddleSpeed() const;

  void setPaddleDirection(float dir);
  Vector2 returnPaddleDirection();
  void paddleMove(float dt);

  float returnSpriteW() const;
  float returnSpriteH() const;

 private:
  sf::Sprite sprite;
  sf::Text score_text;

  Vector2 paddle_direction = {0, 0};

  sf::Texture texture;
  int score = 0;
  int lives;
  int start_lives = 2;
  float paddle_speed = 500.f;
  float sprite_width = 128.;
  float sprite_height = 16.;
};

#endif // BREAKOUTSFML_PLAYER_H
