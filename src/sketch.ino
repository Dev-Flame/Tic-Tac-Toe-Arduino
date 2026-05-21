#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "Game.h"

MCUFRIEND_kbv screen;
TouchScreen touch(8, A3, A2, 9, 300);
Game game;

const int screenWidth = 240;
const int screenHeight = 320;
const int BOARD_Y = 40;
const int CELL = 80;

bool onMenu = true;

// -------------------------
// Draw the empty 3x3 grid
// -------------------------
void drawBoard() {
    screen.fillScreen(0x0000);
    
    // Vertical lines
    screen.fillRect(79,  BOARD_Y, 3, 240, 0xFFFF);
    screen.fillRect(159, BOARD_Y, 3, 240, 0xFFFF);
    
    // Horizontal lines
    screen.fillRect(0, BOARD_Y + 79, 240, 3, 0xFFFF);
    screen.fillRect(0, BOARD_Y + 159, 240, 3, 0xFFFF);

    // Bottom buttons
    screen.fillRect(8, 288, 104, 26, 0x4208);
    screen.fillRect(128, 288, 104, 26, 0x4208);
    
    screen.setTextColor(0xFFFF); 
    screen.setTextSize(2);
    screen.setCursor(38, 295); 
    screen.print("MENU");
    screen.setCursor(140, 295); 
    screen.print("RESTART");
}

// -------------------------
// Draw X or O in a cell
// -------------------------
void drawMark(int row, int col, char mark) {
    int x = col * CELL;
    int y = BOARD_Y + (row * CELL);
    
    if (mark == 'X') {
        for (int i = -2; i <= 2; i++) {
            screen.drawLine(x + 16 + i, y + 16, x + 64 + i, y + 64, 0xFFE0);
            screen.drawLine(x + 16 + i, y + 64, x + 64 + i, y + 16, 0xFFE0);
        }
    } else {
        // Draw O
        for (int r = 25; r <= 29; r++) {
            screen.drawCircle(x + 40, y + 40, r, 0xF800);
        }
    }
}

// -------------------------
// Erase a cell visually
// -------------------------
void eraseMark(int row, int col) {
    screen.fillRect(col * CELL + 2, BOARD_Y + row * CELL + 2, CELL - 4, CELL - 4, 0x0000);
}

// -------------------------
// Draw a line through the winning cells
// -------------------------
void drawWinLine() {
    const Board& b = game.getBoard();
    
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (b.getMark(i,0) != ' ' && b.getMark(i,0) == b.getMark(i,1) && b.getMark(i,1) == b.getMark(i,2)) { 
            screen.fillRect(10, BOARD_Y + i * CELL + 38, 220, 4, 0xFFFF);
            return; 
        }
        // Check columns
        if (b.getMark(0,i) != ' ' && b.getMark(0,i) == b.getMark(1,i) && b.getMark(1,i) == b.getMark(2,i)) { 
            screen.fillRect(i * CELL + 38, BOARD_Y + 10, 4, 220, 0xFFFF);
            return; 
        }
    }
    
    // Check diagonals
    if (b.getMark(0,0) != ' ' && b.getMark(0,0) == b.getMark(1,1) && b.getMark(1,1) == b.getMark(2,2)) {
        screen.drawLine(10, BOARD_Y + 10, 230, BOARD_Y + 230, 0xFFFF);
    } else {
        screen.drawLine(230, BOARD_Y + 10, 10, BOARD_Y + 230, 0xFFFF);
    }
}

// -------------------------
// Update the status bar
// -------------------------
void updateStatus() {
    screen.fillRect(0, 0, screenWidth, BOARD_Y, 0x0000);
    screen.setTextColor(0xFFFF); 
    screen.setTextSize(2);
    screen.setCursor(86, 12);
    
    if (game.isOver()) {
        char winner = game.getWinner();
        if (winner == ' ') {
            screen.print("Draw!");
        } else { 
            screen.setCursor(84, 12);
            screen.print(winner); 
            screen.print(" wins!"); 
            drawWinLine();
        }
    } else {
        screen.print(game.getTurn()); 
        screen.print(" turn");
    }
}

// -------------------------
// Show the main menu
// -------------------------
void showMenu() {
    screen.fillScreen(0xFFFF);  // White background
    
    // Title
    screen.setTextColor(0x0000); 
    screen.setTextSize(3);
    screen.setCursor(20, 20); 
    screen.print("TIC TAC TOE");
    
    // Draw bigger decorative tic tac toe grid under title
    int gridX = 80;
    int gridY = 75;
    int gridSize = 70;
    int cellSize = 23;
    
    // Grid border
    screen.drawRect(gridX, gridY, gridSize, gridSize, 0x0000);
    
    // Draw grid lines
    for (int i = 1; i < 3; i++) {
        screen.drawLine(gridX + i * cellSize, gridY, gridX + i * cellSize, gridY + gridSize, 0x0000);
        screen.drawLine(gridX, gridY + i * cellSize, gridX + gridSize, gridY + i * cellSize, 0x0000);
    }
    
    // Add some X's and O's to the grid (top-left X, middle O, bottom-right X)
    // Top-left X
    for (int i = -2; i <= 2; i++) {
        screen.drawLine(gridX + 5 + i, gridY + 5, gridX + 15 + i, gridY + 15, 0xF800);
        screen.drawLine(gridX + 5 + i, gridY + 15, gridX + 15 + i, gridY + 5, 0xF800);
    }
    
    // Middle O
    for (int r = 6; r <= 8; r++) {
        screen.drawCircle(gridX + 35, gridY + 35, r, 0x07E0);
    }
    
    // Bottom-right X
    for (int i = -2; i <= 2; i++) {
        screen.drawLine(gridX + 50 + i, gridY + 50, gridX + 60 + i, gridY + 60, 0xF800);
        screen.drawLine(gridX + 50 + i, gridY + 60, gridX + 60 + i, gridY + 50, 0xF800);
    }
    
    // 2 Player Button
    screen.fillRect(25, 180, 190, 55, 0x4810);
    screen.setTextColor(0xFFFF);
    screen.setTextSize(2); 
    screen.setCursor(67, 200); 
    screen.print("2 PLAYERS");

    // Play AI Button
    screen.fillRect(25, 250, 190, 55, 0x07E0);
    screen.setTextColor(0x0000);
    screen.setCursor(80, 270); 
    screen.print("PLAY AI");
}

// -------------------------
// Read touch input
// -------------------------

bool readTouch(int &x, int &y) {
    TSPoint p = touch.getPoint();
    
    // Reset pins so the screen can draw properly
    pinMode(A2, OUTPUT); 
    pinMode(A3, OUTPUT);
    
    // Ignore accidental or light touches
    if (p.z < 200 || p.z > 1000) {
        return false;
    }
    
    // Map raw hardware values to the 240x320 screen dimensions
    x = constrain(map(p.x, 120, 900, screenWidth, 0), 0, screenWidth - 1);
    y = constrain(map(p.y, 70,  920, 0, screenHeight), 0, screenHeight - 1);
    return true;
}

// -------------------------
// Helper function to handle expiring marks
// -------------------------
void checkAndEraseExpiredMarks() {
    // Check Player X
    Mark mX = game.getPlayerX().getExpired();
    if (mX.row != -1) {
        eraseMark(mX.row, mX.col);
        game.removeMark(mX.row, mX.col);
    }

    // Check Player O
    Mark mO = game.getPlayerO().getExpired();
    if (mO.row != -1) {
        eraseMark(mO.row, mO.col);
        game.removeMark(mO.row, mO.col);
    }
}

// -------------------------
void setup() {
    Serial.begin(9600);
    uint16_t id = screen.readID();
    
    if (id == 0x0000 || id == 0xFFFF) {
        id = 0x9341;
    }
    
    screen.begin(id);
    screen.setRotation(0);
    showMenu();
}

void loop() {
    // --- EXPIRATION LOGIC ---
    if (!onMenu && !game.isOver()) {
        checkAndEraseExpiredMarks();
    }

    // --- TOUCH LOGIC ---
    int x, y;
    if (!readTouch(x, y)) {
        return; // No touch detected
    }

    if (onMenu) {
        // Clicked 2 Players
        if (x >= 25 && x <= 215 && y >= 180 && y <= 235) { 
            game.start(TWO_PLAYERS);
            onMenu = false; 
            drawBoard(); 
            updateStatus(); 
        }
        // Clicked Play AI
        if (x >= 25 && x <= 215 && y >= 250 && y <= 305) { 
            game.start(PLAYER_VS_COMPUTER);
            onMenu = false; 
            drawBoard(); 
            updateStatus(); 
        }
    } else {
        // Clicked Menu
        if (x >= 8 && x <= 112 && y >= 288 && y <= 314) { 
            onMenu = true;
            showMenu(); 
            return; 
        }
        // Clicked Restart
        if (x >= 128 && x <= 232 && y >= 288 && y <= 314) { 
            if (game.isComputerTurn() || game.getPlayerO().isComputer()) {
                game.start(PLAYER_VS_COMPUTER);
            } else {
                game.start(TWO_PLAYERS);
            }
            drawBoard(); 
            updateStatus(); 
            return; 
        }

        // Clicked the Board
        if (!game.isOver() && y >= BOARD_Y && y < BOARD_Y + 240) {
            int row = (y - BOARD_Y) / CELL;
            int col = x / CELL;
            
            char prevTurn = game.getTurn(); // save turn BEFORE the move advances it
            
            if (game.playHumanMove(row, col)) {
                drawMark(row, col, prevTurn);
                game.getPlayerX().tickMarks();
                game.getPlayerO().tickMarks();
                updateStatus();

                if (game.isComputerTurn()) {
                    delay(400);
                    int r, c;
                    game.playComputerMove(r, c);
                    drawMark(r, c, 'O');
                    game.getPlayerX().tickMarks();
                    game.getPlayerO().tickMarks();
                    updateStatus();
                }
            }
        }
    }

    delay(180); // Debounce delay
}
