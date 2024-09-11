//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

/*
 * Take a sequence of command as input
 * The commands create, delete modify and display shapes to be drawn on the screen.
 * Each command consists of an operation keyword followed by arguments.
 *
 * 1. take input from the standard input,
 * 2. parse it,
 * 3. verify that it is correct,
 * 4. print a response or error message and either create,
 * 5. delete or modify Shape objects that represent the shapes specified in the command
 * 6. loops, processing input as long as input is available
 */

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here

// Main helpers
void maxShapes(int value);
void create(string name, string type, int loc_x, int loc_y, int size_x, int size_y);
void move(int i, int loc1, int loc2);
void rotate(int i, int angle);
void draw_shape(string name);
void delete_shape(string name);

// Others
int contains_command(string command);
void print_error(int type);
bool location_checker(stringstream& line, int& loc, bool endof);
bool angle_checker(stringstream& line, int& angle);
int type_checker(stringstream& line, string& type);
bool name_command(string name);
int name_exists(string name, int print);
bool name_checker(string name);

int main() {

    // Shape new_shape("name", "triangle", 1, 1, 2, 2);
    // cout << new_shape.getName();

    string line;
    string command;

    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input
    int num_com;
    string name, type;
    int loc_x, loc_y, size_x, size_y, angle, pos;

    while ( !cin.eof() ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);

        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;    // Read the command

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here

        num_com = contains_command(command);

        if (num_com == -1){
            print_error(0);
        } else if (num_com == 1){
            /*
             * maxShapes
             * 1. set variable num of max shapes
             * 2. set shape array
             * 3. report if there is too much (7), too less (8), invalid type (1), invalid value (6)
             */

            // invalid argument for maxShapes
            lineStream >> max_shapes;
            if (lineStream.fail()) {
                if (lineStream.eof ()){
                    print_error(8);     // no inputs
                } else{
                    print_error(1);     // wrong type
                }
            } else if (!lineStream.eof()) {
                print_error(7);         // *what if there's more than one variable in the line?
            } else if (max_shapes <= 0){
                print_error(6);
            } else {
                maxShapes(max_shapes);
            }
        } else if (num_com == 2){
            /*
             * create
             * 1. if the array is full (9)
             * 2. check name
             *  2a) invalid shape name (2): a reserved word (e.g., a command name or a shape type)
             *  2b) check if shape name exist (3)
             * 3. type: invalid shape type (5)
             * 4. report if there is too much (7), too less (8), invalid type (1), invalid value (6)
             * 5. create
             */

            if (shapeCount == max_shapes){
                print_error(9);
            } else{

                lineStream >> name;
                if (lineStream.fail()) {
                    if (lineStream.eof ()){
                        print_error(8);     // no inputs
                    } else{
                        print_error(1);     // wrong type
                    }
                } else if (name_checker(name) && type_checker(lineStream, type)
                           && location_checker(lineStream, loc_x, false) && location_checker(lineStream, loc_y, false)){
                    // now all the variable except two sizes are checked
                    int size;
                    bool valid = true;
                    for (int i = 0; i < 2; i++){

                        lineStream >> size;
                        if (lineStream.fail()) {
                            if (lineStream.eof ()){
                                print_error(8);     // no inputs
                            } else{
                                print_error(1);     // wrong type
                            }
                            valid = false;
                            break;
                        } else if (i == 1 && !lineStream.eof()) {
                            print_error(7);         // *what if there's more than one variable in the line?
                            valid = false;
                            break;
                        } else if (size < 0){
                            print_error(6);
                            valid = false;
                            break;
                        } else {
                            if (i == 0) { size_x = size; }
                            else { size_y = size; }
                        }

                    }
                    // now size is checked

                    if (valid && type == shapeTypesList[0] && (size_x != size_y)){
                        valid = false;
                        print_error(6);
                    }
                    // when type = circle, check size

                    if (valid){
                        create(name, type, loc_x, loc_y, size_x, size_y);
                    }

                }

            }

        } else if (num_com == 3){
            /*
             * move
             * 1. check name (if name exist) (4)
             * 2. check location (valid?): invalid type (1), invalid value (6)
             * 3. report if there is too much (7), too less (8)
             * 4. move/modify using function in shape
             */

            lineStream >> name;
            if (lineStream.fail()) {
                if (lineStream.eof ()){
                    print_error(8);     // no inputs
                } else{
                    print_error(1);     // wrong type
                }
            } else if (name_exists(name, 2) != -1
                       && location_checker(lineStream, loc_x, false) && location_checker(lineStream, loc_y, true)){
                // name matches / if not match, print error message and skip
                pos = name_exists(name, 3);
                move(pos, loc_x, loc_y);
            }
            // cout << name_exists(name, 2) << ": " << (name_exists(name, 2) != -1) << " & "
            // << location_checker(lineStream, loc_x, false) << " & " << location_checker(lineStream, loc_y, true);

        } else if (num_com == 4){
            /*
             * rotate
             * 1. check name (if name exist) (4)
             * 2. check angle (valid?): invalid type (1), invalid value (6)
             * 3. report if there is too much (7), too less (8)
             * 4. move/modify using function in shape
             */
            lineStream >> name;
            if (lineStream.fail()) {
                if (lineStream.eof ()){
                    print_error(8);     // no inputs
                } else{
                    print_error(1);     // wrong type
                }
            } else if (name_exists(name, 2) != -1 && angle_checker(lineStream, angle)){
                pos = name_exists(name, 3);
                rotate(pos, angle);
            }
        } else if (num_com == 5){
            /*
             * draw
             * 1. extract name; check name
             * 2. pass to draw method (all/specific)
             */

            lineStream >> name;
            if (lineStream.fail()) {
                if (lineStream.eof ()){
                    print_error(8);     // no inputs
                } else{
                    print_error(1);     // wrong type
                }
            } else{
                // if name not exist, print
                if (name_exists(name, 3) == -1 && name != keyWordsList[0]){
                    cout << "Error: shape " << name << " not found" << endl;
                }
                    // more input than expected
                else if (!lineStream.eof()) {
                    print_error(7);         // *what if there's more than one variable in the line?
                }
                    // then draw
                else{
                    draw_shape(name);
                }
            }

        } else if (num_com == 6){
            /*
             * delete
             * 1. extract name; check name
             * 2. pass to draw method (all/specific)
             */
            lineStream >> name;
            if (lineStream.fail()) {
                if (lineStream.eof ()){
                    print_error(8);     // no inputs
                } else{
                    print_error(1);     // wrong type
                }
            } else{
                // if name not exist, print
                if (name_exists(name, 3) == -1 && name != keyWordsList[0]){
                    cout << "Error: shape " << name << " not found" << endl;
                }
                    // more input than expected
                else if (!lineStream.eof()) {
                    print_error(7);         // *what if there's more than one variable in the line?
                }
                    // then draw
                else{
                    delete_shape(name);
                }

            }

        }

        // *The first line of input to your program will always be the maxShapes command

        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);

    }  // End input loop until EOF.

    return 0;
}

void maxShapes(int value){
    shapesArray = new Shape*[value];
    shapeCount = 0;

    for (int i = 0; i < value; i++){
        * shapesArray = NULL;
    }
    cout << "New database: max shapes is " << value << endl;
}

void create(string name, string type, int loc_x, int loc_y, int size_x, int size_y){
    shapesArray[shapeCount] = new Shape(name, type, loc_x, size_x, loc_y, size_y);
    cout << "Created ";
    shapesArray[shapeCount] -> draw();
    // cout << "shapeCount" << shapeCount;
    shapeCount++;
    cout << endl;
}

void move(int i, int loc1, int loc2){
    // i stored correct position
    shapesArray[i] -> setXlocation(loc1);
    shapesArray[i] -> setYlocation(loc2);
    cout << "Moved " << shapesArray[i] -> getName() << " to " << loc1 << " " << loc2 << endl;
}

void rotate(int i, int angle){
    // valid name and angle
    shapesArray[i] -> setRotate(angle);
    cout << "Rotated " << shapesArray[i] -> getName() << " by " << angle << "degrees" << endl;
}

void draw_shape(string name){
    if (name == keyWordsList[0]){
        cout << "Drew all shapes ";
        for (int i = 0; i < shapeCount; i++){
            if (shapesArray[i] == NULL)
                // cout << "Null shape" << i << endl;
                break;
            shapesArray[i] -> draw();
            cout << " ";
        }
        cout << endl;
    }
    else{
        cout << "Drew ";
        shapesArray[name_exists(name, 3)] -> draw();
        cout << endl;
    }
}

void delete_shape(string name){
    if (name == keyWordsList[0]){
        shapeCount = 0;
        for (int i = 0; i < shapeCount; i++){
            delete shapesArray[i];
            shapesArray[i] = NULL;
        }
        delete [] shapesArray;      // do we need to delete the array?
        shapeCount = 0;
        cout << "Deleted: all shapes" << endl;
    }
    else{
        int pos = name_exists(name, 3);
        delete shapesArray[pos];
        shapesArray[pos] = NULL;
        for (pos; pos < shapeCount - 1; pos++)
            shapesArray[pos] = shapesArray[pos+1];
        shapeCount--;
        cout << "Deleted shape " << name << endl;
    }
}

int contains_command(string command){
    for (int i = 0; i < NUM_KEYWORDS; i++){
        if (command == keyWordsList[i])
            return i;
    }
    return -1;      // returns 0 if command not found
}

void print_error(int type){
    cout << "Error: ";
    if (type == 0)
        cout << "invalid command";
    else if (type == 1)
        cout << "invalid argument";
    else if (type == 2)
        cout << "invalid shape name";
    else if (type == 3)
        cout << "shape name exists";
    else if (type == 4)
        cout << "shape name not found";
    else if (type == 5)
        cout << "invalid shape type";
    else if (type == 6)
        cout << "invalid value";
    else if (type == 7)
        cout << "too many arguments";
    else if (type == 8)
        cout << "too few arguments";
    else if (type == 9)
        cout << "shape array is full";
    cout << '\n';
}

bool location_checker(stringstream& line, int& loc, bool endof){
    line >> loc;
    if (line.fail()) {
        if (line.eof ()){
            print_error(8);     // no inputs
        } else{
            print_error(1);     // wrong type
        }
        return false;
    } else if (endof && !line.eof()) {
        // cout << endof << "end of " << endl;
        print_error(7);         // *what if there's more than one variable in the line?
        return false;
    } else if (loc < 0){
        // cout << "location error";
        print_error(6);     // invalid value
        return false;
    }
    return true;
}

bool angle_checker(stringstream& line, int& angle){
    line >> angle;
    if (line.fail()) {
        if (line.eof ()){
            print_error(8);     // no inputs
        } else{
            print_error(1);     // wrong type
        }
        return false;
    } else if (!line.eof()) {
        print_error(7);         // *what if there's more than one variable in the line?
        return false;
    } else if (angle < 0 || angle > 360){
        print_error(6);     // invalid value
        return false;
    }
    return true;
}

int type_checker(stringstream& line, string& type){
    line >> type;
    if (line.fail()) {
        if (line.eof ()){
            print_error(8);     // no inputs
        } else{
            print_error(1);     // wrong type
        }
        return false;
    } else{
        for (int i = 0; i < NUM_TYPES; i++){
            if (type == shapeTypesList[i])
                return true;
        }
    }
    print_error(5);
    return false;      // returns 0 if command not found
}

bool name_command(string name){
    // check if name match a reserved word
    for (int i = 0; i < NUM_TYPES; i++){
        if (name == shapeTypesList[i]){
            print_error(2);
            return true;
        }
    }
    for (int i = 0; i < NUM_KEYWORDS; i++){
        if (name == keyWordsList[i]){
            print_error(2);
            return true;
        }
    }
    return false;
}

int name_exists(string name, int print){
    // check if name already exists in the array
    for (int i = 0 ; i < shapeCount ; i++){
        if (shapesArray[i] -> getName() == name){
            if (print == 1)
                cout << "Error: shape " << name << " exists" << endl;
            return i;
        }
    }
    if (print == 2)
        cout << "Error: shape " << name << " not found" << endl;
    return -1;
}

bool name_checker(string name){
    if (name_command(name)){
        return false;
    } else if (name_exists(name, 1) != -1){
        // print_error(3);
        return false;
    }
    return true;
}

