#pragma once
#include "Board.h"
#include "Player.h"

enum GameMode { TWO_PLAYERS, PLAYER_VS_COMPUTER };

class Game {
public:
    Game();
    void start(GameMode mode);

    bool playHumanMove(int row, int col);
    bool playComputerMove(int &row, int &col);

    const Board& getBoard() const;
    Player& getPlayerX();
    Player& getPlayerO();
    Player& getCurrentPlayer();

    char getTurn() const;
    char getWinner() const;
    bool isOver() const;
    bool isComputerTurn() const;

    
    void removeMark(int row, int col);

private:
    Board board;
    Player playerX, playerO;
    char turn, winner;
    bool over;

    std::deque<int> availableMoves;
    

    void finishTurn();
    bool findWinningMove(char mark, int &row, int &col) const;
    bool wouldWin(char mark, int row, int col) const;
};
