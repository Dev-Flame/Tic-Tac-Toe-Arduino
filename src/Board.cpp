#include "Board.hpp"


// Default constructor, creates an empty board
Board::Board() : numMarks(0) {
    clear();
}

// Copy constructor, copies a board
Board::Board(const Board& boardToCopy) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            setMark(i, j, boardToCopy.getMark(i, j));
        }
    }
}

// Sets all Marks to EMPTY
void Board::clear() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            setMark(i, j, Mark(Symbol::EMPTY));
        }
    }

    numMarks = 0;
}

Mark Board::getMark(const int row, const int col) const {
    return state[row][col];
}

void Board::setMark(const int row, const int col, const Mark &mark) {
    state[row][col] = mark;
    numMarks++;
}

int Board::getNumMarks() const {
    return numMarks;
}

// Checks to see if a certain player has won (markCheck)
// Does NOT check to see if someone won in general
bool Board::checkWin(Mark& markCheck) const {
    bool win;

    // Check rows
    for (int i = 0; i < 3; i++) {
        win = true;

        for (int j = 0; j < 3; j++) {
            if (getMark(i, j) != markCheck) win = false;
        }

        if (win) return true;
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        win = true;

        for (int j = 0; j < 3; j++) {
            if (getMark(j, i) != markCheck) win = false;
        }

        if (win) return true;
    }

    // check left to right diagonal
    win = true;

    for (int i = 0; i < 3; i++) {
        if (getMark(i, i) != markCheck) win = false;
    }

    if (win) return true;

    // check right to left diagonal
    win = true;

    for (int i = 0; i < 3; i++) {
        if (getMark(i, 2 - i) != markCheck) win = false;
    }

    if (win) return true;

    // At this point, no one has won yet
    return false;
}