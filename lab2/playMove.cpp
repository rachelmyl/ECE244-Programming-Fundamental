//
//  playMove.cpp
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
//  ECE244 Student: write your code for playMove in this file

#include "GameState.h"
#include "globals.h"
#include <iostream>
using namespace std;

// Function prototypes (if you need)
int checkWinner(GameState& game_state);

void playMove(GameState& game_state) {

    if (game_state.get_turn()){
        game_state.set_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn(), R);
    } else{
        game_state.set_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn(), Y);
    }

    game_state.set_moveValid(false);

    // Change turn if move is valid,
    game_state.set_turn(!game_state.get_turn());

    // Change gameOver
    // Change winner
    game_state.set_winner(checkWinner(game_state));
    // cout << "winner in play move" <<  game_state.get_winner() << '\n';
    // cout << "gameover? in play move" <<  game_state.get_gameOver() << '\n';

}

int checkWinner(GameState& game_state){

    // vertical
    for (int row = 0; row < boardSize; row++){
        for (int col = 0; col < boardSize - 3; col++){
            if (game_state.get_gameBoard(row, col) != Empty &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row, col + 1) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row, col + 2) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row, col + 3)) {

                game_state.set_gameOver(true);
                return game_state.get_gameBoard(row, col);
            }
        }
    }

    // horizontal
    for (int col = 0; col < boardSize; col++){
        for (int row = 0; row < boardSize - 3; row++){
            if (game_state.get_gameBoard(row, col) != Empty &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 1, col) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 2, col) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 3, col)) {

                game_state.set_gameOver(true);
                return game_state.get_gameBoard(row, col);
            }
        }
    }

    // diagonal \ (negative slope)
    for (int row = 3; row < boardSize; row++){
        for (int col = 0; col < boardSize - 3; col++){
            if (game_state.get_gameBoard(row, col) != Empty &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row - 1, col + 1) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row - 2, col + 2) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row - 3, col + 3)) {

                game_state.set_gameOver(true);
                return game_state.get_gameBoard(row, col);
            }
        }
    }

    // diagonal / (positive slope)
    for (int col = 0; col < boardSize - 3; col++){
        for (int row = 0; row < boardSize - 3; row++){
            if (game_state.get_gameBoard(row, col) != Empty &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 1, col + 1) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 2, col + 2) &&
                game_state.get_gameBoard(row, col) == game_state.get_gameBoard(row + 3, col + 3)) {

                game_state.set_gameOver(true);
                return game_state.get_gameBoard(row, col);
            }
        }
    }

    // check if the board is full
    for (int row = 0; row < boardSize; row++){
        for (int col = 0; col < boardSize; col++){
            if (game_state.get_gameBoard(row, col) == Empty) {
                return Empty;
            }
        }
    }

    game_state.set_gameOver(true);
    return Empty;
}