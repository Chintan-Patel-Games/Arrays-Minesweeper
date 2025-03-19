#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "../../header/GameLoop/Gameplay/Cell.h"

namespace Gameplay
{
    enum class BoardState
    {
        FIRST_CELL,
        PLAYING,
        COMPLETED,
    };

    class GameplayManager; //Forawrd Declaration
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

        int flaggedCells;

        const std::string boardTexturePath = "assets/textures/board.png";
        sf::Texture boardTexture;
        sf::Sprite boardSprite;

        BoardState boardState;
        Cell* cell[numberOfRows][numberOfColumns];
        GameplayManager* gameplay_manager;

        void initializeBoardImage();
        void initialize(GameplayManager* gameplay_manager);
        void initializeVariables(GameplayManager* gameplay_manager);
        void createBoard();

        float getCellWidthInBoard() const;
        float getCellHeightInBoard() const;

        //Populating the Board
        void populateBoard(sf::Vector2i cell_position);
        void populateMines(sf::Vector2i first_cell_position);
        bool isInvalidMinePosition(sf::Vector2i first_cell_position, int x, int y);
        int countMinesAround(sf::Vector2i cell_position);
        void populateCells();
        bool isValidCellPosition(sf::Vector2i cell_position);

        void toggleFlag(sf::Vector2i cell_position);
        void processCellType(sf::Vector2i cell_position);
        void processEmptyCell(sf::Vector2i cell_position);
        void processMineCell(sf::Vector2i cell_position);

    public:
        Board(GameplayManager* gameplayManager);

        void update(Event::EventPollingManager& eventManager, sf::RenderWindow& window);
        void render(sf::RenderWindow& window);

        BoardState getBoardState() const;
        void setBoardState(BoardState state);
        void onCellButtonClicked(sf::Vector2i cell_position, MouseButtonType mouse_button_type);
        void openCell(sf::Vector2i cell_position);
        void revealAllMines();
    };
}