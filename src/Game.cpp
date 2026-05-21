#include "Game.h"
#include <Arduino.h>

Game::Game() : playerX('X', false), playerO('O', false) {
    start(TWO_PLAYERS);
}

void Game::start(GameMode mode) {
    board.clear();
    playerX = Player('X', false);
    
    if (mode == PLAYER_VS_COMPUTER) {
        playerO = Player('O', true);
    } else {
        playerO = Player('O', false);
    }
    
    playerX.clearMarks();
    playerO.clearMarks();
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
    getCurrentPlayer().addMark(row, col);
    finishTurn();
    return true;
}

bool Game::playComputerMove(int &row, int &col) {
    if (!isComputerTurn() || board.isFull()) {
        return false;
    }
    // Check if AI can win
    row = -1; col = -1;
    if (findWinningMove('O', row, col)) {
        board.placeMark(row, col, turn);
        getCurrentPlayer().addMark(row, col);
        finishTurn();
        return true;
    }

    // Block opponent's winning move
    if (findWinningMove('X', row, col)) {
        board.placeMark(row, col, turn);
        getCurrentPlayer().addMark(row, col);
        finishTurn();
        return true;
    }
    

    // Collect all empty cells
    int moves[9];
    int count = 0;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (board.isEmpty(r, c)) {
                moves[count++] = r * 3 + c;
            }
        }
    }

    if (count == 0) return false;

    int idx = random(count);
    int choice = moves[idx];
    row = choice / 3;
    col = choice % 3;

    board.placeMark(row, col, turn);
    getCurrentPlayer().addMark(row, col);
    finishTurn();
    return true;
}

const Board& Game::getBoard() const { 
    return board; 
}

Player& Game::getPlayerX() { 
    return playerX; 
}

Player& Game::getPlayerO() { 
    return playerO; 
}

Player& Game::getCurrentPlayer() { 
    if (turn == 'X') {
        return playerX;
    } else {
        return playerO;
    }
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
    if (!over && turn == 'O' && playerO.isComputer()) {
        return true;
    }
    return false;
}

void Game::finishTurn() {
    winner = board.getWinner();
    
    if (winner != ' ' || board.isFull()) { 
        over = true; 
        return; 
    }
    
    if (turn == 'X') {
        turn = 'O';
    } else {
        turn = 'X';
    }
}

void Game::removeMark(int row, int col) {
    board.removeMark(row, col);
}

bool Game::findWinningMove(char mark, int &row, int &col) const {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (board.isEmpty(r, c) && wouldWin(mark, r, c)) {
                row = r;
                col = c;
                return true;
            }
        }
    }
    return false;
}

bool Game::wouldWin(char mark, int row, int col) const {
    board.placeMark(row, col, mark);
    bool win = (board.getWinner() == mark);
    board.removeMark(row, col);
    return win;
}