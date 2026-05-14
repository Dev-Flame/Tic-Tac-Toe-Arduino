#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "Game.h"

MCUFRIEND_kbv screen;
TouchScreen touch(8, A3, A2, 9, 300);
Game game;

const int W = 240, H = 320, BOARD_Y = 40, CELL = 80;
bool onMenu = true;

// -------------------------
// Draw the empty 3x3 grid
// -------------------------
void drawBoard() {
    screen.fillScreen(0x0000);

    screen.fillRect(79,  BOARD_Y,      3, 240, 0xFFFF);
    screen.fillRect(159, BOARD_Y,      3, 240, 0xFFFF);
    screen.fillRect(0,   BOARD_Y + 79, 240, 3, 0xFFFF);
    screen.fillRect(0,   BOARD_Y + 159,240, 3, 0xFFFF);

    screen.fillRect(8,   288, 104, 26, 0x4208);
    screen.fillRect(128, 288, 104, 26, 0x4208);
    screen.setTextColor(0xFFFF); screen.setTextSize(2);
    screen.setCursor(28,  295); screen.print("MENU");
    screen.setCursor(132, 295); screen.print("RESTART");
}

// -------------------------
// Draw X or O in a cell
// -------------------------
void drawMark(int row, int col, char mark) {
    int x = col * CELL, y = BOARD_Y + row * CELL;
    if (mark == 'X') {
        for (int i = -2; i <= 2; i++) {
            screen.drawLine(x+16+i, y+16, x+64+i, y+64, 0xFFE0);
            screen.drawLine(x+16+i, y+64, x+64+i, y+16, 0xFFE0);
        }
    } else {
        for (int r = 25; r <= 29; r++)
            screen.drawCircle(x+40, y+40, r, 0xF800);
    }
}

// -------------------------
// Erase a cell visually
// -------------------------
void eraseMark(int row, int col) {
    screen.fillRect(col*CELL+2, BOARD_Y+row*CELL+2, CELL-4, CELL-4, 0x0000);
}

// -------------------------
// Draw a line through the winning cells
// -------------------------
void drawWinLine() {
    const Board& b = game.getBoard();
    for (int i = 0; i < 3; i++) {
        if (b.getMark(i,0)!=' ' && b.getMark(i,0)==b.getMark(i,1) && b.getMark(i,1)==b.getMark(i,2))
            { screen.fillRect(10, BOARD_Y+i*CELL+38, 220, 4, 0xFFFF); return; }
        if (b.getMark(0,i)!=' ' && b.getMark(0,i)==b.getMark(1,i) && b.getMark(1,i)==b.getMark(2,i))
            { screen.fillRect(i*CELL+38, BOARD_Y+10, 4, 220, 0xFFFF); return; }
    }
    if (b.getMark(0,0)!=' ' && b.getMark(0,0)==b.getMark(1,1) && b.getMark(1,1)==b.getMark(2,2))
        screen.drawLine(10, BOARD_Y+10, 230, BOARD_Y+230, 0xFFFF);
    else
        screen.drawLine(230, BOARD_Y+10, 10, BOARD_Y+230, 0xFFFF);
}

// -------------------------
// Update the status bar
// -------------------------
void updateStatus() {
    screen.fillRect(0, 0, W, BOARD_Y, 0x0000);
    screen.setTextColor(0xFFFF); screen.setTextSize(2);
    screen.setCursor(8, 12);
    if (game.isOver()) {
        char winner = game.getWinner();
        if (winner == ' ') screen.print("Draw!");
        else { screen.print(winner); screen.print(" wins!"); drawWinLine(); }
    } else {
        screen.print(game.getTurn()); screen.print(" turn");
    }
}

// -------------------------
// Show the main menu
// -------------------------
void showMenu() {
    screen.fillScreen(0x0000);
    screen.setTextColor(0xFFFF); screen.setTextSize(3);
    screen.setCursor(10, 40); screen.print("TIC TAC TOE");

    screen.fillRect(25, 120, 190, 55, 0x03BF);
    screen.setTextSize(2); screen.setCursor(45, 140); screen.print("2 PLAYERS");

    screen.fillRect(25, 190, 190, 55, 0x07E0);
    screen.setCursor(65, 210); screen.print("PLAY AI");
}

// -------------------------
// Read touch input
// -------------------------
bool readTouch(int &x, int &y) {
    TSPoint p = touch.getPoint();
    pinMode(A2, OUTPUT); pinMode(A3, OUTPUT);
    if (p.z < 200 || p.z > 1000) return false;
    x = constrain(map(p.x, 120, 900, W, 0), 0, W-1);
    y = constrain(map(p.y, 70,  920, 0, H), 0, H-1);
    return true;
}

// -------------------------
void setup() {

    Serial.begin(9600);
    uint16_t id = screen.readID();
    if (id == 0x0000 || id == 0xFFFF) id = 0x9341;
    screen.begin(id);
    screen.setRotation(0);
    showMenu();

}


void loop() {
    // --- EXPIRATION LOGIC ---
    if (!onMenu && !game.isOver()) {
        auto checkExpired = [](Player &p) {
        Mark m = p.getExpired();
        if (m.row != -1) {
            eraseMark(m.row, m.col);
            game.removeMark(m.row, m.col); // <--- Clears the ghost mark from the game board
        }
    };
        checkExpired(game.getPlayerX());
        checkExpired(game.getPlayerO());
    }

    // --- TOUCH LOGIC ---
    int x, y;
    if (!readTouch(x, y)) return;

    if (onMenu) {
        if (x>=25 && x<=215 && y>=120 && y<=175) { game.start(TWO_PLAYERS);        onMenu=false; drawBoard(); updateStatus(); }
        if (x>=25 && x<=215 && y>=190 && y<=245) { game.start(PLAYER_VS_COMPUTER); onMenu=false; drawBoard(); updateStatus(); }
    } else {
        if (x>=8   && x<=112 && y>=288 && y<=314) { onMenu=true; showMenu(); return; }
        if (x>=128 && x<=232 && y>=288 && y<=314) { game.start(game.isComputerTurn() || game.getPlayerO().isComputer() ? PLAYER_VS_COMPUTER : TWO_PLAYERS); drawBoard(); updateStatus(); return; }

        if (!game.isOver() && y>=BOARD_Y && y<BOARD_Y+240) {
            int row = (y-BOARD_Y)/CELL, col = x/CELL;
            char prevTurn = game.getTurn(); // save turn BEFORE the move advances it
            if (game.playHumanMove(row, col)) {
                drawMark(row, col, prevTurn);
                updateStatus();
                if (game.isComputerTurn()) {
                    delay(400);
                    int r, c;
                    game.playComputerMove(r, c);
                    drawMark(r, c, 'O');
                    updateStatus();
                }
            }
        }
    }

    delay(180);
}
