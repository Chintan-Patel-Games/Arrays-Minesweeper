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

    void GameplayManager::initializeVariables()
    {
        board = new Board(this);
        remaining_time = max_level_duration;  // Start with full time
    }

    void GameplayManager::update(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        if (!hasGameEnded()) //Check if the game has ended
            handleGameplay(eventManager, window);
    }

    void GameplayManager::render(sf::RenderWindow& window)
    {
        //Render the background
        window.draw(background_sprite);

        //Render the board
        board->render(window);
    }

    void GameplayManager::updateRemainingTime()
    {
        remaining_time -= Time::TimeManager::getDeltaTime();  // Decrease time
        processTimeOver();  // Check if time's up
    }

    void GameplayManager::processTimeOver()
    {
        if (remaining_time <= 0)
        {
            remaining_time = 0; // Don't go negative
            game_result = GameResult::LOST; // Game over!
        }
    }

    void GameplayManager::handleGameplay(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        updateRemainingTime();
        board->update(eventManager, window);
    }

    bool GameplayManager::hasGameEnded() { return game_result != GameResult::NONE; }

    void GameplayManager::setGameResult(GameResult gameResult) { this->game_result = gameResult; }
}