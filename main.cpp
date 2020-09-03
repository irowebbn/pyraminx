#include "Pyraminx.h"

int main(){
    Pyraminx myPyraminx;
    myPyraminx.print();
    // myPyraminx.turn(Corner::U, 3, Direction::clockwise);
    myPyraminx.turn(Corner::B, 3, Direction::clockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::B, 3, Direction::counterclockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::R, 0, Direction::clockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::R, 0, Direction::counterclockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::U, 2, Direction::counterclockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::U, 2, Direction::clockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::B, 3, Direction::clockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::R, 0, Direction::clockwise);
    myPyraminx.print();
    myPyraminx.turn(Corner::U, 3, Direction::clockwise);
    myPyraminx.print(); 
}