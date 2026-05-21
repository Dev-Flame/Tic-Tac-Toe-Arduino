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
    markQueue.push({row, col, random(2, 9)}); // Lifespan between 2 and 8 turns
}

void Player::tickMarks() {
    for (int i = markQueue.size(); i > 0; i--) {
        Mark m = markQueue.front();
        markQueue.pop();
        m.turnsLeft--;
        markQueue.push(m);
    }
}

Mark Player::getExpired() {
    if (!markQueue.empty() && markQueue.front().turnsLeft <= 0) {
        Mark m = markQueue.front();
        markQueue.pop();
        return m;
    }
    return {-1, -1, 0};
}

void Player::clearMarks() {
    while (!markQueue.empty()) markQueue.pop();
}