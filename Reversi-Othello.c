#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*----------------------PROTOTYPES-------------------------------*/
void initializeBoardGame(char board[][26], int n);
void printBoard(char board[][26], int n);
bool checkLegalInDirection(char board[][26], int n, char row, char col,
        char colour, int deltaRow, int deltaCol);
bool positionInBounds(char board[][26], int n, char row, char col);
bool moveValidityCheck(char board[][26], int n, char row, char col, char colour);
void flipColours(char board[][26], int n, char row, char col, char colour);
void flipInDirection(char board[][26], char row, char col,
        char colour, int deltaRow, int deltaCol);
bool checkIfMovesAvailable(char board[][26], int n, char colour);
bool anyMovesLeft(char board[][26], int n);
bool playerTurn(char board[][26], int n, char colour);
void checkWinner(char board[][26], int n);

/*----------------------MAIN CODE--------------------------------*/
int main(void) {

    int n; //board size
    char colour;
    char row, col, deltaRow, deltaCol;
    char player1Colour, player2Colour;
    bool movesExist;
    bool gameContinues = true;

    //initializing board
    printf("Enter the board dimension: ");
    scanf("%d", &n);

    char board[n][26];

    //checking who plays what colour
    printf("Player 1 plays (B/W): ");
    scanf(" %c", &player1Colour);

    //setting player's colour
    if (player1Colour == 'B') {
        player2Colour = 'W';
        printf("Player 1 is %c and Player 2 is %c \n",
                player1Colour, player2Colour);
        printf("Player 1 (B) will go first \n");
    } else {
        player2Colour = 'B';
        printf("Player 1 is %c and Player 2 is %c \n",
                player1Colour, player2Colour);
        printf("Player 2 (B) will go first \n");
    };

    //first printout of board state
    initializeBoardGame(board, n); //creates board with BW in middle
    printBoard(board, n); //prints the board out

    //game begins here ---------------------------

    //bools that will be used for the turn based part of the game
    bool player1MovesExist, player2MovesExist;
    bool validPlayerMove = true;

    //check if player 2 move first. If so, player 2 gets a turn first.
    //Otherwise, this part is skipped and goes straight to player 1

    if (player2Colour == 'B') {
        colour = player2Colour;
        validPlayerMove = playerTurn(board, n, colour);
        if (validPlayerMove == true) {
            printBoard(board, n);
        } else {
            gameContinues = false;
            validPlayerMove = false;
        }
    }

    //turn based part begins here-----------------

    //only if there are available moves AND the player has made a valid move
    while ((gameContinues == true)&&(validPlayerMove == true)) {

        //player1 turn-------------------
        colour = player1Colour; //switches color
        player1MovesExist = checkIfMovesAvailable(board, n, colour);
        //only make move if there are moves available
        if (player1MovesExist == true) {
            //check if player makes a legitimate move
            validPlayerMove = playerTurn(board, n, colour);
            if (validPlayerMove == true) printBoard(board, n);
        } else {
            //recheck if game can keep going 
            gameContinues = anyMovesLeft(board, n);
            if (gameContinues == true) {
                //only print there's no valid moves if game isn't over
                printf("Player 1 (%c) has no valid move.\n", colour);
            }
        }
        //exits while loop if player messed up
        if (validPlayerMove == false) break;

        //player2 turn---------------
        colour = player2Colour; //switches colour
        player2MovesExist = checkIfMovesAvailable(board, n, colour);
        //only make moves if there are available moves
        if (player2MovesExist == true) {
            //check if player makes a legitimate move
            validPlayerMove = playerTurn(board, n, colour);
            if (validPlayerMove == true) printBoard(board, n);
        } else {
            //recheck if game can keep going 
            gameContinues = anyMovesLeft(board, n);
            if (gameContinues == true) {
                //only print there's no valid moves if game isn't over
                printf("Player 2 (%c) has no valid move.\n", colour);
            }
        }
        //exits while loop if player messed up
        if (validPlayerMove == false) break;

        //check if there's any moves left again
        gameContinues = anyMovesLeft(board, n);
    }

    //end screen---------------------
    if (gameContinues != true) {
        checkWinner(board, n);
    }

    if (validPlayerMove == false) {
        printf("Invalid move.\n");

        if (colour == player1Colour) {
            printf("Player 2 wins");
        } else {
            printf("Player 1 wins");
        }
    }

    return (EXIT_SUCCESS);
}

/*-------------------------FUNCTIONS----------------------------*/

void initializeBoardGame(char board[][26], int n) {

    //setting everything to U first
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    //setting middle pieces to black and white
    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2 - 1][n / 2] = 'B';
    board[n / 2][n / 2 - 1] = 'B';
    board[n / 2][n / 2] = 'W';

    return;
}

void printBoard(char board[][26], int n) {

    //prints top row 
    printf("   ");
    for (int i = 0; i < n; i++) {
        printf("%c", i + 'a');
    }
    printf("\n");

    //prints rest of board from left to right, up to down
    for (int i = 0; i < n; i++) {
        printf("%c  ", i + 'a'); //prints first letter and spaces behind it
        for (int j = 0; j < n; j++) {
            printf("%c", board[i][j]); //prints char of space
        }
        printf("\n");
    }
    return;
}

bool moveValidityCheck(char board[][26], int n, char row, char col, char colour) {
    //looks at all 8 directions
    bool valid = false;
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {

            if ((deltaRow == 0)&&(deltaCol == 0)) {

            } else {
                bool legal = checkLegalInDirection(board, n, row, col,
                        colour, deltaRow, deltaCol);
                if (valid == false) {
                    if (legal == true) {
                        valid = true;
                    }
                }
            }

        }
    }
    return (valid);
}

bool checkLegalInDirection(char board[][26], int n, char row, char col,
        char colour, int deltaRow, int deltaCol) {

    //actually checks each of the directions
    bool legalMove = false; //legal move is false unless meets all criteria

    char oppositeColour = 'B';
    //figures out opposite colour
    if (colour == 'B') oppositeColour = 'W';

    row = row + deltaRow; //shift position
    col = col + deltaCol;

    //check if moving there is allowed
    bool possibleMove = positionInBounds(board, n, row, col);
    if (possibleMove == true) {
        //if new position is of opposite colour
        if (board[row - 'a'][col - 'a'] == oppositeColour) {
            while (possibleMove == true) {
                //keep shifting in that direction
                row = row + deltaRow;
                col = col + deltaCol;
                //make sure new board space isn't empty
                if (board[row - 'a'][col - 'a'] == 'U') {
                    possibleMove = false;
                } else {
                    possibleMove = positionInBounds(board, n, row, col);
                    if (possibleMove == true) {
                        //set legal move to true if you end on original colour
                        if (board[row - 'a'][col - 'a'] == colour) {
                            legalMove = true;
                        }
                    }
                }
            }
        }
    }
    return (legalMove);
}

bool positionInBounds(char board[][26], int n, char row, char col) {
    bool possibleLocation = true;
    //the position is allowed unless proven to be out of bounds
    if ((row < 'a') || (row >= 'a' + n) || (col < 'a') || (col >= 'a' + n)) {
        possibleLocation = false;
    }
    return (possibleLocation);
}

void flipColours(char board[][26], int n, char row, char col, char colour) {
    //first set current position equal to the color 
    board[row - 'a'][col - 'a'] = colour;
    bool moveThatDirection = false;
    //if legal on that side, change that side's colour  
    //go through all 8 directions again
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if ((deltaRow == 0)&&(deltaCol == 0)) {
            } else {
                moveThatDirection = checkLegalInDirection(board, n, row, col,
                        colour, deltaRow, deltaCol);
            }
            if (moveThatDirection == true) {
                //initiate actual color flipping part(
                flipInDirection(board, row, col, colour, deltaRow, deltaCol);
            }
        }
    }
    return;
}

void flipInDirection(char board[][26], char row, char col,
        char colour, int deltaRow, int deltaCol) {

    char oppositeColour = 'B';
    //figure out opposite colour
    if (colour == 'B') oppositeColour = 'W';

    row = row + deltaRow; //shift position
    col = col + deltaCol;
    //keep shifting position and flipping colour until reach original colour
    do {
        board[row - 'a'][col - 'a'] = colour;
        row = row + deltaRow; //shift position
        col = col + deltaCol;
    } while (board[row - 'a'][col - 'a'] == oppositeColour);
    return;
}

bool checkIfMovesAvailable(char board[][26], int n, char colour) {
    char row, col;
    bool movesAvailable = false;
    for (int i = 0; i < n; i++) { //sift through rows
        for (int j = 0; j < n; j++) { //sift through columns

            if (board[i][j] == 'U') { //only check if space is empty
                row = 'a' + i; //set row and col numbers back to letters
                col = 'a' + j;

                //considers move only if it is valid
                bool potentialMove = moveValidityCheck(board, n, row, col, colour);
                if (potentialMove == true) {
                    movesAvailable = true; //if there is even one instance of an
                    //available move then movesAvailable is true
                }
            }
        }
    }
    return (movesAvailable);
}

bool anyMovesLeft(char board[][26], int n) {
    bool movesLeft = false;
    char colour = 'W';
    movesLeft = checkIfMovesAvailable(board, n, colour);
    if (movesLeft == false) {
        colour = 'B';
        movesLeft = checkIfMovesAvailable(board, n, colour);
    }
    return (movesLeft);
}

bool playerTurn(char board[][26], int n, char colour) {

    char selectedRow, selectedCol;
    bool legitMove;

    printf("Enter move for colour %c (RowCol): ", colour);
    scanf(" %c%c", &selectedRow, &selectedCol);

    legitMove = moveValidityCheck(board, n, selectedRow, selectedCol, colour);

    if (legitMove == true) {
        flipColours(board, n, selectedRow, selectedCol, colour);
    }
    return (legitMove);
}

void checkWinner(char board[][26], int n) {

    int blackSquares = 0, whiteSquares = 0;

    for (int i = 0; i < n; i++) { //sift through rows
        for (int j = 0; j < n; j++) { //sift through columns

            //adds scores to black or white each move
            if (board[i][j] == 'B') {
                blackSquares++;
            } else if (board[i][j] == 'W') {
                whiteSquares++;
            }
        }
    }

    //prints message based on who won
    if (blackSquares > whiteSquares) {
        printf("B player wins.");
    } else if (whiteSquares > blackSquares) {
        printf("W player wins.");
    } else if (whiteSquares == blackSquares) {
        printf("Draw!");
    }

    return;
}