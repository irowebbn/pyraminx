#ifndef EQ_POS_H
#define EQ_POS_H

#include "Face.h"
struct Position;
enum class Color;

class Eq_Pos_Table{
    public:
    Eq_Pos_Table();
    // Given a position, return the other possible names for it
    std::vector<Position> get_equivalent_positions(Color center_color, int flat_entry);
    private:
    // Each of these vectors have a list of equivalent 
    // positions for each spot on the face
        std::vector< std::vector<Position> > blue_eq;
        std::vector< std::vector<Position> > yellow_eq;
        std::vector< std::vector<Position> > red_eq;
        std::vector< std::vector<Position> > green_eq;
};

#endif