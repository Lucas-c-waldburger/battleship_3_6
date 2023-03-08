#ifndef BATTLESHIP_LOGIC_H
#define BATTLESHIP_LOGIC_H

#include <iostream>
#include <vector>

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
std::ostream& operator<<(std::ostream& stream, const Ship& ship);

bool operator==(const Segment& shipSegment, const Segment& playerGuess);

// GENERAL PURPOSE FUNCTIONS //
int getRandomNum(int upperBound);

// SHIP PLACEMENT FUNCTIONS //
static std::vector<Ship>allEnemyPieces;

const int shipLengths[5] = {5, 4, 3, 3, 2};

bool decideHorizontal();

void plotSegments(Ship& ship, int startRow, int startColumn);

bool isPathClear(std::vector<Ship>& allPieces, Ship& currentShip, int startingRow, int startingColumn);

int getStartingPosition(int numberOfSegments);

void findLegalPlacement(std::vector<Ship>& allPieces, Ship& ship);

// PLAYER GUESS FUNCTIONS //
static int guessCounter = 0;

void increaseGuessCounter(int& guessCounter);

int convertAlpha(char alpha);

Segment getPlayerGuess();

int getHitShipIndex(std::vector<Ship>& allPieces, Segment& playerGuess);

// GAME STATE CHECK FUNCTIONS //
bool isWholeShipSunk(Ship& ship);

bool areAllShipsSunk(std::vector<Ship>& allPieces);

// REMOVE PIECES FUNCTIONS ///
void removeSegmentFromShip(Ship& ship, Segment& hitSegment);

void removeShipFromPieces(std::vector<Ship>& allPieces);

// VISUAL GRID //
static char visualGrid[7][7];

void printLogo();

void printShipDestroyed(int shipLength);

void clearGridVisual(char grid[gridLength][gridLength]);

void drawGridVisual(char grid[gridLength][gridLength], int& guessCounter);

void drawHitResult(char grid[gridLength][gridLength], Segment& playerGuess, char result);

// FUNCTIONS FOR TESTING/DEBUGGING //
static bool debugMode;

bool enterDebug();

void drawShipLocationsForTesting(char grid[gridLength][gridLength], std::vector<Ship>& allPieces, int& guessCounter);





#endif //BATTLESHIP_LOGIC_H
