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

    // Convert move to its mapped value
    int convert = (row * 3) + (col + 1);
    int temp;

    // Search for mapped value in availableMoves deque
    while (availableMoves.front() != convert) {
        temp  = availableMoves.front();
        availableMoves.pop_front();
        availableMoves.push_back(temp);
    }

    // Permanently remove it from the deque
    availableMoves.pop_front();
    
    getCurrentPlayer().addMark(row, col);
    finishTurn();
    return true;
}

bool Game::playComputerMove(int &row, int &col) {
    if (!isComputerTurn() || board.isFull()) {
        return false;
    }

    // Choose random move from availableMoves deque
    int choice = (random() % availableMoves.size()) + 1;
    int counter = 1;
    int temp;

    while (counter != choice) {
        temp = availableMoves.front();
        availableMoves.pop_front();
        availableMoves.push_back(temp);
    }

    row = (availableMoves.front() / 3) - 1;
    col = (availableMoves.front() % 3);

    // Permanently remove chosen move from deque
    availableMoves.pop_front();
    
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