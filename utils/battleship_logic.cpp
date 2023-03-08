#include "battleship_logic.h"
#include <iostream>


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

// GENERAL PURPOSE FUNCTIONS //
int getRandomNum(int upperBound) {
    return arc4random() % upperBound;
}

// SHIP PLACEMENT FUNCTIONS //
bool decideHorizontal() {
    return static_cast<bool>(getRandomNum(2));
}

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

bool isPathClear(std::vector<Ship>& allPieces, Ship& currentShip, int startingRow, int startingColumn) {
    if (currentShip.isHorizontal) {
        for (int i = startingColumn; i < currentShip.NumberOfSegments + startingColumn; i++) {
            for (Ship piece: allPieces) {
                for (Segment seg: piece.segmentLocations) {
//                    std::cout << "looking at - row: " << startingRow << ", column:" << i << '\n'
//                              << "current segment being churned through - row: " << seg.rowValue << ", column: " << seg.rowValue << '\n' << std::endl;
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
//                    std::cout << "looking at - row: " << startingRow << ", column:" << i << '\n'
//                              << "current segment being churned through - row: " << seg.rowValue << ", column: " << seg.rowValue << '\n' << std::endl;
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

//        std::cout << "Starting Row: " << startingRow << '\n'
//                  << "Starting Column: " << startingColumn << '\n'
//                  << "Is Horizontal" << std::boolalpha << ship.isHorizontal << '\n'
//                  << "Number of segments: " << ship.NumberOfSegments << '\n' << std::endl;

        if (isPathClear(allPieces, ship, startingRow, startingColumn)) {
            plotSegments(ship, startingRow, startingColumn);
        }
    }
}

// PLAYER GUESS FUNCTIONS //
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

// GAME STATE CHECK FUNCTIONS //
bool isWholeShipSunk(Ship& ship) {
    if (ship.segmentLocations.empty()) {
        return true;
    }
    return false;
}

bool areAllShipsSunk(std::vector<Ship>& allPieces) {
    if (allPieces.empty()) {
        return true;
    }
    return false;
}

// REMOVE PIECES FUNCTIONS ///
void removeSegmentFromShip(Ship& ship, Segment& hitSegment) {
    std::cout << hitSegment.rowValue << ", " << hitSegment.columnValue << '\n' << std::endl;
    std::vector<Segment>::iterator it;
    it = std::remove(ship.segmentLocations.begin(), ship.segmentLocations.end(), hitSegment);
    ship.segmentLocations.erase(it, ship.segmentLocations.end());
}

// VISUAL GRID //
void printLogo() {
    std::cout << R"(
 ____              __     __     ___                     __                        __        __
/\  _`\           /\ \__ /\ \__ /\_ \                   /\ \       __             /\ \      /\ \
\ \ \L\ \     __  \ \ ,_\\ \ ,_\\//\ \       __     ____\ \ \___  /\_\   _____    \_\ \___  \_\ \___
 \ \  _ <'  /'__`\ \ \ \/ \ \ \/  \ \ \    /'__`\  /',__\\ \  _ `\\/\ \ /\ '__`\ /\___  __\/\___  __\
  \ \ \L\ \/\ \L\.\_\ \ \_ \ \ \_  \_\ \_ /\  __/ /\__, `\\ \ \ \ \\ \ \\ \ \L\ \\/__/\ \_/\/__/\ \_/
   \ \____/\ \__/.\_\\ \__\ \ \__\ /\____\\ \____\\/\____/ \ \_\ \_\\ \_\\ \ ,__/    \ \_\     \ \_\
    \/___/  \/__/\/_/ \/__/  \/__/ \/____/ \/____/ \/___/   \/_/\/_/ \/_/ \ \ \/      \/_/      \/_/
                                                                           \ \_\
         Coded By: Lucas Waldburger                                         \/_/
    )" << '\n' << std::endl;
}

void printShipDestroyed(int shipLength) {
    std::cout << "   ~~~~~~~~~~~~~" << " Ship " << shipLength << " destroyed! "
              << "~~~~~~~~~~~~~" << std::endl;
}

void clearGridVisual(char grid[gridLength][gridLength]) {
    for (int row = 0; row < gridLength; row++)
        for (int col = 0; col < gridLength; col++)
            grid[row][col] = ' ';
}

void drawGridVisual(char grid[gridLength][gridLength], int& guessCounter) {
    std::cout << border << std::endl;
    for (int row = 0; row < gridLength; row++) {
        for (int col = 0; col < gridLength; col++) {
            if (col == 0) {
                std::cout << static_cast<char>(row + 65) << ' ';
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
//            std::cout << '(' << row << ", " << col << ')' << '\n' << std::endl;
            if (row == playerGuess.rowValue && col == playerGuess.columnValue &&
                grid[row][col] == ' ') {
                grid[row][col] = result;
                break;
            }
        }
    }
}

// FUNCTIONS FOR TESTING/DEBUGGING //
bool enterDebug() {
    char debugChoice;
    std::cout << "Enter debug mode? (Y/N) : ";
    std::cin >> debugChoice;
    std::cout << '\n' << std::endl;

    if (debugChoice == 'Y')
        return true;
    else
        return false;
}

void drawShipLocationsForTesting(char grid[gridLength][gridLength], std::vector<Ship>& allPieces, int& guessCounter) {
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


