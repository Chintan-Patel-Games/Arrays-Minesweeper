#pragma once
#include <SFML/Graphics.hpp>
#include "../../header/GameLoop/Gameplay/Board.h"

namespace Gameplay
{
    class GameplayManager
    {
    private:
        const float background_alpha = 85.f;

        sf::Texture background_texture;
        sf::Sprite background_sprite;
        std::string background_texture_path = "assets/textures/minesweeper_bg.png";

        Board* board;

        void initialize();
        void initializeVariables();
        void initializeBackgroundImage();

    public:
        GameplayManager();
        ~GameplayManager() = default;

		void update(Event::EventPollingManager& eventManager, sf::RenderWindow& window);
        void render(sf::RenderWindow& window);
    };
}