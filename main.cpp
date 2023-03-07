#include <iostream>
#include <vector>
//#include "utils/pieces.h"



//////////////////////////SHIP LOGIC//////////////////////////

// CONSTS //
const int gridLength = 7;
const std::string border = "  --1------2------3------4------5------6------7--";


// STRUCTS //
struct Segment {
    int rowValue;
    int columnValue;
};

struct Ship {
    int NumberOfSegments;
    bool isHorizontal;
    std::vector<Segment> segmentLocations;
};

// OPERATOR OVERLOADS //
std::ostream& operator<<(std::ostream& stream, const Ship& ship) {
    for (Segment seg: ship.segmentLocations)
        stream << "(row: " << seg.rowValue << ", column: " << seg.columnValue << "), ";
    return stream;
}

bool operator==(const Segment& shipSegment, const Segment& playerGuess) {
    if (playerGuess.rowValue == shipSegment.rowValue &&
        playerGuess.columnValue == shipSegment.columnValue) {
        return true;
    }
    return false;
}

// FUNCTIONS //

int getRandomNum(int upperBound) {
    return arc4random() % upperBound;
}

//initial ship placement
void plotSegments(Ship& ship, int startRow, int startColumn) {
    for (int i = 0; i < ship.NumberOfSegments; i++) {
        if (ship.isHorizontal)
            ship.segmentLocations.push_back(
                    Segment{startRow, startColumn + i}
            );
        else
            ship.segmentLocations.push_back(
                    Segment{startRow + i, startColumn}
            );
    }
}

int modifyEmptySpaces(int row, int column, Segment& seg, int adjacentEmptySpaces) {
    if (row == seg.rowValue && column == seg.columnValue)
        adjacentEmptySpaces = 0;
    else
        adjacentEmptySpaces += 1;
    return adjacentEmptySpaces;
}

//void findLegalPlacement(std::vector<Ship>& allPieces, Ship& ship) {
//    int adjacentEmptySpaces = 0;
//    int rowStart = 0;
//    int columnStart = 0;
//    int randomNum = getRandomNum(gridLength - (ship.NumberOfSegments - 1));
//
//    if (ship.isHorizontal)
//        rowStart = randomNum;
//    else
//        columnStart = randomNum;
//
//    std::cout << "Starting Row: " << rowStart << '\n'
//              << "Starting Column: " << columnStart << '\n'
//              << "Number of segment locations: " << ship.segmentLocations.size() << std::endl;
//
//    while (ship.segmentLocations.size() < ship.NumberOfSegments) {
//        for (int row = rowStart; row < gridLength; row++) {
//            for (int col = columnStart; col < gridLength; col++) {
//                for (Ship piece: allPieces) {
//                    for (Segment seg: piece.segmentLocations) {
//                        if (row == seg.rowValue && col == seg.columnValue) {
//                            adjacentEmptySpaces = 0;
//                            std::cout << "re-do!!";
//                            break;
//                        }
//                    }
//                }
//                adjacentEmptySpaces += 1;
//                std::cout << "Adjacent Empty Spaces Counter: " << adjacentEmptySpaces << std::endl;
//
//                if (adjacentEmptySpaces >= ship.NumberOfSegments) {
//                    if (ship.isHorizontal) {
//                        plotSegments(ship, row, col - (ship.NumberOfSegments - 1));
//                    }
//                    else {
//                        plotSegments(ship, col - (ship.NumberOfSegments - 1), col);
//                    }
//                    std::cout << ship << '\n' << std::endl;
//                    return;
//                }
//            }
//        }
//    }
//}




bool isPathClear(std::vector<Ship>& allPieces, Ship& currentShip, int startingRow, int startingColumn) {
    if (currentShip.isHorizontal) {
        for (int i = startingColumn; i < currentShip.NumberOfSegments + startingColumn; i++) {
            for (Ship piece: allPieces) {
                for (Segment seg: piece.segmentLocations) {
                    std::cout << "looking at - row: " << startingRow << ", column:" << i << '\n'
                              << "current segment being churned through - row: " << seg.rowValue << ", column: " << seg.rowValue << '\n' << std::endl;
                    if (startingRow == seg.rowValue && i == seg.columnValue)
                        return false;
                }
            }
        }
    }
    else {
        for (int i = startingRow; i < currentShip.NumberOfSegments + startingRow; i++) {
            for (Ship piece: allPieces) {
                for (Segment seg: piece.segmentLocations) {
                    std::cout << "looking at - row: " << startingRow << ", column:" << i << '\n'
                              << "current segment being churned through - row: " << seg.rowValue << ", column: " << seg.rowValue << '\n' << std::endl;
                    if (i == seg.rowValue && startingColumn == seg.columnValue)
                        return false;
                }
            }
        }
    }
    return true;
}

int getStartingPosition(int numberOfSegments) {
    return getRandomNum((gridLength - numberOfSegments));
}

void findLegalPlacement(std::vector<Ship>& allPieces, Ship& ship) {
    int startingRow;
    int startingColumn;

    while (ship.segmentLocations.empty()) {
        if (ship.isHorizontal) {
            startingRow = getStartingPosition(0);
            startingColumn = getStartingPosition(ship.NumberOfSegments);
        } else {
            startingRow = getStartingPosition(ship.NumberOfSegments);
            startingColumn = getStartingPosition(0);
        }

        std::cout << "Starting Row: " << startingRow << '\n'
                  << "Starting Column: " << startingColumn << '\n'
                  << "Is Horizontal" << std::boolalpha << ship.isHorizontal << '\n'
                  << "Number of segments: " << ship.NumberOfSegments << '\n' << std::endl;

        if (isPathClear(allPieces, ship, startingRow, startingColumn)) {
            plotSegments(ship, startingRow, startingColumn);
        }
    }
}




//player guess
void increaseGuessCounter(int& guessCounter) {
    guessCounter += 1;
}

int convertAlpha(char alpha) {
    if (islower(alpha))
        return alpha - 97;
    return alpha - 65;
}

Segment getPlayerGuess() {
    char rowGuess;
    int columnGuess;

    std::cout << "Enter your guess: ";
    std::cin >> rowGuess >> columnGuess;

    Segment guessAsSegment = {convertAlpha(rowGuess), columnGuess - 1};
    return guessAsSegment;
}

int getHitShipIndex(std::vector<Ship>& allPieces, Segment& playerGuess) {
    for (int i = 0; i < allPieces.size(); i++) {
        for (Segment& seg: allPieces[i].segmentLocations) {
            if (playerGuess == seg) {
                return i;
            }
        }
    }
    return -1;
}

// run through each segment of one ship
bool isWholeShipSunk(Ship& ship) {
    if (ship.segmentLocations.empty()) {
        return true;
    }
    return false;
}

// run through each ship of all enemy pieces to see if won
bool areAllShipsSunk(std::vector<Ship>& allPieces) {
    if (allPieces.empty()) {
        return true;
    }
    return false;
}

void removeSegmentFromShip(Ship& ship, Segment& hitSegment) {
    std::vector<Segment>::iterator it;
    it = std::remove(ship.segmentLocations.begin(), ship.segmentLocations.end(), hitSegment);
    ship.segmentLocations.erase(it, ship.segmentLocations.end());
}

void removeShipFromPieces(std::vector<Ship>& allPieces) {
    std::vector<Ship>::iterator it;
    it = std::remove_if(allPieces.begin(), allPieces.end(), isWholeShipSunk);
    allPieces.erase(it, allPieces.end());
}

////////////////////////// VISUAL GRID //////////////////////////
//Printing
void clearGridVisual(char grid[gridLength][gridLength]) {
    for (int row = 0; row < gridLength; row++)
        for (int col = 0; col < gridLength; col++)
            grid[row][col] = ' ';
}

//void printGridVisual(char grid[gridLength][gridLength], int& guessCounter) {
//    std::cout << border << std::endl;
//    for (int row = 0; row < gridLength; row++) {
//        for (int col = 0; col < gridLength; col++) {
//            if (col == 0) {
//                std::cout << static_cast<char>(row + 65) << ' ';
//            }
//            std::cout << "[ " << grid[row][col] << " ]  ";
//            if (col == 6) {
//                std::cout << '\n';
//                if (row != 6)
//                    std::cout << '\n';
//            }
//        }
//    }
//    std::cout << border << '\n'
//              << "\t\t\t\t" << "Guess Counter: " << guessCounter << '\n' << std::endl;
//}

void printGridVisual(char grid[gridLength][gridLength], std::vector<Ship>& allPieces, int& guessCounter) {
    std::cout << border << std::endl;
    for (int row = 0; row < gridLength; row++) {
        for (int col = 0; col < gridLength; col++) {
            if (col == 0) {
                std::cout << static_cast<char>(row + 65) << ' ';
            }
            for (Ship ship: allPieces) {
                for (Segment seg: ship.segmentLocations) {
                    if (row == seg.rowValue && col == seg.columnValue) {
                        grid[row][col] = 'a' + ship.NumberOfSegments;
                        break;
                    }
                }
            }
            std::cout << "[ " << grid[row][col] << " ]  ";
            if (col == 6) {
                std::cout << '\n';
                if (row != 6)
                    std::cout << '\n';
            }
        }
    }
    std::cout << border << '\n'
              << "\t\t\t\t" << "Guess Counter: " << guessCounter << '\n' << std::endl;
}

void drawHitResult(char grid[gridLength][gridLength], Segment& playerGuess, char result) {
    for (int row = 0; row < gridLength; row++) {
        for (int col = 0; col < gridLength; col++) {
            std::cout << '(' << row << ", " << col << ')' << '\n' << std::endl;
            if (row == playerGuess.rowValue && col == playerGuess.columnValue &&
                grid[row][col] == ' ') {
                grid[row][col] = result;
                break;
            }
        }
    }
}


int main() {

    std::vector<Ship>allEnemyPieces;
    int guessCounter = 0;

    // PUT ALL SHIPS IN IMPLEMENTATION FILE //
    allEnemyPieces.push_back({5, true});
    allEnemyPieces.push_back({4, false});
    allEnemyPieces.push_back({3, true});
    allEnemyPieces.push_back({3, false});
    allEnemyPieces.push_back({2, true});

    for (auto& ship : allEnemyPieces) {
        findLegalPlacement(allEnemyPieces, ship);
    }

    for (auto& ship: allEnemyPieces) {
        std::cout << "Ship" << ship.NumberOfSegments << " - " << ship << std::endl;
    }

    char visualGrid[7][7];
    clearGridVisual(visualGrid);

    while (!allEnemyPieces.empty()) {

        printGridVisual(visualGrid, allEnemyPieces, guessCounter);

        Segment playerGuess = getPlayerGuess();

        increaseGuessCounter(guessCounter);

        std::cout << "Player Guess: (" << playerGuess.rowValue << ", " << playerGuess.columnValue << ')' << std::endl;

        int hitShipIndex = getHitShipIndex(allEnemyPieces, playerGuess);

        std::cout << "Hit Ship Index Position: " << hitShipIndex << '\n' << std::endl;

        if (hitShipIndex != -1) {
            removeSegmentFromShip(allEnemyPieces[hitShipIndex], playerGuess);
            removeShipFromPieces(allEnemyPieces);

            drawHitResult(visualGrid, playerGuess, 'O');
        }
        else {
            drawHitResult(visualGrid, playerGuess, 'X');
        }


        for (Ship &ship: allEnemyPieces) {
            std::cout << ship << std::endl;
        }
    }

    return 0;
}