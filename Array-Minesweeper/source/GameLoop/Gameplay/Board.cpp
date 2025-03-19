#include "../../header/GameLoop/Gameplay/Board.h"
#include <iostream>

namespace Gameplay
{
    Board::Board() { initialize(); }

    void Board::initialize()
    {
        initializeBoardImage();
        initializeVariables(); //initialize random engine
        createBoard();
        populateBoard();
    }

    void Board::initializeBoardImage()
    {
        if (!boardTexture.loadFromFile(boardTexturePath))
        {
            std::cerr << "Failed to load board texture!" << std::endl;
            return;
        }

        boardSprite.setTexture(boardTexture);
        boardSprite.setPosition(boardPosition, 0);
        boardSprite.setScale(boardWidth / boardTexture.getSize().x,
            boardHeight / boardTexture.getSize().y);
    }

    void Board::initializeVariables()
    {
        randomEngine.seed(randomDevice()); //Function to initialize random engine
    }

    void Board::createBoard()
    {
        float cell_width = getCellWidthInBoard();
        float cell_height = getCellHeightInBoard();

        //create cells for the cell[][] array
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                cell[row][col] = new Cell(cell_width, cell_height, sf::Vector2i(row, col));
    }

    void Board::populateBoard()
    {
        populateMines();
        populateCells();
    }

    void Board::populateMines()
    {
        std::uniform_int_distribution<int> x_dist(0, numberOfColumns - 1);
        std::uniform_int_distribution<int> y_dist(0, numberOfRows - 1);

        int mines_placed = 0;
        while (mines_placed < minesCount)
        {
            int x = x_dist(randomEngine);
            int y = y_dist(randomEngine);

            if (cell[x][y]->getCellType() != CellType::MINE) {
                //Step 4
                cell[x][y]->setCellType(CellType::MINE);
                ++mines_placed;
            }
        }
    }

    void Board::populateCells()
    {
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                if (cell[row][col]->getCellType() != CellType::MINE)
                {
                    int mines_around = countMinesAround(sf::Vector2i(row, col));
                    cell[row][col]->setCellType(static_cast<CellType>(mines_around));
                }
    }

    int Board::countMinesAround(sf::Vector2i cell_position)
    {
        int mines_around = 0;

        for (int a = -1; a <= 1; ++a) {
            for (int b = -1; b <= 1; ++b) {
                // Validate cell's postion and check current cell
                if ((a == 0 && b == 0) ||
                    !isValidCellPosition(sf::Vector2i(cell_position.x + a, cell_position.y + b)))
                    continue;

                //Check Mines
                if (cell[cell_position.x + a][cell_position.y + b]->getCellType() ==
                    CellType::MINE)
                    mines_around++;
            }
        }
        return mines_around;
    }

    bool Board::isValidCellPosition(sf::Vector2i cell_position)
    {
        return (cell_position.x >= 0 && cell_position.y >= 0 &&
            cell_position.x < numberOfColumns && cell_position.y < numberOfRows);
    }

    float Board::getCellWidthInBoard() const { return (boardWidth - horizontalCellPadding) / numberOfColumns; }

    float Board::getCellHeightInBoard() const { return (boardHeight - verticalCellPadding) / numberOfRows; }

    void Board::render(sf::RenderWindow& window)
    {
        window.draw(boardSprite);

        //render array's elements one by one
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                cell[row][col]->render(window);
    }
}