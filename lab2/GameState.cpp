//
//  GameState.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//

#include "GameState.h"
#include "globals.h"

// ECE244 Student: add you code below

GameState::GameState(){
    // The constructor. It initializes:
    selectedRow = 0;    //     selectedRow to 0
    selectedColumn = 0;  //     selectedColum to 0
    moveValid = true;    //     moveValid to true
    gameOver = false;   //     gameOver to false
    turn = true;     //     turn to true
    winner = Empty;   //     winner to Empty
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            gameBoard[i][j] = Empty; //     gameBoard locations to Empty (see globals.h)
        }
    }
}

// Return the selected row (in the range 0..boardSize - 1)
int GameState::get_selectedRow(){
    return selectedRow;
}

// Return the selected column (in the range 0..boardSize - 1)
int GameState::get_selectedColumn(){
    return selectedColumn;
}

// Set the selected row to value in the range (in the range 0..boardSize - 1)
// An out of range value is ignored and the function just returns
void GameState::set_selectedRow(int value){
    if (value < 0 || value >= boardSize){
        return;
    }
    selectedRow = value;
}

// Set the selected column in the range (in the range 0..boardSize - 1)
// An out of range value is ignored and the function just returns
void GameState::set_selectedColumn(int value){
    if (value < 0 || value >= boardSize){
        return;
    }
    selectedColumn = value;
}

// Get the moveValid value
bool GameState::get_moveValid(){
    return moveValid;
}

// Set the moveValid variable to value
void GameState::set_moveValid(bool value){
    moveValid = value;
}

// Get the gameOver value
bool GameState::get_gameOver(){
    return gameOver;
}

// Set the gameOver variable to value
void GameState::set_gameOver(bool value){
    gameOver = value;
}

// Get the value of turn
bool GameState::get_turn(){
    return turn;
}

// Set the value of turn
void GameState::set_turn(bool value){
    turn = value;
}

// Get the value of winner
int GameState::get_winner(){
    return winner;
}

// Set the value of winner;
void GameState::set_winner(int value){
    winner = value;
}

// Get the game board value at the board location at row and col
// This method checks that row, col and value are in range/valid
// and if not it returns Empty
int GameState::get_gameBoard(int row, int col){
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize){
        return gameBoard[row][col];
    } else{
        return Empty;
    }
}

// Set the game board value at the board location at row and col to value
// This method checks that row, col and value are in range/valid and if not it
// just returns
void GameState::set_gameBoard(int row, int col, int value){
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize){
        gameBoard[row][col] = value;
    } else{
        return;
    }
}
