#pragma once
#include <SFML/Graphics.hpp>
#include "../../header/GameLoop/Gameplay/Board.h"
#include "../../../header/Time/TimeManager.h"
#include "../../header/UI/GameplayUI/GameplayUI.h"

namespace Gameplay
{
    enum class GameResult
    {
        NONE,
        WON,
        LOST
    };

    class GameplayManager
    {
    private:
        const float background_alpha = 85.f;

        sf::Texture background_texture;
        sf::Sprite background_sprite;
        std::string background_texture_path = "assets/textures/minesweeper_bg.png";

        const float max_level_duration = 150.0f;
        const float game_over_time = 11.0f;
        float remaining_time;

        Board* board;
        GameResult game_result;
        UI::GameplayUI* gameplay_ui;

        void initialize();
        void initializeVariables();
        void initializeBackgroundImage();
        bool hasGameEnded();

        void updateRemainingTime();
        void processTimeOver();
        int getRemainingMinesCount() const;

        void handleGameplay(Event::EventPollingManager& eventManager, sf::RenderWindow& window);
        void gameWon();
        void gameLost();

    public:
        GameplayManager();
        ~GameplayManager() = default;

		void update(Event::EventPollingManager& eventManager, sf::RenderWindow& window);
        void render(sf::RenderWindow& window);

        void checkGameWin();
        void setGameResult(GameResult gameResult);
        void processGameResult();
    };
}