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