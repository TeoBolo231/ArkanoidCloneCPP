

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

/*============================================================================*/

Player::Player()
{
  if (!texture.loadFromFile("Data/Images/paddleBlue.png"))
  {
    std::cout <<"Player texture not loaded\n";
  }
  sprite.setTexture(texture);
  sprite.setScale
    (
      sprite_width/ sprite.getGlobalBounds().width,
      sprite_height/ sprite.getGlobalBounds().height
    );

  lives = start_lives;
}
Player::~Player()
{

}

/*============================================================================*/

void Player::incrementScore()
{
  score++;
}
void Player::resetScore()
{
  score = 0;
}
int Player::returnScore() const
{
  return score;
}
void Player::setScoreText()
{
  score_text.setString(std::to_string(score));
}

/*============================================================================*/
void Player::resetPos(sf::RenderWindow& window)
{
  getSprite()->setPosition
    (
      window.getSize().x/2 - getSprite()->getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/50 - getSprite()->getGlobalBounds().height
    );
}
float Player::returnPaddleSpeed() const
{
  return paddle_speed;
}
void Player::setPaddleDirection(float dir)
{
  paddle_direction.x = dir;
  paddle_direction.y = 0;
  paddle_direction.normalise();
}
Vector2 Player::returnPaddleDirection()
{
  return paddle_direction;
}
void Player::paddleMove(float dt)
{
  getSprite()->move(returnPaddleDirection().x * returnPaddleSpeed() * dt,
                    returnPaddleDirection().y * returnPaddleSpeed() * dt);
}
void Player::inBound(sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x < 0)
  {
    getSprite()->setPosition(0,getSprite()->getPosition().y);
  }
  if (getSprite()->getPosition().x > window.getSize().x - getSprite()->getGlobalBounds().width)
  {
    getSprite()->setPosition(window.getSize().x - getSprite()->getGlobalBounds().width ,
                             getSprite()->getPosition().y);
  }
}
/*============================================================================*/

 sf::Sprite* Player::getSprite()
{
  return &sprite;
}
sf::Text Player::getScoreText()
{
  return score_text;
}

/*============================================================================*/

void Player::setLives(int val)
{
  lives = lives + val;
}
int Player::returnLives() const
{
  return lives;
}
int Player::returnStartLives() const
{
  return start_lives;
}

/*============================================================================*/

float Player::returnSpriteW() const
{
  return sprite_width;
}

float Player::returnSpriteH() const
{
  return sprite_height;
}