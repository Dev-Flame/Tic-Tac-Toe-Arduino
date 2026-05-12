#include "Board.h"

Board::Board() {
    clear();
}

void Board::clear() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            spaces[row][col] = ' ';
        }
    }
}

bool Board::placeMark(int row, int col, char mark) {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        return false;
    }

    if (!isEmpty(row, col)) {
        return false;
    }

    spaces[row][col] = mark;
    return true;
}

char Board::getMark(int row, int col) const {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        return ' ';
    }

    return spaces[row][col];
}

bool Board::isEmpty(int row, int col) const {
    return getMark(row, col) == ' ';
}

bool Board::isFull() const {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (spaces[row][col] == ' ') {
                return false;
            }
        }
    }

    return true;
}

char Board::getWinner() const {
    for (int i = 0; i < 3; i++) {
        if (spaces[i][0] != ' ' && spaces[i][0] == spaces[i][1] && spaces[i][1] == spaces[i][2]) {
            return spaces[i][0];
        }

        if (spaces[0][i] != ' ' && spaces[0][i] == spaces[1][i] && spaces[1][i] == spaces[2][i]) {
            return spaces[0][i];
        }
    }

    if (spaces[0][0] != ' ' && spaces[0][0] == spaces[1][1] && spaces[1][1] == spaces[2][2]) {
        return spaces[0][0];
    }

    if (spaces[0][2] != ' ' && spaces[0][2] == spaces[1][1] && spaces[1][1] == spaces[2][0]) {
        return spaces[0][2];
    }

    return ' ';
}
