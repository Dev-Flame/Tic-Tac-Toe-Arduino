#include "Tic-Tac-Toe.hpp"

// Default constructor
// Player vs player
// X goes first
TicTacToe::TicTacToe() : board(new Board()), turnMark(Mark(Symbol::X)), gameDone(false) {};

TicTacToe::~TicTacToe() {
    delete board;
}

// Called after every move
bool TicTacToe::checkWin() {
    bool win;

    // Check rows
    for (int i = 0; i < 3; i++) {
        win = true;

        for (int j = 0; j < 3; j++) {
            if (board->getMark(i, j) != turnMark) win = false;
        }

        if (win) return true;
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        win = true;

        for (int j = 0; j < 3; j++) {
            if (board->getMark(j, i) != turnMark) win = false;
        }

        if (win) return true;
    }

    // check left to right diagonal
    win = true;

    for (int i = 0; i < 3; i++) {
        if (board->getMark(i, i) != turnMark) win = false;
    }

    if (win) return true;

    // check right to left diagonal
    win = true;

    for (int i = 0; i < 3; i++) {
        if (board->getMark(i, 2 - i) != turnMark) win = false;
    }

    if (win) return true;

    // At this point, no one has won yet
    return false;
}

bool TicTacToe::isDone() const {
    return gameDone;
}

// Since we are using touchscreen, no input validation needed
void TicTacToe::makeMove(const int row, const int col) {
    // Make sure that the slot is empty
    if (board->getMark(row, col).getSymbol() != Symbol::EMPTY) return;

    board->setMark(row, col, turnMark);

    // Check if someone has won or if a draw took place
    if (checkWin()) {
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