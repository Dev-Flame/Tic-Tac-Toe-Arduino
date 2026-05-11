#include "Tic-Tac-Toe.hpp"

// Default constructor
// Player vs player
// X goes first
TicTacToe::TicTacToe() : board(new Board()), turnMark(Mark(Symbol::X)), gameDone(false) {};

TicTacToe::~TicTacToe() {
    delete board;
}

bool TicTacToe::isDone() const {
    return gameDone;
}

// Since we are using touchscreen, no input validation needed
void TicTacToe::makeMove(const int row, const int col) {
    // Make sure that the slot is empty
    if (board->getMark(row, col).getSymbol() != Symbol::EMPTY) return;

    board->setMark(row, col, turnMark);

    // Check if person who just made a move won
    if (board->checkWin(turnMark)) {
        gameDone = true;
        winner = turnMark;
    } else if (board->getNumMarks() == 9) {
        gameDone = true;
    }

    // Change turns
    if (turnMark.getSymbol() == Symbol::X) {
        turnMark.setSymbol(Symbol::O);
    } else {
        turnMark.setSymbol(Symbol::X);
    }
}

Board* TicTacToe::getBoard() const {
    return board;
}

// Resets the game
void TicTacToe::reset() {
    turnMark.setSymbol(Symbol::X);
    gameDone = false;
    board->clear();
}