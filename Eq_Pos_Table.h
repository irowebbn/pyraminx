#ifndef EQ_POS_H
#define EQ_POS_H

#include "Face.h"
struct Position;
enum class Color;

class Eq_Pos_Table{
    public:
    Eq_Pos_Table();
    std::vector<Position> get_equivalent_positions(Color center_color, int flat_entry);
    private:
        std::vector< std::vector<Position> > blue_eq;
        std::vector< std::vector<Position> > yellow_eq;
        std::vector< std::vector<Position> > red_eq;
        std::vector< std::vector<Position> > green_eq;
};

#endif