#include <iostream>
#include <stdlib.h>
#include <vector>
#include "utils/battleship_logic.h"


int main() {

    debugMode = enterDebug();

    for (int length: shipLengths) {
        allEnemyPieces.push_back({length, decideHorizontal()});
    }

    for (auto& ship : allEnemyPieces) {
        findLegalPlacement(allEnemyPieces, ship);
    }

    printLogo();

    clearGridVisual(visualGrid);

    while (!allEnemyPieces.empty()) {

        drawGridVisual(visualGrid, guessCounter);

        if (debugMode) {
//            drawShipLocationsForTesting(visualGrid, allEnemyPieces, guessCounter);
            for (auto &ship: allEnemyPieces) {
                std::cout << "Ship" << ship.NumberOfSegments << " - " << ship << std::endl;
            }
        }

        Segment playerGuess = getPlayerGuess();

        increaseGuessCounter(guessCounter);

        int hitShipIndex = getHitShipIndex(allEnemyPieces, playerGuess);

        if (debugMode) {
            std::cout << "Player Guess: (" << playerGuess.rowValue << ", " << playerGuess.columnValue << ')'
                      << std::endl;
            std::cout << "Hit Ship Index Position: " << hitShipIndex << '\n' << std::endl;
        }

        if (hitShipIndex != -1) {
            Ship* hitShip = &allEnemyPieces[hitShipIndex];

            removeSegmentFromShip(*hitShip, playerGuess);

            if ((*hitShip).segmentLocations.empty()) {
                printShipDestroyed((*hitShip).NumberOfSegments);
                allEnemyPieces.erase(allEnemyPieces.begin() + hitShipIndex);
            }

            drawHitResult(visualGrid, playerGuess, 'O');
        }
        else {
            drawHitResult(visualGrid, playerGuess, 'X');
        }

        if (debugMode) {
            for (Ship &ship: allEnemyPieces) {
                std::cout << ship << std::endl;
            }
        }
    }

    std::cout << "YOU WIN!" << '\n'
              << "Final guess count: " << guessCounter << std::endl;

    return 0;
}