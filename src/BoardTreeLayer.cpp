#include "BoardTreeLayer.hpp"


// Initializes the empty board, the very first layer of the board tree
// Starts the chain of creating the layers of boards in the tree
BoardTreeLayer::BoardTreeLayer() : currentMark(Mark(Symbol::X)), layerStates(new Board[1]), layerStateScores(new int(0)), nextLayers(new BoardTreeLayer*[9]) {
    constructNextLayers();
}

// Used to create a layer of all possible boards from a previous board
BoardTreeLayer::BoardTreeLayer(int size, Board* newLayer, BoardTreeLayer* prevLayer) : numStates(size), layerStates(newLayer), layerStateScores(new int[size]), nextLayers(new BoardTreeLayer*[size]), prevLayer(prevLayer) {
    constructNextLayers();
};

// Destructor
BoardTreeLayer::~BoardTreeLayer() {
    delete [] layerStates;
    delete [] layerStateScores;

    // Recursively delete the layers that follow
    for (int i = 0; i < numStates; i++) {
        delete nextLayers[i];
    }

    delete [] nextLayers;
}

void BoardTreeLayer::constructNextLayers() {
    Board* newLayer;
    Board* newBoard;
    Board* currentBoard;
    int newBoards = 0;

    // Create a new layer of boards for each board in the current layer
    for (int i = 0; i < numStates; i++) {
        currentBoard = &layerStates[i];

        // Compute the number of boards that will be in the new layer
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                // We using dot notation instead of ->, since accessing via index gives us the actual object
                if (currentBoard->getMark(j, k).getSymbol() == Symbol::EMPTY) {
                    newBoards++;
                }
            }
        }

        // Allocate memory for new layer of boards
        newLayer = new Board[newBoards];


        for (int j = 0; j < newBoards; j++) {
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    newBoard = new Board(*currentBoard);
                }
            }
        }
    }
}