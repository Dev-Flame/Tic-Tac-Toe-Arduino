#pragma once

class Board {
public:
    Board();
    void clear();
    bool placeMark(int row, int col, char mark);
    void removeMark(int row, int col);
    char getMark(int row, int col) const;
    bool isEmpty(int row, int col) const;
    bool isFull() const;
    char getWinner() const;

private:
    char spaces[3][3];
};
