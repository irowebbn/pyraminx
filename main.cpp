#include <string>
#include <iostream>
#include "Pyraminx.h"

void input_to_turn(Pyraminx& pyraminx, std::string turn);

int main(){
    Pyraminx myPyraminx;
    std::string input;
    bool quit = false;
    printf("Enter a command of the form LX[`] where L is the letter corresponding with a \n"
           "corner of the Pyraminx (U, L, R, B),  X is a integer from 0-3 specifying the \n"
           "layer of the row relative to the tip (layer 0) at the corner given, and the \n"
           "optional ` mark indicates whether the layer should be turned counterclockwise.\n"
           "The default rotation is clockwise. The corner characters are case insensitive.\n"
           "There should be no spaces in your command.\n"
           "Enter 's' to scramble, 'q' to quit.\n");
    myPyraminx.print();
    while(!quit){
        printf("Enter command: ");
        std::getline (std::cin, input);
        if(!input.empty()){
            if(input[0] == 'q' || input[0] == 'Q'){
                quit = true;
                continue;
            }
            else if(input[0] == 's' || input[0] == 'S'){
                myPyraminx.scramble(20);
            }
            else{
                input_to_turn(myPyraminx, input);
            }
            printf("\n");
            myPyraminx.print();
        }
    }
}

void input_to_turn(Pyraminx& pyraminx, std::string turn){
    Corner input_corner;
    Direction dir = Direction::clockwise;

    int turn_length = turn.length();
    if(turn_length != 2 && turn_length != 3){
        printf("Invalid turn command.\n");
        return;
    }
    char corner_char = turn[0];
    int layer;
    try{
        layer = std::stoi(turn.substr(1,1));
    }
    catch(std::invalid_argument& e){
        printf("Invalid turn command. Second character must be 0, 1, 2, or 3.\n");
        return;
    }   
    if(layer < 0 || layer > 3){
        printf("Invalid layer number. Use integers 0, 1, 2, or 3.\n");
        return;
    }
    switch (corner_char)
    {
    case 'u':
    case 'U':
        input_corner = Corner::U;
        break;
    case 'l':
    case 'L':
        input_corner = Corner::L;
        break;
    case 'r':
    case 'R':
        input_corner = Corner::R;
        break;
    case 'b':
    case 'B':
        input_corner = Corner::B;
        break;
    default:
        printf("Invalid corner name. Use letters U, L, R, or B (case insensitive).\n");
        return;
        break;
    }
    if(turn_length == 3){
        if(turn[2] == '`'){
            dir = Direction::counterclockwise;
        }
        else{
            printf("Unknown character at end of turn command. Use ` (tick) to specify inverted/counterclockwise turn.\n");
            return;
        }
    }
    pyraminx.turn_layer(input_corner, layer, dir);
}