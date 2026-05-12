#pragma once

class Player {
    private:
        char mark;
        bool computer;

    public:
        Player();
        Player(char playerMark, bool isComputer);

        char getMark() const;
        bool isComputer() const;
};
