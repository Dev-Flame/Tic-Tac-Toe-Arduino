#pragma once
#include <queue>

struct Mark {
    int row,
     col,
     turnsLeft;
};

class Player {
public:
    Player();
    Player(char playerMark, bool isComputer);

    char getMark() const;
    bool isComputer() const;

    void addMark(int row, int col);
    void tickMarks();
    Mark getExpired();
    void clearMarks();

private:
    char mark;
    bool computer;
    std::queue<Mark> markQueue;
};