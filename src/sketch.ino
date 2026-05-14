#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <queue>

MCUFRIEND_kbv screen;
TouchScreen touch(8, A3, A2, 9, 300);

// --- Constants ---
const int W = 240, H = 320;
const int BOARD_Y = 40;
const int CELL = 80;

// --- Queue Setup for Disappearing Marks ---
struct Move {
	int r, c;
	unsigned long time;
};
std::queue<Move> moveQueue;

// --- Game State ---
char board[3][3];
char turn = 'X';
bool gameOver = false;
bool vsAI = false;
bool onMenu = true;

// -------------------------
// Draw the empty 3x3 grid
// -------------------------
void drawBoard() {
	screen.fillScreen(0x0000); // black

	// Grid lines
	screen.fillRect(79, BOARD_Y, 3, 240, 0xFFFF);
	screen.fillRect(159, BOARD_Y, 3, 240, 0xFFFF);
	screen.fillRect(0, BOARD_Y + 79, 240, 3, 0xFFFF);
	screen.fillRect(0, BOARD_Y + 159, 240, 3, 0xFFFF);

	// Buttons at bottom
	screen.fillRect(8, 288, 104, 26, 0x4208);
	screen.fillRect(128, 288, 104, 26, 0x4208);

	screen.setTextColor(0xFFFF);
	screen.setTextSize(2);

	screen.setCursor(28, 295);
	screen.print("MENU");

	screen.setCursor(132, 295);
	screen.print("RESTART");
}

// -------------------------
// Draw X or O in a cell
// -------------------------
void drawMark(int row, int col, char mark) {
	int x = col * CELL;
	int y = BOARD_Y + row * CELL;

	if (mark == 'X') {
		for (int i = -2; i <= 2; i++) {
			screen.drawLine(x + 16 + i, y + 16, x + 64 + i, y + 64, 0xFFE0); // yellow
			screen.drawLine(x + 16 + i, y + 64, x + 64 + i, y + 16, 0xFFE0);
		}
	} else {
		for (int r = 25; r <= 29; r++) {
			screen.drawCircle(x + 40, y + 40, r, 0xF800); // red
		}
	}
}

// -------------------------
// Show whose turn it is (or who won)
// -------------------------
void updateStatus() {
	screen.fillRect(0, 0, W, BOARD_Y, 0x0000);
	screen.setTextColor(0xFFFF);
	screen.setTextSize(2);
	screen.setCursor(8, 12);

	if (gameOver) {
		char winner = ' ';

		// Check rows, cols, diagonals
		for (int i = 0; i < 3; i++) {
			if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
				winner = board[i][0];
			}
			if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
				winner = board[0][i];
			}
		}

		if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
			winner = board[0][0];
		}
		if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
			winner = board[0][2];
		}

		if (winner == ' ') {
			screen.print("Draw!");
		} else {
			screen.print(winner);
			screen.print(" wins!");
		}
	} else {
		screen.print(turn);
		screen.print(" turn");
	}
}

// -------------------------
// Check if the game is over
// -------------------------
bool checkOver() {
	// Check for a winner
	for (int i = 0; i < 3; i++) {
		if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
		if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
	}
	if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
	if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;

	// Check for a draw (no empty cells left)
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			if (board[r][c] == ' ') return false;
		}
	}
	return true; // draw
}

// -------------------------
// AI picks a random empty cell
// -------------------------
void aiMove() {
	delay(400);

	int r, c;
	do {
		r = random(3);
		c = random(3);
	} while (board[r][c] != ' ');

	board[r][c] = 'O';
	drawMark(r, c, 'O');
	moveQueue.push({r, c, millis()}); // Queue the AI's move

	gameOver = checkOver();
	turn = 'X';
	updateStatus();
}

// -------------------------
// Reset board and redraw
// -------------------------
void startGame() {
	// Clear the board array
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			board[r][c] = ' ';
		}
	}

	// Empty the queue
	while (!moveQueue.empty()) {
		moveQueue.pop();
	}

	turn = 'X';
	gameOver = false;

	drawBoard();
	updateStatus();
}

// -------------------------
// Show the main menu
// -------------------------
void showMenu() {
	screen.fillScreen(0x0000);
	screen.setTextColor(0xFFFF);
	screen.setTextSize(3);
	screen.setCursor(10, 40);
	screen.print("TIC TAC TOE");

	// 2 Players button
	screen.fillRect(25, 120, 190, 55, 0x03BF); // blue
	screen.setTextSize(2);
	screen.setCursor(45, 140);
	screen.print("2 PLAYERS");

	// vs AI button
	screen.fillRect(25, 190, 190, 55, 0x07E0); // green
	screen.setCursor(65, 210);
	screen.print("PLAY AI");
}

// -------------------------
// Read touch input
// -------------------------
bool readTouch(int &x, int &y) {
	TSPoint p = touch.getPoint();

	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);

	if (p.z < 200 || p.z > 1000) {
		return false;
	}

	x = constrain(map(p.x, 120, 900, W, 0), 0, W - 1);
	y = constrain(map(p.y, 70, 920, 0, H), 0, H - 1);

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
	// Add !gameOver so pieces stop disappearing once someone wins!
	if (!onMenu && !gameOver && !moveQueue.empty() && millis() - moveQueue.front().time > 6000) {
		Move oldest = moveQueue.front();
		moveQueue.pop();
		
		board[oldest.r][oldest.c] = ' '; // Clear from game logic
		
		// Clear visually using an inset fillRect to save grid lines
		int cellX = oldest.c * CELL;
		int cellY = BOARD_Y + (oldest.r * CELL);
		screen.fillRect(cellX + 2, cellY + 2, CELL - 4, CELL - 4, 0x0000); 
		
		gameOver = checkOver(); // Update game state
		updateStatus();
	}


	
	// --- TOUCH LOGIC ---
	int x, y;
	if (!readTouch(x, y)) {
		return;
	}

	if (onMenu) {
		// Pick mode from menu
		if (x >= 25 && x <= 215 && y >= 120 && y <= 175) {
			vsAI = false;
			onMenu = false;
			startGame();
		}
		if (x >= 25 && x <= 215 && y >= 190 && y <= 245) {
			vsAI = true;
			onMenu = false;
			startGame();
		}
	} else {
		// Menu button
		if (x >= 8 && x <= 112 && y >= 288 && y <= 314) {
			onMenu = true;
			showMenu();
			return;
		}

		// Restart button
		if (x >= 128 && x <= 232 && y >= 288 && y <= 314) {
			startGame();
			return;
		}

		// Board tap
		if (!gameOver && y >= BOARD_Y && y < BOARD_Y + 240) {
			int row = (y - BOARD_Y) / CELL;
			int col = x / CELL;

			if (board[row][col] == ' ') {
				board[row][col] = turn;
				drawMark(row, col, turn);
				
				// Queue the player's move
				moveQueue.push({row, col, millis()});

				gameOver = checkOver();
				turn = (turn == 'X') ? 'O' : 'X';
				updateStatus();

				if (vsAI && !gameOver && turn == 'O') {
					aiMove();
				}
			}
		}
	}

	delay(180); // Simple debounce
}