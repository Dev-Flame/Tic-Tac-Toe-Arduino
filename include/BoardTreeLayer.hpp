#pragma once


#include "Board.hpp"


class BoardTreeLayer {
    private:
        // The current "turn" of the layer
        Mark currentMark;
        // Number of states in this layer
        int numStates;

        // Stores an array of possible board states
        Board* layerStates;
        // Each state in the layer will be assigned a numerical score
        int* layerStateScores;
        // Tells us which states in the layer are in winning states
        bool* winningStates;

        // Each board in layerStates will have a corresponding link to the next possible layer of states
        // A pointer to an array of pointers
        BoardTreeLayer** nextLayers;

        // Each layer will have a pointer to the previous layer and an integer for the index of the specific board that constructed it
        BoardTreeLayer* prevLayer;
        int prevLayerBoard;


    public:
        BoardTreeLayer();
        BoardTreeLayer(int size, Board* newLayer, BoardTreeLayer* prevLayer);
        ~BoardTreeLayer();

        void constructNextLayers();
        void setWinningStates();
};