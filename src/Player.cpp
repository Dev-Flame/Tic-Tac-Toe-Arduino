#include "Player.h"
#include <Arduino.h>

Player::Player() : mark('X'), computer(false) {}

Player::Player(char playerMark, bool isComputer) : mark(playerMark), computer(isComputer) {}

char Player::getMark() const { return mark; }
bool Player::isComputer() const { return computer; }

void Player::addMark(int row, int col) {
    markQueue.push({row, col, millis()});
}

// Returns the expired mark if one is older than 6 seconds, otherwise {-1,-1,0}
Mark Player::getExpired() {
    if (!markQueue.empty() && millis() - markQueue.front().time > 6000) {
        Mark m = markQueue.front();
        markQueue.pop();
        return m;
    }
    return {-1, -1, 0};
}

void Player::clearMarks() {
    while (!markQueue.empty()) markQueue.pop();
}
