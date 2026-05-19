#include "Player.h"
#include <Arduino.h>

Player::Player() : mark('X'), computer(false) {}

Player::Player(char playerMark, bool isComputer) : mark(playerMark), computer(isComputer) {}

char Player::getMark() const { 
    return mark; 
}

bool Player::isComputer() const { 
    return computer; 
}

void Player::addMark(int row, int col) {
    markQueue.push({row, col});
}

// Returns the expired mark, otherwise {-1,-1}
Mark Player::getExpired() {
    // Randomly expires after 2, 3, or 4 marks are currently on the board
    if (markQueue.size() > random(2, 5)) {
        Mark m = markQueue.front();
        markQueue.pop();
        return m;
    }
    
    return {-1, -1};
}

void Player::clearMarks() {
    while (!markQueue.empty()) {
        markQueue.pop();
    }
}