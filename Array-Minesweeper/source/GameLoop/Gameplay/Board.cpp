#include "../../header/GameLoop/Gameplay/Board.h"
#include <iostream>

namespace Gameplay
{
    Board::Board() { initialize(); }

    void Board::initialize()
    {
        initializeBoardImage();
        createBoard();
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

    void Board::createBoard()
    {
        float cell_width = getCellWidthInBoard();
        float cell_height = getCellHeightInBoard();

        //Create a cell for each array index
        for (int col = 0; col < numberOfColumns; ++col) { cell[col] = new Cell(cell_width, cell_height, sf::Vector2i(col, 0)); }
    }

    float Board::getCellWidthInBoard() const { return (boardWidth - horizontalCellPadding) / numberOfColumns; }

    float Board::getCellHeightInBoard() const { return (boardHeight - verticalCellPadding) / numberOfRows; }

    void Board::render(sf::RenderWindow& window)
    {
        window.draw(boardSprite);

        //render array's elements one by one
        for (int col = 0; col < numberOfColumns; ++col) { cell[col]->render(window);
        }
    }
}