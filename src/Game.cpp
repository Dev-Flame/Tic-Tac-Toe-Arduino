#include "Game.h"

Game::Game() : playerX('X', false), playerO('O', false) {
    start(TWO_PLAYERS);
}

void Game::start(GameMode mode) {
    board.clear();
    playerX = Player('X', false);
    playerO = Player('O', mode == PLAYER_VS_COMPUTER);
    playerX.clearMarks();
    playerO.clearMarks();
    turn = 'X';
    winner = ' ';
    over = false;
}

bool Game::playHumanMove(int row, int col) {
    if (over || isComputerTurn()) return false;
    if (!board.placeMark(row, col, turn)) return false;
    getCurrentPlayer().addMark(row, col);
    finishTurn();
    return true;
}

bool Game::playComputerMove(int &row, int &col) {
    if (!isComputerTurn() || board.isFull() ) return false;

    do {
        row = random() % 3;
        col = random() % 3;
    }
    while(!board.isEmpty(row, col));
    board.placeMark(row, col, turn);
    getCurrentPlayer().addMark(row, col);
    finishTurn();
    return true;
    
   
}

const Board& Game::getBoard() const { return board; }
Player& Game::getPlayerX() { return playerX; }
Player& Game::getPlayerO() { return playerO; }
Player& Game::getCurrentPlayer() { return turn == 'X' ? playerX : playerO; }
char Game::getTurn() const { return turn; }
char Game::getWinner() const { return winner; }
bool Game::isOver() const { return over; }
bool Game::isComputerTurn() const { return !over && turn == 'O' && playerO.isComputer(); }

void Game::finishTurn() {
    winner = board.getWinner();
    if (winner != ' ' || board.isFull()) { over = true; return; }
    turn = (turn == 'X') ? 'O' : 'X';
}



void Game::removeMark(int row, int col) {
    board.removeMark(row, col);
}