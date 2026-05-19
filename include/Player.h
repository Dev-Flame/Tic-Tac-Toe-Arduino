#pragma once
#include <queue>

struct Mark {
    int row, col;
};

class Player {
public:
    Player();
    Player(char playerMark, bool isComputer);

    char getMark() const;
    bool isComputer() const;

    void addMark(int row, int col);
    Mark getExpired();   // returns {-1,-1} if nothing expired
    void clearMarks();

private:
    char mark;
    bool computer;
    std::queue<Mark> markQueue;
};
