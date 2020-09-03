#include "Pyraminx.h"

int main(){
    Pyraminx myPyraminx;
    myPyraminx.print();
    myPyraminx.turn(Corner::U, 3, Direction::clockwise);
    myPyraminx.print();
}