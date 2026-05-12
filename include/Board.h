#pragma once

class Board {
    private:
        char spaces[3][3];

    public:
        Board();

        void clear();
        bool placeMark(int row, int col, char mark);
        char getMark(int row, int col) const;
        bool isEmpty(int row, int col) const;
        bool isFull() const;
        char getWinner() const;
};
