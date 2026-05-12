#pragma once

#include "Board.h"
#include "Player.h"

enum GameMode {
    TWO_PLAYERS,
    PLAYER_VS_COMPUTER
};

class Game {
    private:
        Board board;
        Player playerX;
        Player playerO;
        char turn;
        char winner;
        bool over;

        void finishTurn();
        bool findWinningMove(char mark, int& row, int& col) const;
        bool wouldWin(char mark, int row, int col) const;

    public:
        Game();

        void start(GameMode mode);
        bool playHumanMove(int row, int col);
        bool playComputerMove(int& row, int& col);

        const Board& getBoard() const;
        char getTurn() const;
        char getWinner() const;
        bool isOver() const;
        bool isComputerTurn() const;
};
