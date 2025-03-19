#include "../../header/GameLoop/Gameplay/GameplayManager.h"
#include "../../header/GameLoop/Gameplay/Board.h"
#include <iostream>

namespace Gameplay
{
    GameplayManager::GameplayManager() { initialize(); }

    void GameplayManager::initialize()
    {
        //initialize background image
        initializeBackgroundImage();
        initializeVariables();
    }

    void GameplayManager::initializeBackgroundImage()
    {
        if (!background_texture.loadFromFile(background_texture_path))
        {
            std::cerr << "Failed to load background texture!" << std::endl;
        }
        background_sprite.setTexture(background_texture);
        background_sprite.setColor(sf::Color(255, 255, 255, background_alpha));
    }

    void GameplayManager::initializeVariables() { board = new Board(this); }

    void GameplayManager::update(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        if (!hasGameEnded()) //Check if the game has ended
            board->update(eventManager, window);
    }

    void GameplayManager::render(sf::RenderWindow& window)
    {
        //Render the background
        window.draw(background_sprite);

        //Render the board
        board->render(window);
    }

    bool GameplayManager::hasGameEnded() { return game_result != GameResult::NONE; }

    void GameplayManager::setGameResult(GameResult gameResult) { this->game_result = gameResult; }
}