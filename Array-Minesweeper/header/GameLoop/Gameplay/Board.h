#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "../../header/GameLoop/Gameplay/Cell.h"

namespace Gameplay
{
    class Board
    {
    private:
        // Board Constants
        static const int numberOfRows = 9;
        static const int numberOfColumns = 9;

        const float horizontalCellPadding = 115.f;
        const float verticalCellPadding = 329.f;

        const float boardWidth = 866.f;
        const float boardHeight = 1080.f;
        const float boardPosition = 530.f;

        //Randomization
        std::default_random_engine randomEngine;
        std::random_device randomDevice;

        //Number of Mines
        static const int minesCount = 9;

        const std::string boardTexturePath = "assets/textures/board.png";
        sf::Texture boardTexture;
        sf::Sprite boardSprite;

        Cell* cell[numberOfRows][numberOfColumns];

        void initializeBoardImage();
        void initialize();
        void createBoard();

        float getCellWidthInBoard() const;
        float getCellHeightInBoard() const;

        //Populating the Board
        void populateBoard();
        void populateMines();

        void initializeVariables();

    public:
        Board();

        void render(sf::RenderWindow& window);
    };
}