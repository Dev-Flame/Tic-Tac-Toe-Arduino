#include "Board.h"

Board::Board() { clear(); }

void Board::clear() {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            spaces[r][c] = ' ';
}

bool Board::placeMark(int row, int col, char mark) {
    if (!isEmpty(row, col)) return false;
    spaces[row][col] = mark;
    return true;
}

void Board::removeMark(int row, int col) {
    spaces[row][col] = ' ';
}

char Board::getMark(int row, int col) const { return spaces[row][col]; }
bool Board::isEmpty(int row, int col) const { return spaces[row][col] == ' '; }

bool Board::isFull() const {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (spaces[r][c] == ' ') return false;
    return true;
}

char Board::getWinner() const {
    for (int i = 0; i < 3; i++) {
        if (spaces[i][0] != ' ' && spaces[i][0] == spaces[i][1] && spaces[i][1] == spaces[i][2]) return spaces[i][0];
        if (spaces[0][i] != ' ' && spaces[0][i] == spaces[1][i] && spaces[1][i] == spaces[2][i]) return spaces[0][i];
    }
    if (spaces[0][0] != ' ' && spaces[0][0] == spaces[1][1] && spaces[1][1] == spaces[2][2]) return spaces[0][0];
    if (spaces[0][2] != ' ' && spaces[0][2] == spaces[1][1] && spaces[1][1] == spaces[2][0]) return spaces[0][2];
    return ' ';
}



