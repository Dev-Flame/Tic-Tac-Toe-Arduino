#pragma once


#include "Board.hpp"

/*
Mark turnmark
    Stores the mark of whoever's turn it is

Mark winner
    Stores the mark of whoever won the game
    It stores an EMPTY mark by default which signals a draw

*/

class TicTacToe {
    private:
        Board* board;

        bool gameDone;
        Mark turnMark;
        Mark winner;

    public:
        TicTacToe();
        ~TicTacToe();

        bool checkWin();
        bool isDone() const;
        void makeMove(const int row, const int column);

        Board* getBoard() const;

        void reset();
};