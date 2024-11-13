
#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Vector2.h"
#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event) const;

  // Objects
  Player* player;
  Ball* ball;
  Brick* brick = nullptr;
  Brick* bricks = nullptr;


 private:

  void setStartScreen();
  void setGameScreen();
  void setEndScreen();
  void setPauseMenu();

  void startScreenBG();
  void startScreenGameTitle();
  void startScreenRules();
  void startScreenPlay();
  void startScreenQuit();

  void gameScreenBG();
  void gameScreenPlayer();
  void gameScreenBall();
  void levelOne();

  void endScreenBG();
  void endScreenText();
  void endScreenPlay();
  void endScreenQuit();

  void pauseMenuBG();
  void pauseMenuText();
  void pauseMenuRestart();
  void pauseMenuQuit();

  static bool collisionCheckText(sf::Vector2i click, sf::Text text);
  void endGameManager();

  // General
  sf::Font font_game;
  bool play_selected = true;
  sf::RenderWindow& window;

  // Game States
  bool start_menu;
  bool pause_menu;
  bool end_screen;
  bool game_running;

  // Start Menu
  sf::Text start_menu_title_text;

  sf::Sprite start_menu_bg_sprite;
  sf::Texture start_menu_bg_texture;

  sf::Text start_menu_play_option;
  sf::Text start_menu_quit_option;

  sf::Text rules_text;

  // Pause Menu
  sf::Sprite background_pause_sprite;
  sf::Texture background_pause_texture;

  sf::Text pause_title_text;

  sf::Text restart_opt_pause_text;
  sf::Text quit_opt_pause_text;

  // End Screen
  sf::Sprite end_bg_sprite;
  sf::Texture end_bg_texture;

  sf::Text end_title_text;

  sf::Text play_again_end_text;
  sf::Text quit_end_text;

  // Game
  sf::Sprite game_bg_sprite;
  sf::Texture game_bg_texture;
};

#endif // BREAKOUT_GAME_H
