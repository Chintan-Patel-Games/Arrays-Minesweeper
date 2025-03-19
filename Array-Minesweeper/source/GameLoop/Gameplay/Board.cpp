#include "../../header/GameLoop/Gameplay/Board.h"
#include "../../header/GameLoop/Gameplay/GameplayManager.h"
#include <iostream>

namespace Gameplay
{
    Board::Board(GameplayManager* gameplayManager) { initialize(gameplayManager); }

    void Board::initialize(GameplayManager* gameplayManager)
    {
        initializeBoardImage();
        initializeVariables(gameplayManager); //initialize random engine
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

    void Board::initializeVariables(GameplayManager* gameplayManager)
    {
        this->gameplay_manager = gameplayManager;
        randomEngine.seed(randomDevice()); //Function to initialize random engine
        boardState = BoardState::FIRST_CELL;  // Start with first cell state
    }

    void Board::createBoard()
    {
        float cell_width = getCellWidthInBoard();
        float cell_height = getCellHeightInBoard();

        //create cells for the cell[][] array
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                cell[row][col] = new Cell(cell_width, cell_height, sf::Vector2i(row, col), this);
    }

    void Board::populateBoard(sf::Vector2i cell_position)
    {
        populateMines(cell_position);
        populateCells();
    }

    void Board::populateMines(sf::Vector2i first_cell_position)
    {
        std::uniform_int_distribution<int> x_dist(0, numberOfColumns - 1);
        std::uniform_int_distribution<int> y_dist(0, numberOfRows - 1);

        int mines_placed = 0;
        while (mines_placed < minesCount)
        {
            int x = x_dist(randomEngine);
            int y = y_dist(randomEngine);

            if (isInvalidMinePosition(first_cell_position, x, y))
                continue;  // Skip first cell's position before placing a mine

            cell[x][y]->setCellType(CellType::MINE);
            ++mines_placed;
        }
    }
    
    bool Board::isInvalidMinePosition(sf::Vector2i first_cell_position, int x, int y)
    {
        return (x == first_cell_position.x && y == first_cell_position.y) ||
            cell[x][y]->getCellType() == CellType::MINE;
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

    void Board::update(Event::EventPollingManager& eventManager, sf::RenderWindow& window)
    {
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                cell[row][col]->update(eventManager, window);
    }

    void Board::render(sf::RenderWindow& window)
    {
        window.draw(boardSprite);

        //render array's elements one by one
        for (int row = 0; row < numberOfRows; ++row)
            for (int col = 0; col < numberOfColumns; ++col)
                cell[row][col]->render(window);
    }

    BoardState Board::getBoardState() const { return boardState; }

    void Board::setBoardState(BoardState state) { boardState = state; }

    void Board::onCellButtonClicked(sf::Vector2i cell_position, MouseButtonType mouse_button_type)
    {
        if (mouse_button_type == MouseButtonType::LEFT_MOUSE_BUTTON)
        {
            Sound::SoundManager::PlaySound(Sound::SoundType::BUTTON_CLICK); //play click sound
            openCell(cell_position); // Open the cell when left-clicked
        }
        else if (mouse_button_type == MouseButtonType::RIGHT_MOUSE_BUTTON)
        {
            Sound::SoundManager::PlaySound(Sound::SoundType::FLAG);//play flag sound
            toggleFlag(cell_position);
        }
    }

    void Board::openCell(sf::Vector2i cell_position)
    {
        if (!cell[cell_position.x][cell_position.y]->canOpenCell())
            return; // Can't open this cell!

        if (boardState == BoardState::FIRST_CELL)
        {
            populateBoard(cell_position);    // Place mines after first click
            boardState = BoardState::PLAYING; // Now we can play normally
        }

        processCellType(cell_position);
    }

    void Board::processCellType(sf::Vector2i cell_position)
    {
        switch (cell[cell_position.x][cell_position.y]->getCellType())
        {
        case CellType::EMPTY:
            processEmptyCell(cell_position);
            break;
        case CellType::MINE:
            processMineCell(cell_position);
            break;
        default:
            cell[cell_position.x][cell_position.y]->open();
            break;
        }
    }
    
    void Board::processEmptyCell(sf::Vector2i cell_position)
    {
        CellState cell_state = cell[cell_position.x][cell_position.y]->getCellState();

        switch (cell_state)
        {
        case::Gameplay::CellState::OPEN:
            return;  // Already open, stop here
        default:
            cell[cell_position.x][cell_position.y]->open();


            // Check all 8 neighbors
            for (int a = -1; a <= 1; ++a)
            {
                for (int b = -1; b <= 1; ++b)
                {
                    //Store neighbor cells position
                    sf::Vector2i next_cell_position = sf::Vector2i(a + cell_position.x, b + cell_position.y);

                    // Skip current cell and invalid positions
                    if ((a == 0 && b == 0) || !isValidCellPosition(next_cell_position))
                        continue;

                    //Flagged Cell Case
                    CellState next_cell_state = cell[next_cell_position.x][next_cell_position.y]->getCellState();

                    if (next_cell_state == CellState::FLAGGED)
                        toggleFlag(next_cell_position);

                    //Open neighbor cell
                    openCell(next_cell_position);  // Open neighbor
                }
            }
        }
    }

    void Board::processMineCell(sf::Vector2i cell_position)
    {
        gameplay_manager->setGameResult(GameResult::LOST);  // Game Over!
        Sound::SoundManager::PlaySound(Sound::SoundType::EXPLOSION);
        revealAllMines();                                   // Show all mines
    }

    void Board::toggleFlag(sf::Vector2i cell_position)
    {
        cell[cell_position.x][cell_position.y]->toggleFlag();
        flaggedCells += (cell[cell_position.x][cell_position.y]->getCellState() == CellState::FLAGGED) ? 1 : -1;
    }

    void Board::revealAllMines()
    {
        for (int row = 0; row < numberOfRows; row++)
        {
            for (int col = 0; col < numberOfColumns; col++)
            {
                if (cell[row][col]->getCellType() == CellType::MINE)
                    cell[row][col]->setCellState(CellState::OPEN);  // Show the mines
            }
        }
    }
}