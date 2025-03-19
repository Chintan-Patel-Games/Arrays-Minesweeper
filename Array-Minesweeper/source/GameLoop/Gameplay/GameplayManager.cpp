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
        gameplay_ui = new UI::GameplayUI(this);
        remaining_time = max_level_duration;  // Start with full time
    }

    void GameplayManager::update(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        if (!hasGameEnded()) //Check if the game has ended
            handleGameplay(eventManager, window);
        else if (board->getBoardState() != BoardState::COMPLETED)
            processGameResult();  // Handle win/loss
		    
		//update the UI
        gameplay_ui->update(getRemainingMinesCount(),
            static_cast<int>(remaining_time),
            eventManager, window);
    }

    void GameplayManager::render(sf::RenderWindow& window)
    {
        window.draw(background_sprite);  //Render the background
        board->render(window);  //Render the board
        gameplay_ui->render(window);  // render UI
    }

    int GameplayManager::getRemainingMinesCount() const { return board->getRemainingMinesCount(); }

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
        checkGameWin();  // See if player has won
    }

    bool GameplayManager::hasGameEnded() { return game_result != GameResult::NONE; }

    void GameplayManager::checkGameWin()
    {
        if (board->areAllCellsOpen())
            game_result = GameResult::WON;  // Victory!
    }

    void GameplayManager::setGameResult(GameResult gameResult) { this->game_result = gameResult; }

    void GameplayManager::processGameResult()
    {
        switch (game_result)
        {
        case GameResult::WON:
            gameWon();    // Victory! 🎉
            break;
        case GameResult::LOST:
            gameLost();   // Game Over! 💥
            break;
        default:
            break;
        }
    }

    void GameplayManager::gameWon()
    {
        Sound::SoundManager::PlaySound(Sound::SoundType::GAME_WON);  // Play victory sound
        board->flagAllMines();  // Show all mines
        board->setBoardState(BoardState::COMPLETED);  // Stop the game
    }

    void GameplayManager::gameLost()
    {
        Sound::SoundManager::PlaySound(Sound::SoundType::EXPLOSION);  // Boom!
        board->setBoardState(BoardState::COMPLETED);  // Game over
        board->revealAllMines();  // Show where the mines were
    }
}