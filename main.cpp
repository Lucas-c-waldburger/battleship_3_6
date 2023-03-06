#include <iostream>
#include <vector>
//#include "utils/pieces.h"




// STRUCTS //

struct Segment {
    int rowValue;
    int columnValue;
};

struct Ship {
    int NumberOfSegments;
    bool isHorizontal;
    std::vector<Segment> segmentLocations;

//    bool operator==(const Ship& otherShip) const {
//        if (NumberOfSegments == otherShip.NumberOfSegments) {
//            return true;
//        }
//        return false;
//    }
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

//EnemyPieces& createShips(int shipSizes[5]) {
//
//}

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

    Segment guessAsSegment = {convertAlpha(rowGuess), columnGuess};
    return guessAsSegment;
}



Ship * getHitShip(std::vector<Ship>& allPieces, Segment& playerGuess) {
    for (Ship& ship: allPieces) {
        for (Segment& seg: ship.segmentLocations) {
            if (playerGuess == seg) {
                std::cout << "Address of ship found within getHitShip" << &ship;
                return &ship;
            }
        }
    }
    return nullptr;
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

bool isSegmentSunk(std::vector<Ship>& allPieces) {
    for (const Ship& ship: allPieces) {
        for (Segment seg: ship.segmentLocations) {
            if (seg.isSunk) {
                return true;
            }
        }
    }
    return false;
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
    std::cout << "inside remove segment function: " << &ship << std::endl;
}

void removeShipFromPieces(std::vector<Ship>& allPieces) {
    std::vector<Ship>::iterator it;
    it = std::remove_if(allPieces.begin(), allPieces.end(), isWholeShipSunk);
    allPieces.erase(it, allPieces.end());
}


int main() {

    std::vector<Ship>allEnemyPieces;

    // PUT ALL SHIPS IN IMPLEMENTATION FILE //
    allEnemyPieces.push_back({5, true, false,{
            {0, 2, false},
            {0, 3, false},
            {0, 4, false},
            {0, 5, false},
            {0, 6, false}}});


    while (!allEnemyPieces.empty()) {
        Segment playerGuess = getPlayerGuess();

        std::cout << "Player Guess: (" << playerGuess.rowValue << ", " << playerGuess.columnValue << ')' << std::endl;

        int hitShipIndex = getHitShipIndex(allEnemyPieces, playerGuess);

        if (hitShipIndex != -1) {
            removeSegmentFromShip(allEnemyPieces[hitShipIndex], playerGuess);
        }

        for (Ship &ship: allEnemyPieces) {
            std::cout << ship;
        }
    }

    return 0;
}