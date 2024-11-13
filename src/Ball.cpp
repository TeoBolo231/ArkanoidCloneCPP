
#include "Ball.h"
#include <iostream>

/*============================================================================*/

Ball::Ball()
{
  if (!texture.loadFromFile("Data/Images/ball.png"))
  {
    std::cout <<"Ball texture not loaded\n";
  }
  sprite.setTexture(texture);
  sprite.setScale
    (
      sprite_width/ sprite.getGlobalBounds().width,
      sprite_height/ sprite.getGlobalBounds().height
    );
  ball_started = false;
}
Ball::~Ball()
{

}

/*============================================================================*/

sf::Sprite* Ball::getSprite()
{
  return &sprite;
}
float Ball::returnSpeed() const
{
    return speed;
}
void Ball::resetPos(sf::RenderWindow& window, Player *player)
{
  getSprite()->setPosition
    (
      window.getSize().x/2 - getSprite()->getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/50 - player->getSprite()->getGlobalBounds().height - getSprite()->getGlobalBounds().height
    );
}
void Ball::setDirection(float dir_x, float dir_y)
{
  direction.x = dir_x;
  direction.y = dir_y;
  direction.normalise();
}
Vector2 Ball::returnDirection()
{
  return direction;
}

void Ball::move(sf::RenderWindow& window, Player *player, float dt)
{
  if (!ball_started)
  {
    if (getSprite()->getPosition().x < player->returnSpriteW()/2 - sprite_width/2)
    {
      getSprite()->setPosition(player->returnSpriteW()/2 - getSprite()->getGlobalBounds().width/2,
                               window.getSize().y - window.getSize().y/50 - player->returnSpriteH() - getSprite()->getGlobalBounds().height);
    }
    if (getSprite()->getPosition().x > window.getSize().x - player->returnSpriteW()/2 - sprite_width/2)
    {
      getSprite()->setPosition(window.getSize().x - player->returnSpriteW()/2 - sprite_width/2,
                               window.getSize().y - window.getSize().y/50 - player->returnSpriteH() - getSprite()->getGlobalBounds().height);
    }
    getSprite()->move(returnDirection().x * player->returnPaddleSpeed() * dt,
                      returnDirection().y * player->returnPaddleSpeed() * dt);
  }
  else
  {
    getSprite()->move(returnDirection().x * returnSpeed() * dt,
                      returnDirection().y * returnSpeed() * dt);
  }

}
void Ball::inBound(sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x < 0)
  {
    getSprite()->setPosition(0, getSprite()->getPosition().y);
  }

  if (getSprite()->getPosition().x > window.getSize().x - sprite_width)
  {
    getSprite()->setPosition(window.getSize().x - sprite_width, getSprite()->getPosition().y);
  }

  if (getSprite()->getPosition().y < 0)
  {
    getSprite()->setPosition(getSprite()->getPosition().x, 0);
  }
}
//void Ball::brickInBound(Brick* brick)
//{
//  for (int i = 0; i < brick->returnGridSizeX() * brick->returnGridSizeY(); ++i)
//  {
//    if (getSprite()->getPosition().x > brick[i].getSprite()->getPosition().x - sprite_width)
//    {
//      getSprite()->setPosition(brick[i].getSprite()->getPosition().x - sprite_width,
//                               getSprite()->getPosition().y);
//    }
//    if (getSprite()->getPosition().x < brick[i].getSprite()->getPosition().x + brick[i].spriteWidth())
//    {
//      getSprite()->setPosition(brick[i].getSprite()->getPosition().x + brick[i].spriteWidth(),
//                               getSprite()->getPosition().y);
//    }
//    if (getSprite()->getPosition().y > brick[i].getSprite()->getPosition().y - sprite_height)
//    {
//      getSprite()->setPosition(getSprite()->getPosition().x,
//                               brick[i].getSprite()->getPosition().y - sprite_height);
//    }
//    if (getSprite()->getPosition().y < brick[i].getSprite()->getPosition().y + brick[i].spriteHeight())
//    {
//      getSprite()->setPosition(getSprite()->getPosition().x,
//                               brick[i].getSprite()->getPosition().y);
//    }
//  }
//}
void Ball::wallBounce(sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x <= 0 || getSprite()->getPosition().x >= window.getSize().x - getSprite()->getGlobalBounds().width  )
  {
    setDirection(returnDirection().x * -1, returnDirection().y);
  }

  else if (getSprite()->getPosition().y <= 0)
  {
    setDirection(returnDirection().x, returnDirection().y * -1);
  }
}
void Ball::paddleBounce(Player *player)
{
  if (getSprite()->getGlobalBounds().intersects(player->getSprite()->getGlobalBounds()))
  {
    if ((player->returnPaddleDirection().x > 0 && returnDirection().x < 0) || (player->returnPaddleDirection().x < 0 && returnDirection().x > 0))
    {

      setDirection(returnDirection().x * -1, returnDirection().y * -1);
    }
    else
    {
      setDirection(returnDirection().x, returnDirection().y * -1);
    }
  }
}

void Ball::brickBounce(Brick *brick)
{
  for (int i = 0; i < brick->returnGridSizeX() * brick->returnGridSizeY(); i++)
  {
    if (getSprite()->getGlobalBounds().intersects(brick[i].getSprite()->getGlobalBounds()))
    {
      if (getSprite()->getPosition().x + sprite_width/2 <= brick[i].getSprite()->getPosition().x &&
          getSprite()->getPosition().y + sprite_height/2 >= brick[i].getSprite()->getPosition().y + brick[i].spriteHeight() &&
          brick[i].getVisibility())
      {
        setDirection(-1 * corner_bounce, corner_bounce);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
      else if (getSprite()->getPosition().x + sprite_width/2 >= brick[i].getSprite()->getPosition().x + brick[i].spriteWidth() &&
               getSprite()->getPosition().y + sprite_height/2 >= brick[i].getSprite()->getPosition().y + brick[i].spriteHeight() &&
               brick[i].getVisibility())
      {
        setDirection(corner_bounce , corner_bounce);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
      else if (getSprite()->getPosition().x + sprite_width/2 >= brick[i].getSprite()->getPosition().x + brick[i].spriteWidth() &&
               getSprite()->getPosition().y + sprite_height/2 <= brick[i].getSprite()->getPosition().y &&
               brick[i].getVisibility())
      {
        setDirection(corner_bounce , -1 * corner_bounce);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
      else if (getSprite()->getPosition().x + sprite_width/2 <= brick[i].getSprite()->getPosition().x &&
               getSprite()->getPosition().y + sprite_height/2 <= brick[i].getSprite()->getPosition().y &&
               brick[i].getVisibility())
      {
        setDirection(-1 * corner_bounce , -1 * corner_bounce);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
      else if (getSprite()->getPosition().x + sprite_width/2 >= brick[i].getSprite()->getPosition().x &&
          getSprite()->getPosition().x + sprite_width/2 <= brick[i].getSprite()->getPosition().x + brick[i].spriteWidth() &&
          brick[i].getVisibility())
      {
        setDirection(returnDirection().x, returnDirection().y * -1);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
      else if (getSprite()->getPosition().y + sprite_height/2 >= brick[i].getSprite()->getPosition().y &&
               getSprite()->getPosition().y + sprite_height/2 <= brick[i].getSprite()->getPosition().y + brick[i].spriteHeight() &&
               brick[i].getVisibility())
      {
        setDirection(returnDirection().x * -1, returnDirection().y);
        brick[i].setVisibility(false);
        std::cout<<"collision\n";
      }
    }
  }
}

void Ball::loseLife(sf::RenderWindow& window, Player *player)
{
  if (getSprite()->getPosition().y >= window.getSize().y)
  {
    player->setLives(-1);
    if (player->returnLives() > 0)
    {
      resetPos(window, player);
      player->resetPos(window);
      setBallStarted(false);
      setDirection(0,0);
    }
  }

}

void Ball::setBallStarted(bool val)
{
  ball_started = val;
}
bool Ball::returnBallStarted() const
{
  return ball_started;
}
float Ball::returnCornerBounce() const
{
  return corner_bounce;
}
