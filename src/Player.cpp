#include "Player.h"

Player::Player() : mark('X'), computer(false) {
}

Player::Player(char playerMark, bool isComputer) : mark(playerMark), computer(isComputer) {
}

char Player::getMark() const {
    return mark;
}

bool Player::isComputer() const {
    return computer;
}
