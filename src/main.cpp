/*
Useful Info:
- Screen size of ILI9341: 240 x 320
	- Width of vertical lines = 3 px
	- Each Tic-Tac-Toe tile will be (240 - 6) / 3 = 78 by 78 pixels
- Text size of 1 --> each character occupies a 6x8 block



 */


#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"
#include "U8g2_for_Adafruit_GFX.h"

#include "Tic-Tac-Toe.hpp"


#define   	TFT_CS    			4
#define   	TFT_DC    			3
#define 	TFT_RST   			2

// ASSUMPTION: Screen is oriented vertically
#define		SCREEN_WIDTH		240
#define		SCREEN_HEIGHT		320

#define		BOARD_SIDE_LENGTH	240
#define		TILE_SIDE_LENGTH	78
#define		LINE_WIDTH 			3

#define		ROWS				3
#define		COLUMNS				3

const int pinsToInitiateSIZE = 3;
const int pinsToInitiate[pinsToInitiateSIZE] = {TFT_CS, TFT_DC, TFT_RST}; 

Adafruit_ILI9341 display(TFT_CS, TFT_DC, TFT_RST);
Adafruit_FT6206 touchController = Adafruit_FT6206();

TicTacToe game;


// Function prototypes
void drawBoard(Board* board);
void drawCircle(const int row, const int col);
void drawLines();
void drawX(const int row, const int col);
void initiatePins(const int pins[], const int size);


// Setup + loop ------------------------------------------------------------
void setup() {
	// Serial monitor for output to console, baud rate 9600
	Serial.begin(9600);

	// Set CS, DC, and RST pins of ILI9341 to OUTPUT mode
	initiatePins(pinsToInitiate, pinsToInitiateSIZE);

	// Initate the ILI9341 display
	display.begin();
	display.fillScreen(ILI9341_BLACK);

	// Initiate the capacitive touch screen and check that it is successfully connected
	// We want to check this, because when debugging touch errors we want know for sure that it's due to
	//		a bug in the code rather than from the FT6206 failing to communicate with the Arduino
	// If it is not detected, enter an infinite loop and output an error message
	if (!touchController.begin()) {
		Serial.println("Error: FT6206 controller not detected.");
		while (true);
	}

	drawLines();

	// Initiate the tic-tac-toe game
	game = TicTacToe();
}

void loop() {
	if (touchController.touched()) {
		TS_Point point = touchController.getPoint();

		// By default, the FT6206's origin is the lower right corner of the ILI9341
		// Remap the coordinates to move it to the upper left corner
		int x = map(point.x, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0);
		int y = map(point.y, 0, SCREEN_HEIGHT, SCREEN_HEIGHT, 0);

		if (!game.isDone()) {
			int counter = 0;
			int row, col;

			// Determine which column was tapped
			while (!((x >= (TILE_SIDE_LENGTH + LINE_WIDTH) * counter) && (x <= (TILE_SIDE_LENGTH * (counter + 1) + LINE_WIDTH * counter)))) {
				counter++;
			}

			col = counter;
			counter = 0;

			// Determine which row was tapped
			while (!((y >= (TILE_SIDE_LENGTH + LINE_WIDTH) * counter) && (y <= (TILE_SIDE_LENGTH * (counter + 1) + LINE_WIDTH * counter)))) {
				counter++;
			}

			row = counter;

			// If valid tile is pressed, make the move
			if (row < 3 && col < 3) {
				Serial.print("Row: ");
				Serial.print(row);
				Serial.println();
				Serial.print("Column: ");
				Serial.print(col);
				Serial.println();

				game.makeMove(row, col);
				drawBoard(game.getBoard());
			}
		} else {
			Serial.println("game done");
		}

	}
}

// Other functions ------------------------------------------------------------
void drawBoard(Board* board) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (board->getMark(i, j).getSymbol() == Symbol::X) {
				drawX(i, j);
			} else if (board->getMark(i, j).getSymbol() == Symbol::O) {
				drawCircle(i, j);
			}
		}
	}
}

void drawCircle(const int row, const int col) {
	display.drawCircle((TILE_SIDE_LENGTH / 2) + (TILE_SIDE_LENGTH + LINE_WIDTH) * col, (TILE_SIDE_LENGTH / 2) + (TILE_SIDE_LENGTH + LINE_WIDTH) * row, TILE_SIDE_LENGTH / 2, ILI9341_WHITE);
}

// Draws lines separating the marks on the board
void drawLines() {
	// Vertical lines
	display.fillRect(TILE_SIDE_LENGTH, 0, LINE_WIDTH, BOARD_SIDE_LENGTH, ILI9341_WHITE);
	display.fillRect(2 * TILE_SIDE_LENGTH + LINE_WIDTH, 0, LINE_WIDTH, BOARD_SIDE_LENGTH, ILI9341_WHITE);

	// Horizontal lines
	display.fillRect(0, TILE_SIDE_LENGTH, BOARD_SIDE_LENGTH, LINE_WIDTH, ILI9341_WHITE);
	display.fillRect(0, 2 * TILE_SIDE_LENGTH, BOARD_SIDE_LENGTH, LINE_WIDTH, ILI9341_WHITE);
}

void drawX(const int row, const int col) {
	int leftX = (TILE_SIDE_LENGTH + LINE_WIDTH) * col;
	int rightX = TILE_SIDE_LENGTH * (col + 1) + LINE_WIDTH * col;
	int upY = (TILE_SIDE_LENGTH + LINE_WIDTH) * row;
	int downY = TILE_SIDE_LENGTH * (row + 1) + LINE_WIDTH * row;

	display.drawLine(leftX, upY, rightX, downY, ILI9341_WHITE);
	display.drawLine(leftX, downY, rightX, upY, ILI9341_WHITE);
}

void initiatePins(const int pins[], const int size) {
	for (int i = 0; i < size; i++) {
		pinMode(pins[i], OUTPUT);
	}
}