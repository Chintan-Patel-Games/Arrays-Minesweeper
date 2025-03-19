#include "../../header/GameLoop/Gameplay/GameplayManager.h"
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

    void GameplayManager::initializeVariables() { board = new Board(); }

    void GameplayManager::update(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        board->update(eventManager, window);
    }

    void GameplayManager::render(sf::RenderWindow& window)
    {
        //Render the background
        window.draw(background_sprite);

        //Render the board
        board->render(window);
    }
}