#pragma once

#include "Mark.hpp"

class Board {
    private:
        Mark state[3][3];
        int numMarks;

    public:
        Board();
        Board(const Board& boardToCopy);

        void clear();

        Mark getMark(const int row, const int col) const;
        void setMark(const int row, const int col, const Mark &mark);
        int getNumMarks() const;

        bool checkWin(Mark& markCheck) const;
};