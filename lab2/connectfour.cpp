//
//  connectfour.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iostream>
#include <string>
using namespace std;

#include "GameState.h"
#include "globals.h"

void playMove(GameState& game_state);   // implement in playmMove.cpp
bool validCol(int& col, int& row, GameState& state);
void printBoard(GameState game_state);

// Function prototypes

// The main function
int main() {
    /**********************************************************************************/
    /* Create three initialized game state objects in an array */
    /**********************************************************************************/
    const int numOfRounds = 3;
    GameState game_state[numOfRounds];

    // Read one integer from the user that represents the column
    // the player would like to place their piece (R or Y) in
    // You can assume there will be no formatting errors in the input

    int col;
    int row;
    int score[2] = {0};     // red/yellow score

    for (int round = 0; round < numOfRounds; round++){

        cout << "Game " << round + 1 << '\n';

        while (!game_state[round].get_gameOver()) {
            cout << "Enter column to place piece: ";
            cin >> col;

            if( cin.eof() ) {
                cerr << endl << "Game ended by user." << endl ;
                exit( 0 );
            }
            if( cin.fail() ) {
                cin.clear();
                cin.ignore( 1000, '\n' );
                col = -1; // giving col invalid value so it will be handled as invalid input below
            }

            // Check validity of input and if not valid, handle accordingly

            // check whether col is out of bound
            // check whether according row is full

            while (!validCol(col, row, game_state[round])){
                cout << "Invalid column!" << '\n' << "Enter column to place piece: ";
                cin >> col;
            }
            cout << "column chosen: " << col << '\n';
            // cout<< "row" << row <<'\n';

            // The coordinates are valid; set the selectedRow and selectedColumn
            // members of the game state to the read values
            // Note that the corresponding mutators of GameState must be first
            // implemented before this works

            game_state[round].set_selectedRow(row);
            game_state[round].set_selectedColumn(col);

            // Call playMove

            playMove(game_state[round]);

            // Print the GameState object, as prescribed in the handout

            printBoard(game_state[round]);
            // cout << "winner in main" <<  game_state[round].get_winner() << '\n';
            // cout << "gameover? in main" <<  game_state[round].get_gameOver() << '\n';

            }

        // Check if a player won this round and if so handle accordingly

        if (game_state[round].get_winner() == R){
            cout << '\n' << "R won this round!" << '\n';
        } else if (game_state[round].get_winner() == Y){
            cout << '\n' << "Y won this round!" << '\n';
        } else{
            cout << '\n' << "It's a tie this round!" << '\n';
        }


        // add score
        if (game_state[round].get_winner() == R){
            score[0]++;
        } else if (game_state[round].get_winner() == Y){
            score[1]++;
        }

        // check if someone win the match
        if (score[0] >= 2){
            cout << "R won the match!";
            break;
        } else if (score[1] >= 2){
            cout << "Y won the match!";
            break;
        } else if (round == 2){
            cout << "It's a tie for this match!";
            break;
        }

    }

}

bool validCol(int& col, int& row, GameState& game_state){

    // check whether col is out of bound
    // check whether according row is full
    if (col < 0 | col >= boardSize){
        row = -1;
        // cout << "invalid col " << row << '\n';
        game_state.set_moveValid(false);
        return false;
    }

    for (row = 0; row < boardSize; row++){
        if (game_state.get_gameBoard(row, col) == Empty){
            // cout << "gameboard" << game_state.get_gameBoard(row, col) << '\n';
            break;
        }
    }

    if (row >= boardSize) {
        game_state.set_moveValid(false);
        return false;
    } else{
        game_state.set_moveValid(true);
        // cout << row;
        return true;
    }
}

void printBoard(GameState game_state){
    for (int row = boardSize - 1; row >= 0; row--){
        for (int col = 0; col < boardSize; col++){
            if (game_state.get_gameBoard(row, col) == Empty){
                cout << '_';
            } else if (game_state.get_gameBoard(row, col) == R){
                cout << 'R';
            } else{
                cout << 'Y';
            }
        }
        if (row != boardSize -1){
            cout << '\n';
        }
    }
}


  
