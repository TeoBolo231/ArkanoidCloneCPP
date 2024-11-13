
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(nullptr));

  start_menu = true;
  pause_menu = false;
  end_screen = false;
  game_running = false;
}

/*============================================================================*/

Game::~Game()
{
  delete brick;
  brick = nullptr;
  delete[] bricks;
  bricks = nullptr;
  delete player;
  player = nullptr;
  delete ball;
  ball = nullptr;
}

/*============================================================================*/

bool Game::init()
{
  //Start Screen
  setStartScreen();

  // Pause Menu
  setPauseMenu();

  // End Screen
  setEndScreen();

  // Game Screen
  setGameScreen();

  return true;
}

/*============================================================================*/

void Game::update(float dt)
{
  if (game_running)
  {
    player->inBound(window);
    player->paddleMove(dt);
    ball->inBound(window);
    ball->move(window, player, dt);
    ball->wallBounce(window);
    ball->brickBounce(bricks);
    ball->paddleBounce(player);
    ball->loseLife(window, player);
    endGameManager();
  }
}

/*============================================================================*/

void Game::render()
{
  // Start Menu
  if (start_menu)
  {
    window.draw(start_menu_bg_sprite);
    window.draw(start_menu_title_text);
    window.draw(rules_text);
    window.draw(start_menu_play_option);
    window.draw(start_menu_quit_option);
  }

  // Pause Menu
  else if (pause_menu)
  {
    window.draw(background_pause_sprite);
    window.draw(pause_title_text);
    window.draw(restart_opt_pause_text);
    window.draw(quit_opt_pause_text);
  }

  // Win Screen
  else if (end_screen)
  {
    window.draw(end_bg_sprite);
    window.draw(end_title_text);
    window.draw(play_again_end_text);
    window.draw(quit_end_text);
  }

  // Game
  else if (game_running)
  {
    window.draw(game_bg_sprite);
    window.draw(*player->getSprite());
    window.draw(*ball->getSprite());

    for (int i = 0; i < bricks->returnGridSizeX() * bricks->returnGridSizeY(); i++)
    {
      if (bricks[i].getVisibility())
      {
        window.draw(*bricks[i].getSprite());
      }
    }
  }

  // Error
  else
  {
    std::cout<< "Rendering Error\n";
  }
}

/*============================================================================*/

void Game::keyPressed(sf::Event event)
{
  // Start Menu
  if (start_menu)
  {
    if ((event.key.code == sf::Keyboard::Left) ||
        (event.key.code == sf::Keyboard::Right))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        start_menu_play_option.setString(">Play<");
        start_menu_play_option.setStyle(sf::Text::Italic | sf::Text::Bold);
        start_menu_quit_option.setString("Quit");
        start_menu_quit_option.setStyle(sf::Text::Regular);
      }
      else
      {
        start_menu_play_option.setString("Play");
        start_menu_play_option.setStyle(sf::Text::Regular);
        start_menu_quit_option.setString(">Quit<");
        start_menu_quit_option.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        start_menu = false;
        game_running = true;
      }
      else
      {
        window.close();
      }
    }
  }

  // Pause Menu
  if (pause_menu)
  {
    if ((event.key.code == sf::Keyboard::Left) ||
        (event.key.code == sf::Keyboard::Right))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        restart_opt_pause_text.setString(">Restart<");
        restart_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
        quit_opt_pause_text.setString("Quit");
        quit_opt_pause_text.setStyle(sf::Text::Regular);
      }
      else
      {
        restart_opt_pause_text.setString("Restart");
        restart_opt_pause_text.setStyle(sf::Text::Regular);
        quit_opt_pause_text.setString(">Quit<");
        quit_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        pause_menu = false;
        game_running = true;
        ball->setBallStarted(false);
        player->resetPos(window);
        ball->resetPos(window, player);
        player->setLives(player->returnStartLives());
        ball->setDirection(0,0);
        for (int i = 0; i < bricks->returnGridSizeX() * bricks->returnGridSizeY(); ++i)
        {
          bricks[i].setVisibility(true);
        }
      }
      else
      {
        window.close();
      }
    }
  }

  // End Screen
  if (end_screen)
  {
    if ((event.key.code == sf::Keyboard::Left) ||
        (event.key.code == sf::Keyboard::Right))
    {
      play_selected = !play_selected;
      if(play_selected)
      {
        play_again_end_text.setString(">Play Again<");
        play_again_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
        quit_end_text.setString("Quit");
        quit_end_text.setStyle(sf::Text::Regular);
      }
      else
      {
        play_again_end_text.setString("Play Again");
        play_again_end_text.setStyle(sf::Text::Regular);
        quit_end_text.setString(">Quit<");
        quit_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        end_screen = false;
        game_running = true;
        ball->setBallStarted(false);
        player->resetPos(window);
        ball->resetPos(window, player);
        player->setLives(player->returnStartLives());
        ball->setDirection(0,0);
        for (int i = 0; i < bricks->returnGridSizeX() * bricks->returnGridSizeY(); ++i)
        {
          bricks[i].setVisibility(true);
        }
      }
      else
      {
        window.close();
      }
    }
  }

  // In Game
  if (game_running)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      ball->setBallStarted(true);
      ball->setDirection(ball->returnCornerBounce(), -1 * ball->returnCornerBounce());
    }

    if (event.key.code == sf::Keyboard::A)
    {
      if (!ball->returnBallStarted())
      {
        player->setPaddleDirection(-1);
        ball->setDirection(-1,0);
      }
      else
      {
        player->setPaddleDirection(-1);
      }
    }

    if (event.key.code == sf::Keyboard::D)
    {
      if (!ball->returnBallStarted())
      {
        player->setPaddleDirection(1);
        ball->setDirection(1,0);
      }
      else
      {
        player->setPaddleDirection(1);
      }
    }

    //TEST END SCREEN
    if (event.key.code == sf::Keyboard::N)
    {
      game_running = false;
      end_screen = true;
    }
  }

  // Open/Close Pause Menu
  if (event.key.code == sf::Keyboard:: Escape)
  {
    if (game_running)
    {
      game_running = false;
      pause_menu = true;
    }
    else if (pause_menu)
    {
      pause_menu = false;
      game_running = true;
    }
  }
}
void Game::keyReleased(sf::Event event) const
{
  // In Game
  if (game_running)
  {
    if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
    {
      if (!ball->returnBallStarted())
      {
        player->setPaddleDirection(0);
        ball->setDirection(0, 0);
      }
      else
      {
        player->setPaddleDirection(0);
      }
    }
  }
}
void Game::mouseClicked(sf::Event event)
{
  // Gets Click Position
  sf::Vector2i click = sf::Mouse::getPosition(window);

  // Start Menu
  if (start_menu)
  {
    if (collisionCheckText(click, start_menu_play_option))
    {
      start_menu = false;
      game_running = true;
    }
    if (collisionCheckText(click, start_menu_quit_option))
    {
      window.close();
    }
  }

  // Pause Menu
  if (pause_menu)
  {
    if (collisionCheckText(click, restart_opt_pause_text))
    {
      pause_menu = false;
      game_running = true;
      ball->setBallStarted(false);
      player->resetPos(window);
      ball->resetPos(window, player);
      player->setLives(player->returnStartLives());
      ball->setDirection(0,0);
      for (int i = 0; i < bricks->returnGridSizeX() * bricks->returnGridSizeY(); ++i)
      {
        bricks[i].setVisibility(true);
      }
    }
    if (collisionCheckText(click, quit_opt_pause_text))
    {
      window.close();
    }
  }

  // End Screen
  if (end_screen)
  {
    if (collisionCheckText(click, play_again_end_text))
    {
      end_screen = false;
      game_running = true;
      ball->setBallStarted(false);
      player->resetPos(window);
      ball->resetPos(window, player);
      player->setLives(player->returnStartLives());
      ball->setDirection(0,0);
      for (int i = 0; i < bricks->returnGridSizeX() * bricks->returnGridSizeY(); ++i)
      {
        bricks[i].setVisibility(true);
      }
    }
    if (collisionCheckText(click, quit_end_text))
    {
      window.close();
    }
  }
}
bool Game::collisionCheckText(sf::Vector2i click, sf::Text text)
{
  if (click.x > text.getPosition().x
      && click.y > text.getPosition().y
      && click.x < text.getPosition().x + text.getGlobalBounds().width
      && click.y < text.getPosition().y + text.getGlobalBounds().height)
  {
    return true;
  }

  else
  {
    return false;
  }
}

/*============================================================================*/
// Start Screen
void Game::setStartScreen()
{
  // Background
  startScreenBG();

  // Game Title
  startScreenGameTitle();

  // Rules
  startScreenRules();

  // Play Option
  startScreenPlay();

  // Quit Option
  startScreenQuit();
}
void Game::startScreenBG()
  {
    if (!start_menu_bg_texture.loadFromFile("Data/Images/start_bg.png"))
    {
      std::cout << "Start menu background texture not loaded\n";
    }
    start_menu_bg_sprite.setTexture(start_menu_bg_texture);
    start_menu_bg_sprite.setScale
      (
        window.getSize().x / start_menu_bg_sprite.getGlobalBounds().width,
        window.getSize().y / start_menu_bg_sprite.getGlobalBounds().height
      );
    start_menu_bg_sprite.setPosition(0, 0);
  }
void Game::startScreenGameTitle()
  {
    if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
    {
      std::cout<<"Font not loaded\n";
    }
    start_menu_title_text.setString("PONG");
    start_menu_title_text.setFont(font_game);
    start_menu_title_text.setStyle(sf::Text::Italic | sf::Text::Bold);
    start_menu_title_text.setCharacterSize(50);
    start_menu_title_text.setFillColor(sf::Color(0, 0, 0, 255));
    start_menu_title_text.setPosition
      (
        window.getSize().x/2 - start_menu_title_text.getGlobalBounds().width/2,
        0
      );
  }
void Game::startScreenRules()
  {
    if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
    {
      std::cout<<"Font not loaded\n";
    }
    rules_text.setString("\n\n\n\n                     Controls\n\n"
                         "A = Move paddle left\n"
                         "D = Move paddle right\n"
                         "Space = Throw the ball\n\n");

    rules_text.setFont(font_game);
    rules_text.setCharacterSize(30);
    rules_text.setFillColor(sf::Color(0, 0, 0, 255));
    rules_text.setPosition
      (
        window.getSize().x/4 + window.getSize().x/30,
        window.getSize().y/5 - rules_text.getGlobalBounds().height/2
      );
  }
void Game::startScreenPlay()
  {
    if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
    {
      std::cout<<"Font not loaded\n";
    }
    start_menu_play_option.setString(">Play<");
    start_menu_play_option.setStyle(sf::Text::Italic | sf::Text::Bold);
    start_menu_play_option.setFont(font_game);
    start_menu_play_option.setCharacterSize(30);
    start_menu_play_option.setFillColor(sf::Color(0, 0, 0, 255));
    start_menu_play_option.setPosition
      (
        window.getSize().x/3 - start_menu_play_option.getGlobalBounds().width/2 + window.getSize().x/20,
        window.getSize().y - window.getSize().y/3 - start_menu_play_option.getGlobalBounds().height/2
      );
  }
void Game::startScreenQuit()
  {
    if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
    {
      std::cout<<"Font not loaded\n";
    }
    start_menu_quit_option.setString("Quit");
    start_menu_quit_option.setFont(font_game);
    start_menu_quit_option.setCharacterSize(30);
    start_menu_quit_option.setFillColor(sf::Color(0, 0, 0, 255));
    start_menu_quit_option.setPosition
      (
        window.getSize().x - window.getSize().x/3 - start_menu_quit_option.getGlobalBounds().width/2 - window.getSize().x/20,
        window.getSize().y - window.getSize().y/3 - start_menu_quit_option.getGlobalBounds().height/2
      );
  }

/*============================================================================*/
// Game Screen
void Game::setGameScreen()
{
  // BG GAME
  gameScreenBG();

  // Player
  gameScreenPlayer();

  // Ball
  gameScreenBall();

  // Level
  levelOne();
}
void Game::gameScreenBG()
{
  if (!game_bg_texture.loadFromFile("Data/Images/game_bg.jpg"))
  {
    std::cout << "Game background texture not loaded\n";
  }
  game_bg_sprite.setTexture(game_bg_texture);
  game_bg_sprite.setScale
    (
      window.getSize().x / game_bg_sprite.getGlobalBounds().width,
      window.getSize().y / game_bg_sprite.getGlobalBounds().height
    );
  game_bg_sprite.setPosition(0, 0);
}
void Game::gameScreenPlayer()
{
  player = new Player();
  player->getSprite()->setPosition
    (
      window.getSize().x/2 - player->getSprite()->getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/50 - player->getSprite()->getGlobalBounds().height
    );
}
void Game::gameScreenBall()
{
  ball = new Ball();
  ball->getSprite()->setPosition
    (
      window.getSize().x/2 - ball->getSprite()->getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/50 - player->getSprite()->getGlobalBounds().height - ball->getSprite()->getGlobalBounds().height
    );
}
void Game::levelOne()
{
  // Bricks
  brick = new Brick;
  bricks = new Brick[brick->returnGridSizeX() * brick->returnGridSizeY()];

  for (int i = 0; i < bricks->returnGridSizeX(); i++)
  {
    for (int j = 0; j < bricks->returnGridSizeY(); j++)
    {
      if ((i * bricks->returnGridSizeY() + j) % 3 == 0)
      {
        bricks[i * bricks->returnGridSizeY() + j].addTexture(Brick::Colours::Blue);
      }
      else if ((i * bricks->returnGridSizeY() + j) % 3 == 1)
      {
        bricks[i * bricks->returnGridSizeY() + j].addTexture(Brick::Colours::Green);
      }
      else
      {
        bricks[i * bricks->returnGridSizeY() + j].addTexture(Brick::Colours::Yellow);
      }

      bricks[i * bricks->returnGridSizeY() + j].getSprite()->setScale(
        bricks->spriteWidth()/bricks[i * bricks->returnGridSizeY() + j].getSprite()->getGlobalBounds().width,
        bricks->spriteHeight()/bricks[i * bricks->returnGridSizeY() + j].getSprite()->getGlobalBounds().height
      );
      bricks[i * bricks->returnGridSizeY() +j].getSprite()->setPosition(
        window.getSize().x/2 - (bricks->spriteWidth() * bricks->returnGridSizeX())/2 + bricks->spriteWidth()*i,
        bricks->spriteHeight() * j
      );
    }
  }
}

/*============================================================================*/
// End Screen
void Game::setEndScreen()
{
  // BG
  endScreenBG();

  // Text
  endScreenText();

  // Play Again Option
  endScreenPlay();

  // Quit option
  endScreenQuit();
}
void Game::endScreenBG()
{
  if (!end_bg_texture.loadFromFile("Data/Images/end_bg.png"))
  {
    std::cout << "End screen background texture not loaded\n";
  }
  end_bg_sprite.setTexture(end_bg_texture);
  end_bg_sprite.setScale
    (
      window.getSize().x / end_bg_sprite.getGlobalBounds().width,
      window.getSize().y / end_bg_sprite.getGlobalBounds().height
    );
  end_bg_sprite.setPosition(0, 0);
}
void Game::endScreenText()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  end_title_text.setString("GAME OVER");
  end_title_text.setFont(font_game);
  end_title_text.setCharacterSize(80);
  end_title_text.setFillColor(sf::Color(0, 0, 0, 255));
  end_title_text.setPosition
    (
      window.getSize().x/2 - end_title_text.getGlobalBounds().width/2,
      window.getSize().y/3 - end_title_text.getGlobalBounds().height/2 - window.getSize().y/10
    );
}
void Game::endScreenPlay()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  play_again_end_text.setString(">Play again<");
  play_again_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  play_again_end_text.setFont(font_game);
  play_again_end_text.setCharacterSize(30);
  play_again_end_text.setFillColor(sf::Color(0, 0, 0, 255));
  play_again_end_text.setPosition
    (
      window.getSize().x/3 - play_again_end_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - play_again_end_text.getGlobalBounds().height/2
    );
}
void Game::endScreenQuit()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  quit_end_text.setString("Quit");
  quit_end_text.setFont(font_game);
  quit_end_text.setCharacterSize(30);
  quit_end_text.setFillColor(sf::Color(0, 0, 0, 255));
  quit_end_text.setPosition
    (
      window.getSize().x - window.getSize().x/3 - quit_end_text.getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/3 - quit_end_text.getGlobalBounds().height/2
    );
}

/*============================================================================*/
// Pause Screen
void Game::setPauseMenu()
{
  // Background
  pauseMenuBG();

  // Pause Text
  pauseMenuText();

  // Restart Option
  pauseMenuRestart();

  // Quit option
  pauseMenuQuit();
}
void Game::pauseMenuBG()
{
  if (!background_pause_texture.loadFromFile("Data/Images/pause_bg.png"))
  {
    std::cout << "Pause background texture not loaded\n";
  }
  background_pause_sprite.setTexture(background_pause_texture);
  background_pause_sprite.setScale
    (
      window.getSize().x / background_pause_sprite.getGlobalBounds().width,
      window.getSize().y / background_pause_sprite.getGlobalBounds().height
    );
  background_pause_sprite.setPosition(0, 0);
}
void Game::pauseMenuText()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  pause_title_text.setString("PAUSE");
  pause_title_text.setFont(font_game);
  pause_title_text.setCharacterSize(80);
  pause_title_text.setFillColor(sf::Color(0, 0, 0, 255));
  pause_title_text.setPosition
    (
      window.getSize().x/2 - pause_title_text.getGlobalBounds().width/2,
      window.getSize().y/3 - pause_title_text.getGlobalBounds().height/2 - window.getSize().y/10
    );
}
void Game::pauseMenuRestart()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  restart_opt_pause_text.setString(">Restart<");
  restart_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  restart_opt_pause_text.setFont(font_game);
  restart_opt_pause_text.setCharacterSize(30);
  restart_opt_pause_text.setFillColor(sf::Color(0, 0, 0, 255));
  restart_opt_pause_text.setPosition
    (
      window.getSize().x/3 - restart_opt_pause_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - restart_opt_pause_text.getGlobalBounds().height/2
    );
}
void Game::pauseMenuQuit()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  quit_opt_pause_text.setString("Quit");
  quit_opt_pause_text.setFont(font_game);
  quit_opt_pause_text.setCharacterSize(30);
  quit_opt_pause_text.setFillColor(sf::Color(0, 0, 0, 255));
  quit_opt_pause_text.setPosition
    (
      window.getSize().x - window.getSize().x/3 - quit_opt_pause_text.getGlobalBounds().width,
      window.getSize().y - window.getSize().y/3 - quit_opt_pause_text.getGlobalBounds().height/2
    );
}

/*============================================================================*/

void Game::endGameManager()
{
  if (player->returnLives() == 0 ||
      bricks->returnBrickCount() == bricks->returnGridSizeX() * bricks->returnGridSizeY())
  {
    game_running = false;
    end_screen = true;
  }
}

