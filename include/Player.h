#pragma once
#include <queue>

struct Mark {
    int row, col;
    unsigned long time;
};

class Player {
public:
    Player();
    Player(char playerMark, bool isComputer);

    char getMark() const;
    bool isComputer() const;

    void addMark(int row, int col);
    Mark getExpired();   // returns {-1,-1,0} if nothing expired
    void clearMarks();

private:
    char mark;
    bool computer;
    std::queue<Mark> markQueue;
};
