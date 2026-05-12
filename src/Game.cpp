#include "Game.h"

Game::Game() : playerX('X', false), playerO('O', false) {
    start(TWO_PLAYERS);
}

void Game::start(GameMode mode) {
    board.clear();
    playerX = Player('X', false);
    playerO = Player('O', mode == PLAYER_VS_COMPUTER);
    turn = 'X';
    winner = ' ';
    over = false;
}

bool Game::playHumanMove(int row, int col) {
    if (over || isComputerTurn()) {
        return false;
    }

    if (!board.placeMark(row, col, turn)) {
        return false;
    }

    finishTurn();
    return true;
}

bool Game::playComputerMove(int& row, int& col) {
    if (!isComputerTurn()) {
        return false;
    }

    if (findWinningMove('O', row, col) || findWinningMove('X', row, col)) {
        board.placeMark(row, col, 'O');
        finishTurn();
        return true;
    }

    if (board.isEmpty(1, 1)) {
        row = 1;
        col = 1;
    } else if (board.isEmpty(0, 0)) {
        row = 0;
        col = 0;
    } else if (board.isEmpty(0, 2)) {
        row = 0;
        col = 2;
    } else if (board.isEmpty(2, 0)) {
        row = 2;
        col = 0;
    } else if (board.isEmpty(2, 2)) {
        row = 2;
        col = 2;
    } else {
        for (row = 0; row < 3; row++) {
            for (col = 0; col < 3; col++) {
                if (board.isEmpty(row, col)) {
                    board.placeMark(row, col, 'O');
                    finishTurn();
                    return true;
                }
            }
        }

        return false;
    }

    board.placeMark(row, col, 'O');
    finishTurn();
    return true;
}

const Board& Game::getBoard() const {
    return board;
}

char Game::getTurn() const {
    return turn;
}

char Game::getWinner() const {
    return winner;
}

bool Game::isOver() const {
    return over;
}

bool Game::isComputerTurn() const {
    return !over && turn == 'O' && playerO.isComputer();
}

void Game::finishTurn() {
    winner = board.getWinner();

    if (winner != ' ' || board.isFull()) {
        over = true;
        return;
    }

    turn = (turn == 'X') ? 'O' : 'X';
}

bool Game::findWinningMove(char mark, int& row, int& col) const {
    for (int testRow = 0; testRow < 3; testRow++) {
        for (int testCol = 0; testCol < 3; testCol++) {
            if (board.isEmpty(testRow, testCol) && wouldWin(mark, testRow, testCol)) {
                row = testRow;
                col = testCol;
                return true;
            }
        }
    }

    return false;
}

bool Game::wouldWin(char mark, int row, int col) const {
    Board testBoard = board;
    testBoard.placeMark(row, col, mark);
    return testBoard.getWinner() == mark;
}
